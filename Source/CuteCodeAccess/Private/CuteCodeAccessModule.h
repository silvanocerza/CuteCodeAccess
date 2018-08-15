#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "CuteCodeAccessor.h"

class FCuteCodeAccessModule : public IModuleInterface
{
public:
	FCuteCodeAccessModule();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FCuteCodeAccessor& GetAccessor();

protected:
	void RegisterSettings();
	void UnregisterSettings();

private:

	TSharedRef<FCuteCodeAccessor> CuteCodeAccessor;
};