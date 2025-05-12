// Copyright EPortfolio


#include "Weapon/ERifle.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/EProjectile.h"

void AERifle::Fire(const FVector& HitTarget)
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
