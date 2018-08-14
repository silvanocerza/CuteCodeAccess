#include "CuteCodeInitializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogCuteCodeInitializer, Log, All);

#define LOCTEXT_NAMESPACE "FCuteCodeInitializer"


bool FCuteCodeFastXmlCallback::ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue)
{
	if (CurrentElementName == "ClCompile" && AttributeName == FString{"Include"}) 
	{
		Sources.Add(AttributeValue);
	}
	else if (CurrentElementName == "ClInclude" && AttributeName == FString{"Include"})
	{
		Headers.Add(AttributeValue);
	}
	return true;
}

bool FCuteCodeFastXmlCallback::ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber)
{
	CurrentElementName = ElementName;

	if (CurrentElementName == "NMakePreprocessorDefinitions")
	{
		Defines = ElementData;
	}
	else if (CurrentElementName == "NMakeIncludeSearchPath")
	{
		Includes = ElementData;
	}
	return true;
}

bool FCuteCodeFastXmlCallback::ProcessClose(const TCHAR* Element)
{
	CurrentElementName = "";
	return true;
}

bool FCuteCodeFastXmlCallback::ProcessComment(const TCHAR* Comment)
{
	return true;
}

bool FCuteCodeFastXmlCallback::ProcessXmlDeclaration(const TCHAR * ElementData, int32 XmlFileLineNumber)
{
	return true;
}

FCuteCodeInitializer::FCuteCodeInitializer(const FString& SolutionPath, const FString& ProjectName)
	: SolutionPath{SolutionPath}
	, ProjectName{ProjectName}
	, FastXmlCallback{new FCuteCodeFastXmlCallback}
{
	FText OutErrorMessage;
	int32 OutErrorLineNumber = -1;

	FString VcxProjFile = FPaths::Combine(
		SolutionPath, 
		TEXT("Intermediate/ProjectFiles"),
		ProjectName + TEXT(".vcxproj")
	);

	FFastXml::ParseXmlFile(
		FastXmlCallback,
		*VcxProjFile,
		TEXT(""),
		nullptr,
		false,
		false,
		OutErrorMessage,
		OutErrorLineNumber
	);

	if (!OutErrorMessage.IsEmpty())
	{
		UE_LOG(LogCuteCodeInitializer, Error, TEXT("Error parsing .vcxproj file at line: %d %s"),
			OutErrorLineNumber, *OutErrorMessage.ToString());
	}
}

FCuteCodeInitializer::~FCuteCodeInitializer()
{
	delete FastXmlCallback;
}

void FCuteCodeInitializer::Run() const
{
	CreateProFile();
	// TODO: Create pri files
}

void FCuteCodeInitializer::CreateProFile() const
{
	TArray<FString> ProFileLines{{
		"TEMPLATE = app",
		"",
		"CONFIG += console c++11",
		"CONFIG -= app_bundle qt",
		"",
		"include(defines.pri)",
		"include(includes.pri)",
		""
	}};

	ProFileLines.Add("HEADERS += \\");
	AppendFileNames(ProFileLines, FastXmlCallback->GetHeaders());

	ProFileLines.Add("");

	ProFileLines.Add("SOURCES += \\");
	AppendFileNames(ProFileLines, FastXmlCallback->GetSources());

	FString ProFilePath = FPaths::Combine(
		SolutionPath,
		TEXT("Intermediate/ProjectFiles"),
		ProjectName + ".pro"
	);

	FFileHelper::SaveStringArrayToFile(ProFileLines, *ProFilePath);
}

void FCuteCodeInitializer::AppendFileNames(TArray<FString>& Array, const TArray<FString>& FileNames) const
{
	for (int32 i = 0; i < FileNames.Num() - 1; i++)
	{
		Array.Add(FString::Format(TEXT("{0} \\"), { FileNames[i] }));
	}
	Array.Add(FileNames.Last());
}

#undef LOCTEXT_NAMESPACE
