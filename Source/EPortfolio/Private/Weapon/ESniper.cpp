// Copyright EPortfolio

#include "Weapon/ESniper.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/EProjectile.h"
#include "Controller/EPlayerController.h"

#include "ELogHelpers.h"

void AESniper::Fire(const FVector& HitTarget)
{
	if (!GetWeaponMesh()) return;

	Super::Fire(HitTarget);

	{
		const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("muzzle"));

		if (MuzzleFlashSocket)
		{
			FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
			FVector ToTarget = HitTarget - SocketTransform.GetLocation();
			FRotator TargetRotation = ToTarget.Rotation();

			SpawnProjectile(SocketTransform.GetLocation(), TargetRotation);
		}
	}
}

void AESniper::Aimimg(bool bAiming)
{
	Super::Aimimg(bAiming);

	if (OwnerController)
	{
		OwnerController->UpdateSniperScopeHUD(bAiming);
	}
}
