// Copyright 2018 Silvano Cerza All Rights Reserved.

#include "CuteCodeVisitor.h"
#include "Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogCuteCodeVisitor, Log, All);

#define LOCTEXT_NAMESPACE "FCuteCodeVisitor";

FCuteCodeVisitor::FCuteCodeVisitor(const FString& SolutionPath)
    : SolutionPath{SolutionPath}
{

}

FCuteCodeVisitor::~FCuteCodeVisitor()
{

}

bool FCuteCodeVisitor::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
    if (!bIsDirectory)
    {
        FString FileName = FString{FilenameOrDirectory}.Replace(*SolutionPath, TEXT("../.."));
        FPaths::NormalizeFilename(FileName);

        // Project and plugin files might not be inside a Source folder so we check it first
        if (FileName.EndsWith(".uproject") || FileName.EndsWith(".uplugin"))
        {
            OtherFiles.Add(FileName);
            return true;
        }

        FString SourcePath = FString{"/Source/"};
        FPaths::NormalizeFilename(SourcePath);
        // Skips directory not containing source files
        if (!FileName.Contains(SourcePath))
        {
            return true;
        }

        if (FileName.EndsWith(".h") || FileName.EndsWith(".hpp"))
        {
            Headers.Add(FileName);
        }
        else if (FileName.EndsWith(".c") || FileName.EndsWith(".cpp"))
        {
            Sources.Add(FileName);
        }
        else if (FileName.EndsWith(".cs"))
        {
            OtherFiles.Add(FileName);
        }
    }

    // NEVER STOP!
    return true;
}


#undef LOCTEXT_NAMESPACE
