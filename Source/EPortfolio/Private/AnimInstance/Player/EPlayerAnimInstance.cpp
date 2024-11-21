// Copyright EPortfolio


#include "AnimInstance/Player/EPlayerAnimInstance.h"
#include "AnimInstance/Player/EPlayerLinkedAnimLayer.h"
#include "Character/Player/EPlayer.h"
#include "Weapon/EWeapon.h"
#include "Kismet/KismetMathLibrary.h"

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

		FVector Velocity2D = FVector(OwnerCharacter->GetVelocity().X, OwnerCharacter->GetVelocity().Y, 0.f);

		DirectionAngle = UKismetMathLibrary::NormalizeAxis((CalculateDirection(Velocity2D, OwnerCharacter->GetActorRotation())));

		if (UKismetMathLibrary::InRange_FloatFloat(DirectionAngle, -45, 45, false, true))
		{
			DirectionType = EDirectionType::Forward;
		}
		else if (UKismetMathLibrary::InRange_FloatFloat(DirectionAngle, 45, 135, false, true))
		{
			DirectionType = EDirectionType::Right;
			DirectionAngle -= 90.f;
		}
		else if (DirectionAngle <= -135 || DirectionAngle > 135)
		{
			DirectionType = EDirectionType::Backward;
			DirectionAngle -= 180.f;
		}
		else if (UKismetMathLibrary::InRange_FloatFloat(DirectionAngle, -135, -45, false, true))
		{
			DirectionType = EDirectionType::Left;
			DirectionAngle -= -90.f;
		}


		CharacterRotationLastFrame = CharacterRotation;
		CharacterRotation = OwnerCharacter->GetActorRotation();
		const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
		const float Target = Delta.Yaw / DeltaSeconds;
		const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.f);
		Lean = FMath::Clamp(Interp, -90.f, 90.f);

		if (AEPlayer* Player = Cast<AEPlayer>(OwnerCharacter))
		{
			bIsAiming = Player->IsAiming();
			AimOffsetYaw = Player->GetAimOffsetYaw();
			AimOffsetPitch = Player->GetAimOffsetPitch();
			EquippedWeapon = Player->GetEquippedWeapon();
			bIsEquipped = IsValid(EquippedWeapon);
			TurnType = Player->GetTurnType();

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
				Player->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), LeftHandTransform.Rotator(), LeftOutPosition, LeftOutRotation);
				LeftHandTransform.SetLocation(LeftOutPosition);
				LeftHandTransform.SetRotation(FQuat(LeftOutRotation));
			}
		}
	}

	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}
