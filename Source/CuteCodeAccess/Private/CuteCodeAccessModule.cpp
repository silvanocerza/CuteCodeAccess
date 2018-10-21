// Copyright 2018 Silvano Cerza All Rights Reserved.

#include "CuteCodeAccessModule.h"
#include "CuteCodeEditorSettings.h"
#include "Features/IModularFeatures.h"
#include "Modules/ModuleManager.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"

IMPLEMENT_MODULE(FCuteCodeAccessModule, CuteCodeAccess);

#define LOCTEXT_NAMESPACE "FCuteCodeAccessModule"

FCuteCodeAccessModule::FCuteCodeAccessModule()
    : CuteCodeAccessor(MakeShareable(new FCuteCodeAccessor()))
{
}

void FCuteCodeAccessModule::StartupModule()
{
    RegisterSettings();

    CuteCodeAccessor->Startup();

    // Bind our source control provider to the editor
    IModularFeatures::Get().RegisterModularFeature(TEXT("SourceCodeAccessor"), &CuteCodeAccessor.Get());
}

void FCuteCodeAccessModule::ShutdownModule()
{
    UnregisterSettings();

    // unbind provider from editor
    IModularFeatures::Get().UnregisterModularFeature(TEXT("SourceCodeAccessor"), &CuteCodeAccessor.Get());

    CuteCodeAccessor->Shutdown();
}

FCuteCodeAccessor& FCuteCodeAccessModule::GetAccessor()
{
    return CuteCodeAccessor.Get();
}

void FCuteCodeAccessModule::RegisterSettings()
{
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

    if (SettingsModule)
    {
        ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(
            "Editor", "Plugins", "CuteCodeAccessor",
            LOCTEXT("CuteCodeAccessSettingsName", "Cute Code Accessor"),
            LOCTEXT("CuteCodeAccessSettingsDescription", "Edit Cute Code Accessor settings"),
            GetMutableDefault<UCuteCodeEditorSettings>()
        );
    }
}

void FCuteCodeAccessModule::UnregisterSettings()
{
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

    if (SettingsModule)
    {
        SettingsModule->UnregisterSettings("Editor", "Plugins", "CuteCodeAccessor");
    }
}

#undef LOCTEXT_NAMESPACE
