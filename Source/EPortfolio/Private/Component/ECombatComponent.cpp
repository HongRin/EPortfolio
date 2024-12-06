// Copyright EPortfolio


#include "Component/ECombatComponent.h"
#include "Weapon/EWeapon.h"
#include "AnimInstance/Player/EPlayerLinkedAnimLayer.h"
#include "AnimInstance/Player/EPlayerAnimInstance.h"
#include "Character/Player/EPlayer.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Controller/EPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/EHUD.h"
#include "Camera/CameraComponent.h"

UECombatComponent::UECombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UECombatComponent::BeginPlay()
{
	Super::BeginPlay();
	ItemAnimLayer = UnarmedAnimLayer;

	if (Player->GetFollowCamera())
	{
		DefaultFOV = Player->GetFollowCamera()->FieldOfView;
		CurrentFOV = DefaultFOV;
	}
}


void UECombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (Player && Player->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}
}

void UECombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UECombatComponent, bAiming);
	DOREPLIFETIME(UECombatComponent, EquippedWeapon);
	DOREPLIFETIME(UECombatComponent, ItemAnimLayer);

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

void UECombatComponent::SetAiming(bool bIsAiming)
{
	if (Player->HasAuthority())
	{
		bAiming = bIsAiming;
	}
	else
	{
		ServerSetAiming(bIsAiming);
	}
}

void UECombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}

void UECombatComponent::SetItemAnimLayer()
{
	if (EquippedWeapon)
	{
		ItemAnimLayer = EquippedWeapon->GetWeaponAnimLayerClass();
	}
}

void UECombatComponent::ServerSetItemAnimLayer_Implementation()
{
	if (EquippedWeapon)
	{
		ItemAnimLayer = EquippedWeapon->GetWeaponAnimLayerClass();
	}
}

void UECombatComponent::Firing()
{
	if (Player == nullptr) return;

	if (IsValid(EquippedWeapon))
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		ServerFiring(HitResult.ImpactPoint);
	}
}

void UECombatComponent::ServerFiring_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastFiring(TraceHitTarget);
}

void UECombatComponent::MulticastFiring_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	Player->PlayAnimMontage(EquippedWeapon->GetWeaponFireMontage());
	EquippedWeapon->Fire(TraceHitTarget);
}


void UECombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		FVector End = Start + CrosshairWorldDirection * 80000.f;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
		else
		{
			DrawDebugSphere(
				GetWorld(),
				TraceHitResult.ImpactPoint,
				12.f,
				12,
				FColor::Red
			);
		}
	}
}

void UECombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (Player == nullptr || Player->Controller == nullptr) return;

	Controller = (Controller == nullptr) ? Cast<AEPlayerController>(Player->Controller) : Controller.Get();
	if (Controller)
	{
		HUD = (HUD == nullptr) ? Cast<AEHUD>(Controller->GetHUD()) : HUD.Get();
		if (HUD)
		{
			FHUDPackage HUDPackage;
			if (EquippedWeapon)
			{
				HUDPackage.SetCrossHairTexture(EquippedWeapon->CrosshairsCenter, EquippedWeapon->CrosshairsLeft, EquippedWeapon->CrosshairsRight, EquippedWeapon->CrosshairsBottom, EquippedWeapon->CrosshairsTop);
			}
			else
			{
				HUDPackage.SetCrossHairTexture();
			}

			FVector2D WalkSpeedRange(0.f, Player->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = Player->GetVelocity();
			Velocity.Z = 0.f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (Player->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
			}

			HUDPackage.CrosshairSpread = CrosshairVelocityFactor + CrosshairInAirFactor + 2;

			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UECombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr) return;

	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime, EquippedWeapon->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}

	if (Player && Player->GetFollowCamera())
	{
		Player->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}
}





