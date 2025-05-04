// Copyright EPortfolio


#include "Weapon/EProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/EProjectile.h"

void AEProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	if (GetWeaponMesh())
	{
		const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("muzzle"));

		if (MuzzleFlashSocket)
		{
			FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
			FVector ToTarget = HitTarget - SocketTransform.GetLocation();
			FRotator TargetRotation = ToTarget.Rotation();
			if (ProjectileClass && InstigatorPawn)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = InstigatorPawn;
				UWorld* World = GetWorld();
				if (World)
				{
					World->SpawnActor<AEProjectile>(
						ProjectileClass,
						SocketTransform.GetLocation(),
						TargetRotation,
						SpawnParams
					);
				}
			}
		}
	}
}
