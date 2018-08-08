#include "CuteCodeAccessModule.h"
#include "Features/IModularFeatures.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FCuteCodeAccessModule, CuteCodeAccess);

#define LOCTEXT_NAMESPACE "FCuteCodeAccessModule"

FCuteCodeAccessModule::FCuteCodeAccessModule()
	: CuteCodeAccessor(MakeShareable(new FCuteCodeAccessor()))
{
}

void FCuteCodeAccessModule::StartupModule()
{
	CuteCodeAccessor->Startup();

	// Bind our source control provider to the editor
	IModularFeatures::Get().RegisterModularFeature(TEXT("SourceCodeAccessor"), &CuteCodeAccessor.Get());
}

void FCuteCodeAccessModule::ShutdownModule()
{
	// unbind provider from editor
	IModularFeatures::Get().UnregisterModularFeature(TEXT("SourceCodeAccessor"), &CuteCodeAccessor.Get());

	CuteCodeAccessor->Shutdown();
}

FCuteCodeAccessor& FCuteCodeAccessModule::GetAccessor()
{
	return CuteCodeAccessor.Get();
}

#undef LOCTEXT_NAMESPACE