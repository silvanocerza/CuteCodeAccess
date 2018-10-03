// Copyright 2018 Silvano Cerza All Rights Reserved.

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
    FFilePath QtCreatorPath{""};

    // Sets a default kit name to search in Qt Creator configs
    UPROPERTY(config, EditAnywhere)
    FString UnrealKitName = "Unreal Engine 4";
};
