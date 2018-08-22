#include "CuteCodeAccessor.h"
#include "CuteCodeAccessModule.h"
#include "CuteCodeInitializer.h"
#include "CuteCodeConstants.h"
#include "CuteCodeEditorSettings.h"
#include "Developer/SourceCodeAccess/Public/ISourceCodeAccessModule.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"
#include "Misc/ScopeLock.h"
#include "Misc/UProjectInfo.h"
#include "Misc/App.h"
#include "Misc/FileHelper.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#endif
#include "Internationalization/Regex.h"

DEFINE_LOG_CATEGORY_STATIC(LogCuteCodeAccessor, Log, All);

#define LOCTEXT_NAMESPACE "FCuteCodeAccessor"

static FString MakePath(const FString& InPath)
{
    return TEXT("\"") + InPath + TEXT("\"");
}

FString FCuteCodeAccessor::GetSolutionPath() const
{
    FScopeLock Lock(&CachedSolutionPathCriticalSection);

    if (IsInGameThread())
    {
        CachedSolutionPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());

        // Get project file path
        FString ProjectFile = FPaths::Combine(
            FPaths::ProjectDir(),
            INTERMEDIATE_PROJECTFILES,
            FApp::GetProjectName() + FString(".pro")
        );

        // Creates project files if necessary
        if (!FPaths::FileExists(ProjectFile))
        {
            FCuteCodeInitializer Initializer{FPaths::ProjectDir(), FApp::GetProjectName()};
            Initializer.Run();
        }
    }

    return CachedSolutionPath;
}

/** save all open documents in Qt Creator, when recompiling */
static void OnModuleCompileStarted(bool bIsAsyncCompile)
{
    FCuteCodeAccessModule& CuteCodeAccessModule = FModuleManager::LoadModuleChecked<FCuteCodeAccessModule>(TEXT("FCuteCodeAccessModule"));
    CuteCodeAccessModule.GetAccessor().SaveAllOpenDocuments();
}

void FCuteCodeAccessor::Startup()
{
    GetSolutionPath();
    RefreshAvailability();
}

void FCuteCodeAccessor::RefreshAvailability()
{
    const UCuteCodeEditorSettings* Settings = GetDefault<UCuteCodeEditorSettings>();

    FString URL;

    // If user has set a specific Qt Creator path use that, otherwise search it
    if (Settings && !Settings->QtCreatorPath.FilePath.IsEmpty())
    {
        URL = Settings->QtCreatorPath.FilePath;
    }
    else
    {
        FString IDEPath;

        FWindowsPlatformMisc::QueryRegKey(
            HKEY_CURRENT_USER,
            TEXT("SOFTWARE\\Classes\\Applications\\QtProject.QtCreator.pro\\shell\\Open\\Command"),
            TEXT(""),
            IDEPath
        );

        FRegexPattern Pattern("(.*) -");
        FRegexMatcher Matcher(Pattern, IDEPath);

        if (Matcher.FindNext())
        {
            URL = Matcher.GetCaptureGroup(1);
        }
    }

    if (!URL.IsEmpty() && FPaths::FileExists(URL))
    {
        Location.URL = FPaths::ConvertRelativePathToFull(URL);
        UE_LOG(LogCuteCodeAccessor, Log, TEXT("Qt Creator found at \"%s\""), *URL);
        return;
    }

    UE_LOG(LogCuteCodeAccessor, Error, TEXT("Qt Creator not found"));
}

void FCuteCodeAccessor::Shutdown()
{
}

bool FCuteCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths)
{
    if (Location.IsValid())
    {
        FString SolutionDir = GetSolutionPath();
        TArray<FString> Args;

        for (const FString& SourcePath : AbsoluteSourcePaths)
        {
            Args.Add(MakePath(SourcePath));
        }

        return Launch(Args);
    }

    return false;
}

