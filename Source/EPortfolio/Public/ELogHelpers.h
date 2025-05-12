// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"

class EPORTFOLIO_API ELogHelpers
{
public :
	static void PrintNet(AActor* InActor, FString PrintStr = FString());
	static void PrintIsValid(UObject* InObj, const WIDECHAR* InName);
	static void PrintFunctionCall();

};
