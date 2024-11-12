// Copyright EPortfolio


#include "AnimInstance/Player/EPlayerAnimInstance.h"
#include "AnimInstance/Player/EPlayerLinkedAnimLayer.h"
#include "Character/Player/EPlayer.h"
#include "Weapon/EWeapon.h"

void UEPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UEPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerCharacter)
	{
		bIsJumpping = OwnerCharacter->GetVelocity().Z > 100;
		bIsCrouched = OwnerCharacter->bIsCrouched;
		if (AEPlayer* Player = Cast<AEPlayer>(OwnerCharacter))
		{
			bIsAiming = Player->IsAiming();

			AimOffsetYaw = Player->GetAimOffsetYaw();
			AimOffsetPitch = Player->GetAimOffsetPitch();
			EquippedWeapon = Player->GetEquippedWeapon();
			bIsEquipped = IsValid(EquippedWeapon);

			if (Player->GetAnimLayer() != ItemAnimLayer)
			{
				ItemAnimLayer = Player->GetAnimLayer();

				AsyncTask(ENamedThreads::GameThread, [this]()
					{
						if (GetOwningComponent())
						{
							GetOwningComponent()->LinkAnimClassLayers(ItemAnimLayer);
						}
					});
			}

			if (EquippedWeapon && EquippedWeapon->GetWeaponMesh() && Player->GetMesh())
			{
				LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
				FVector LeftOutPosition;
				FRotator LeftOutRotation;
				Player->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), LeftHandTransform.GetRotation().Rotator(), LeftOutPosition, LeftOutRotation);
				LeftHandTransform.SetLocation(LeftOutPosition);
				LeftHandTransform.SetRotation(FQuat(LeftOutRotation));
			}
		}
	}

	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}
