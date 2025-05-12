// Copyright EPortfolio


#include "Weapon/EShotgun.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/EProjectile.h"

void AEShotgun::Fire(const FVector& HitTarget)
{
	if (!GetWeaponMesh()) return;

	Super::Fire(HitTarget);

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("muzzle"));

	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FVector direction = ToTarget.GetSafeNormal();

		for (int i = 0; i < BulletSpreadCount; i++)
		{
			direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, StartSpreadAngle);

			FRotator TargetRotation = direction.Rotation();
			
			SpawnProjectile(SocketTransform.GetLocation(), TargetRotation);
		}
	}
}
