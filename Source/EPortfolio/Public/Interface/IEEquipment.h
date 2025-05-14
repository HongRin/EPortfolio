// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEEquipment.generated.h"

UINTERFACE(MinimalAPI)
class UIEEquipment : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EEquipmentType: uint8
{
	WS_Weapon   UMETA(DisplayName = "Weapon"),
	WS_Ammo     UMETA(DisplayName = "Ammo"),
	WS_MAX      UMETA(DisplayName = "DefaultMAX")
};

class EPORTFOLIO_API IIEEquipment
{
	GENERATED_BODY()

public:
	virtual EEquipmentType GetEquipmentType() = 0;
	virtual void ShowPickupWidget(bool bShow) = 0;
};
