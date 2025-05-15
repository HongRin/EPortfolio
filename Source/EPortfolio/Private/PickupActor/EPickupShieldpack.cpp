// Copyright EPortfolio


#include "PickupActor/EPickupShieldpack.h"
#include "Component/EBuffComponent.h"
#include "Character/Player/EPlayer.h"

void AEPickupShieldpack::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


	if (Player == nullptr) return;

	if (UEBuffComponent* BuffComponent = Player->GetBuffComponent())
	{
		BuffComponent->ReplenishShield(ShieldAmount);
	}

	Destroy();
}
