#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETurnType : uint8
{
	T_Left UMETA(DisplayName = "Turning Left"),
	T_Right UMETA(DisplayName = "Turning Right"),
	T_Not UMETA(DisplayName = "Not Turning"),
	T_MAX UMETA(DisplayName = "DefaultMAX")
};