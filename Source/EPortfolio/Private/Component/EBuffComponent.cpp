// Copyright EPortfolio


#include "Component/EBuffComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Character/Player/EPlayer.h"

UEBuffComponent::UEBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UEBuffComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UEBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ShieldRecovery(DeltaTime);
	HealthRecovery(DeltaTime);

}

void UEBuffComponent::PlayPickupEffect(UNiagaraSystem* PickupEffect)
{
	MulticastPickupEffect(PickupEffect);
}

void UEBuffComponent::ReplenishShield(float InShieldAmount)
{
	bReplenishingShield = true;
	ShieldReplenishRate = InShieldAmount / ShieldReplenishTime;
	ShieldAmount += InShieldAmount;
}

void UEBuffComponent::Heal(float InHealAmount)
{
	bHealing = true;
	HealingRate = InHealAmount / HealingTime;
	HealAmount += InHealAmount;
}

void UEBuffComponent::ShieldRecovery(float DeltaTime)
{
	if (!bReplenishingShield || Player == nullptr || Player->IsElimmed()) return;

	const float ReplenishThisFrame = ShieldReplenishRate * DeltaTime;
	Player->SetShield(FMath::Clamp(Player->GetShield() + ReplenishThisFrame, 0.f, Player->GetMaxShield()));
	Player->UpdateShield();
	ShieldAmount -= ReplenishThisFrame;

	if (ShieldAmount <= 0.f || Player->GetShield() >= Player->GetMaxShield())
	{
		bReplenishingShield = false;
		ShieldAmount = 0.f;
	}
}


void UEBuffComponent::HealthRecovery(float DeltaTime)
{
	if (!bHealing || Player == nullptr || Player->IsElimmed()) return;

	const float HealThisFrame = HealingRate * DeltaTime;
	Player->SetHealth(FMath::Clamp(Player->GetHealth() + HealThisFrame, 0.f, Player->GetMaxHealth()));
	Player->UpdateHealth();
	HealAmount -= HealThisFrame;

	if (HealAmount <= 0.f || Player->GetHealth() >= Player->GetMaxHealth())
	{
		bHealing = false;
		HealAmount = 0.f;
	}
}

void UEBuffComponent::MulticastPickupEffect_Implementation(UNiagaraSystem* PickupEffect)
{
	UNiagaraFunctionLibrary::SpawnSystemAttached
	(
		PickupEffect,
		Player->GetMesh(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true
	);
}

