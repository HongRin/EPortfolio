// Copyright EPortfolio


#include "AnimInstance/Player/EPlayerAnimInstance.h"
#include "AnimInstance/Player/EPlayerLinkedAnimLayer.h"
#include "Character/Player/EPlayer.h"
#include "Weapon/EWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"

void UEPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwnerCharacter)
	{
		Player = Cast<AEPlayer>(OwnerCharacter);
	}
}

void UEPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bElimmed) return;

	if (OwnerCharacter)
	{
		bIsJumpping = Velocity.Z > 0.f;
		bIsCrouched = OwnerCharacter->bIsCrouched;
		CharacterRotationLastFrame = CharacterRotation;
		CharacterRotation = OwnerCharacter->GetActorRotation();

		SetDirectionType();
		SetLean(DeltaSeconds);
		AimOffset(DeltaSeconds);
	}

	if (Player)
	{
		bIsAiming   = Player->IsAiming();
		bIsFiring   = Player->IsFiring();
		bIsEquipped = IsValid(Player->GetEquippedWeapon());
		bElimmed    = Player->IsElimmed();
		bUseFABRIK  = Player->GetCombatState() != ECombatState::ECS_Reloading && bIsEquipped;

		if (bIsEquipped && Player->GetMesh())
		{
			SetLeftHandTransform();
		}

		if (Player->GetAnimLayer() != ItemAnimLayer)
		{
			ItemAnimLayer = Player->GetAnimLayer();

			AsyncTask(ENamedThreads::GameThread, [this, DeltaSeconds]()
				{
					if (GetOwningComponent())
					{
						GetOwningComponent()->LinkAnimClassLayers(ItemAnimLayer);
					}
				});
		}
	}

}

void UEPlayerAnimInstance::SetDirectionType()
{
	FVector Velocity2D = FVector(Velocity.X, Velocity.Y, 0.f);

	DirectionAngle = UKismetMathLibrary::NormalizeAxis((UKismetAnimationLibrary::CalculateDirection(Velocity2D, CharacterRotation)));

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
}

void UEPlayerAnimInstance::SetLean(float DeltaSeconds)
{
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);
}

void UEPlayerAnimInstance::SetLeftHandTransform()
{
	LeftHandTransform = Player->GetEquippedWeapon()->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
	FVector  LeftOutPosition;
	FRotator LeftOutRotation;
	Player->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), LeftHandTransform.Rotator(), LeftOutPosition, LeftOutRotation);
	LeftHandTransform.SetLocation(LeftOutPosition);
	LeftHandTransform.SetRotation(FQuat(LeftOutRotation));
}

void UEPlayerAnimInstance::AimOffset(float DeltaSeconds)
{
	if (Speed == 0.f && !bIsInAir) 
	{
		FRotator CurrentAimRotation = FRotator(0.f, OwnerCharacter->GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AimOffsetYaw = DeltaAimRotation.Yaw;
		if (bIsTurn == false)
		{
			InterpAimOffsetYaw = AimOffsetYaw;
		}
		TurnInPlace(DeltaSeconds);
	}

	if (Speed > 0.f || bIsInAir || bIsFiring || bIsAiming)
	{
		StartingAimRotation = FRotator(0.f, OwnerCharacter->GetBaseAimRotation().Yaw, 0.f);
		AimOffsetYaw = 0.f;
		bIsTurn = false;
	}

	AimOffsetPitch = OwnerCharacter->GetBaseAimRotation().Pitch;
	if (AimOffsetPitch > 90.f && !OwnerCharacter->IsLocallyControlled())
	{
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AimOffsetPitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AimOffsetPitch);
	}
}

void UEPlayerAnimInstance::TurnInPlace(float DeltaSeconds)
{
	if (FMath::Abs(AimOffsetYaw) > 90.f)
	{
		bIsTurn = true;
	}

	if (bIsTurn)
	{
		InterpAimOffsetYaw = FMath::FInterpTo(InterpAimOffsetYaw, 0.f, DeltaSeconds, 4.f);
		AimOffsetYaw = InterpAimOffsetYaw;
		if (FMath::Abs(AimOffsetYaw) < 1.f)
		{
			bIsTurn = false;
			StartingAimRotation = FRotator(0.f, OwnerCharacter->GetBaseAimRotation().Yaw, 0.f);
		}
	}
}
