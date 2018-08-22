#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "CuteCodeXmlCallbacks.h"

class FCuteCodeInitializer
{
public:
    FCuteCodeInitializer(const FString& SolutionPath, const FString& ProjectName);
    ~FCuteCodeInitializer();

    void Run() const;

private:
    void CreateProFile() const;
    void CreatePriFiles() const;
    void CreateProUserFile() const;

    FString SolutionPath;
    FString ProjectName;

    FCuteCodeVCProjXmlCallback* VCProjXmlCallback;

    void AppendFormattedStrings(TArray<FString>& Array, const FString& Formatter, const TArray<FString>& ToAppend) const;
};