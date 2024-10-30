// Copyright EPortfolio


#include "Component/ECombatComponent.h"
#include "Weapon/EWeapon.h"
#include "Character/Player/EPlayer.h"
#include "Engine/SkeletalMeshSocket.h"

UECombatComponent::UECombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UECombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UECombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

