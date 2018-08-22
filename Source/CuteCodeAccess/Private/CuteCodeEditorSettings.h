#pragma once

#include "Runtime/Engine/Classes/Engine/EngineTypes.h"

#include "CuteCodeEditorSettings.generated.h"

UCLASS(config=Editor)
class UCuteCodeEditorSettings : public UObject
{
    GENERATED_BODY()
public:
    UCuteCodeEditorSettings();
    virtual ~UCuteCodeEditorSettings() {}

    UPROPERTY(config, EditAnywhere, meta = (FilePathFilter = "exe"))
    FFilePath QtCreatorPath;

    UPROPERTY(config, EditAnywhere)
    FString UnrealKitName;
};