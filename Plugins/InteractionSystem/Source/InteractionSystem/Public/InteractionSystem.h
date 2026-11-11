// Copyright Dredd 2026. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FInteractionSystemModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
