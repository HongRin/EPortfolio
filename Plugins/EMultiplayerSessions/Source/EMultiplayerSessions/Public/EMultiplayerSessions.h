// Copyright Epic Games, Inc. All Rights Reserved.

// Docs : https://velog.io/@cle_0324/FEMultiplayerSessionsModule

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"



class FEMultiplayerSessionsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
