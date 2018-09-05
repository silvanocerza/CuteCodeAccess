#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "CuteCodeXmlCallbacks.h"
#include "CuteCodeVisitor.h"

class FCuteCodeInitializer
{
public:
    FCuteCodeInitializer(const FString& SolutionPath, const FString& ProjectName);
    ~FCuteCodeInitializer();

    void Run() const;

private:
    void CreateProFile() const;
    void CreateHeadersPriFile() const;
    void CreateSourcesPriFile() const;
    void CreateIncludesPriFile() const;
    void CreateDefinesPriFile() const;
    void CreateProUserFile() const;

    FString SolutionPath;
    FString ProjectName;

    FCuteCodeVCProjXmlCallback* VCProjXmlCallback;
    FCuteCodeVisitor ProjectVisitor;

    void AppendFormattedStrings(TArray<FString>& Array, const FString& Formatter, const TArray<FString>& ToAppend) const;
};
