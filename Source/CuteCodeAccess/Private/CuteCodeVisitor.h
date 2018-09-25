#pragma once

#include "CoreMinimal.h"
#include "PlatformFile.h"

/// Visitor used to search for headers and sources, must be used
/// with FileManager
class FCuteCodeVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
    FCuteCodeVisitor(const FString& SolutionPath);
    ~FCuteCodeVisitor() override;

    bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;

    TArray<FString> GetSources() const { return Sources; }
    TArray<FString> GetHeaders() const { return Headers; }
    TArray<FString> GetOtherFiles() const { return OtherFiles; }

private:
    FString SolutionPath;

    TArray<FString> Sources;
    TArray<FString> Headers;
    TArray<FString> OtherFiles;
};
