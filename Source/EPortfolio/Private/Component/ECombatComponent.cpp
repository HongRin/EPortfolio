// Copyright EPortfolio


#include "Component/ECombatComponent.h"
#include "Weapon/EWeapon.h"
#include "AnimInstance/Player/EPlayerLinkedAnimLayer.h"
#include "AnimInstance/Player/EPlayerAnimInstance.h"
#include "Character/Player/EPlayer.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

UECombatComponent::UECombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UECombatComponent::BeginPlay()
{
	Super::BeginPlay();
	ItemAnimLayer = UnarmedAnimLayer;
}


void UECombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UECombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UECombatComponent, bAiming);
	DOREPLIFETIME(UECombatComponent, EquippedWeapon);
	DOREPLIFETIME(UECombatComponent, ItemAnimLayer);

}
void UECombatComponent::EquipWeapon(AEWeapon* WeaponToEquip)
{
	if (Player == nullptr || WeaponToEquip == nullptr) return;
	EquippedWeapon = WeaponToEquip;

	EquippedWeapon->SetWeaponState(EWeaponState::WS_Equipped);
	const USkeletalMeshSocket* HandSocket = Player->GetMesh()->GetSocketByName(FName("hand_r_weapon"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Player->GetMesh());
	}
	EquippedWeapon->SetOwner(Player);
	EquippedWeapon->ShowPickupWidget(false);
}

void UECombatComponent::SetAiming(bool bIsAiming)
{
	if (Player->HasAuthority())
	{
		bAiming = bIsAiming;
	}
	else
	{
		ServerSetAiming(bIsAiming);
	}
}

void UECombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}

void UECombatComponent::SetItemAnimLayer()
{
	if (EquippedWeapon)
	{
		ItemAnimLayer = EquippedWeapon->GetWeaponAnimLayerClass();
	}
}

void UECombatComponent::ServerSetItemAnimLayer_Implementation()
{
	if (EquippedWeapon)
	{
		ItemAnimLayer = EquippedWeapon->GetWeaponAnimLayerClass();
	}
}




