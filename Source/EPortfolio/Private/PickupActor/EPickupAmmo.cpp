// Copyright EPortfolio


#include "PickupActor/EPickupAmmo.h"
#include "Character/Player/EPlayer.h"
#include "Component/ECombatComponent.h"

void AEPickupAmmo::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (Player == nullptr) return;
 
	if (UECombatComponent* Combat = Player->GetCombatComponent())
	{
		Combat->PickupAmmo(WeaponType, AmmoAmount);
	}

	Destroy();
}