bool FCuteCodeAccessor::AddSourceFiles(const TArray<FString>& AbsoluteSourcePaths, const TArray<FString>& AvailableModules)
{
    FString ProjectFile = FPaths::Combine(
        FPaths::ProjectDir(),
        INTERMEDIATE_PROJECTFILES,
        FApp::GetProjectName() + FString(".pro")
    );

    // Files can't be added if .pro file doesn't exist
    if (!FPaths::FileExists(ProjectFile))
    {
        return false;
    }

    // Gets project directory to get relative path from .pro file
    FString ProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
    FPaths::NormalizeDirectoryName(ProjectDir);

    // Separates Headers and Sources into different Arrays and formats paths
    TArray<FString> Headers, Sources;
    for (FString FilePath : AbsoluteSourcePaths)
    {
        FPaths::NormalizeDirectoryName(FilePath);
        FilePath = FilePath.RightChop(ProjectDir.Len());

        const FString& Extension = FPaths::GetExtension(FilePath).ToLower();
        const FString& FormattedPath = FString::Format(TEXT("../..{0} \\"), {FilePath});
        if (Extension.Contains(TEXT("c")))
        {
            Sources.Add(FormattedPath);
        }
        else if (Extension.Contains(TEXT("h")))
        {
            Headers.Add(FormattedPath);
        }
        else
        {
            UE_LOG(LogCuteCodeAccessor, Warning, TEXT("Ignoring unknown extension %s"), *Extension);
        }
    }

    // Reads .pro file
    TArray<FString> ProFileLines;
    FFileHelper::LoadFileToStringArray(ProFileLines, *ProjectFile);

    // Inserts Headers and Sources file paths
    for (int32 i = 0; i < ProFileLines.Num(); i++)
    {
        const FString& Line = ProFileLines[i];
        if (Line.Contains("HEADERS", ESearchCase::CaseSensitive))
        {
            ProFileLines.Insert(Headers, i + 1);
        }
        else if (Line.Contains("SOURCES", ESearchCase::CaseSensitive))
        {
            ProFileLines.Insert(Sources, i + 1);
        }
    }

    // Saves updated .pro file
    return FFileHelper::SaveStringArrayToFile(ProFileLines, *ProjectFile);
}

bool FCuteCodeAccessor::OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber)
{
    if (Location.IsValid())
    {
        // Column & line numbers are 1-based, so dont allow zero
        LineNumber = LineNumber > 0 ? LineNumber : 1;
        ColumnNumber = ColumnNumber > 0 ? ColumnNumber : 1;

        FString SolutionDir = GetSolutionPath();
        TArray<FString> Args;
        Args.Add(MakePath(SolutionDir));
        Args.Add(MakePath(FullPath) + FString::Printf(TEXT(":%d:%d"), LineNumber, ColumnNumber));
        return Launch(Args);
    }

    return false;
}

bool FCuteCodeAccessor::CanAccessSourceCode() const
{
    // True if we have any versions of Qt Creator installed
    return Location.IsValid();
}

FName FCuteCodeAccessor::GetFName() const
{
    return FName("CuteCodeAccess");
}

FText FCuteCodeAccessor::GetNameText() const
{
    return LOCTEXT("CuteCodeDisplayName", "Cute Code Accessor");
}

FText FCuteCodeAccessor::GetDescriptionText() const
{
    return LOCTEXT("CuteCodeDisplayDesc", "Open source code accessor for Qt Creator.");
}

void FCuteCodeAccessor::Tick(const float DeltaTime)
{
    // noop
}

bool FCuteCodeAccessor::OpenSolution()
{
    if (Location.IsValid())
    {
        return OpenSolutionAtPath(GetSolutionPath());
    }

    return false;
}

bool FCuteCodeAccessor::OpenSolutionAtPath(const FString& InSolutionPath)
{
    if (Location.IsValid())
    {
        FString SolutionPath = InSolutionPath;

        if (!SolutionPath.EndsWith(INTERMEDIATE_PROJECTFILES))
        {
            SolutionPath = SolutionPath + INTERMEDIATE_PROJECTFILES;
        }

        TArray<FString> Args;
        Args.Add(MakePath(SolutionPath));
        return Launch(Args);
    }

    return false;
}

bool FCuteCodeAccessor::DoesSolutionExist() const
{
    return FPaths::FileExists(GetSolutionPath());
}


bool FCuteCodeAccessor::SaveAllOpenDocuments() const
{
    // There's no way that I know of to save all open files in Qt Creator
    STUBBED("FCuteCodeAccessor::SaveAllOpenDocuments");
    return false;
}

bool FCuteCodeAccessor::Launch(const TArray<FString>& InArgs)
{
    if (Location.IsValid())
    {
        FString ArgsString;
        // -client attempts to connect to already running Qt Creator instance
        ArgsString.Append(TEXT("-client "));

        // Adds solution path to args in case it's not already open
        ArgsString.Append(MakePath(FPaths::Combine(GetSolutionPath(), INTERMEDIATE_PROJECTFILES)));
        ArgsString.Append(TEXT(" "));

        for (const FString& Arg : InArgs)
        {
            ArgsString.Append(Arg);
            ArgsString.Append(TEXT(" "));
        }

        FProcHandle hProcess = FPlatformProcess::CreateProc(*Location.URL, *ArgsString, true, false, false, nullptr, 0, nullptr, nullptr, nullptr);
        return hProcess.IsValid();
    }

    return false;
}

#undef LOCTEXT_NAMESPACE
