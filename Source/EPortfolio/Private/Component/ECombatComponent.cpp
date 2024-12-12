// Copyright EPortfolio

#include "Component/ECombatComponent.h"
#include "EPortfolio/EPortfolio.h"
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
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

UECombatComponent::UECombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CrosshairBaseFactor = 2;
	HitDistance = 80000.f;
	RecoilAngle = 0.75f;
	bCanFire = true;
}

void UECombatComponent::BeginPlay()
{
	Super::BeginPlay();
	ItemAnimLayer = UnarmedAnimLayer;
}


void UECombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (Player && Player->IsLocallyControlled())
	{
		SetHUDCrosshairsSpread(DeltaTime);
	}
}

void UECombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UECombatComponent, bAiming);
	DOREPLIFETIME(UECombatComponent, EquippedWeapon);
	DOREPLIFETIME(UECombatComponent, ItemAnimLayer);
	DOREPLIFETIME(UECombatComponent, bFiring);


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

void UECombatComponent::PlayHitReactMontage()
{
	Player->PlayAnimMontage(HitReactMontage);
}

void UECombatComponent::SetAiming(bool bIsAiming)
{
	if (IsValid(EquippedWeapon))
	{
		if (Player->HasAuthority())
		{
			bAiming = bIsAiming;
		}
		else
		{
			ServerSetAiming(bIsAiming);
		}

		if (Player->IsLocallyControlled())
		{
			EquippedWeapon->Aimimg(bIsAiming);
		}

		CrosshairAimFactor = bIsAiming  ? -0.75f : 0.f;

	}
}

void UECombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}

void UECombatComponent::ServerSetFiring_Implementation(bool bIsFiring)
{
	bFiring = bIsFiring;
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

void UECombatComponent::Firing(bool bFireButtonPressed)
{
	if (IsValid(EquippedWeapon))
	{
		if (Player->HasAuthority())
		{
			bFiring = bFireButtonPressed;
		}
		else
		{
			ServerSetFiring(bFireButtonPressed);
		}
		OnFiring();
	}
}

void UECombatComponent::OnFiring()
{
	if (bCanFire)
	{
		bCanFire = false;
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		ServerFiring(HitResult.ImpactPoint);
		CrosshairShootFactor = 3.0f;

		Player->GetWorldTimerManager().SetTimer(
			FireTimer,
			this,
			&UECombatComponent::FiringTimerFunction,
			EquippedWeapon->GetAutoFireInterval()
		);
	}
}

void UECombatComponent::FiringTimerFunction()
{
	if (EquippedWeapon == nullptr) return;
	bCanFire = true;
	if (EquippedWeapon->GetAutomatic() && bFiring)
	{
		OnFiring();
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
		FVector Start = CrosshairWorldPosition + CrosshairWorldDirection;

		if (Player)
		{
			float DistanceToCharacter = (Player->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * DistanceToCharacter;
		}

		CrosshairWorldDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(CrosshairWorldDirection, RecoilAngle);

		FVector End = Start + CrosshairWorldDirection * HitDistance;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Player);
		QueryParams.AddIgnoredActor(EquippedWeapon);
		QueryParams.bReturnPhysicalMaterial = true;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECC_Shot,
			QueryParams
		);

		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
	}
}

void UECombatComponent::SetHUDCrosshairsSpread(float DeltaTime)
{
	if (Player == nullptr || Player->Controller == nullptr) return;

	Controller = (Controller == nullptr) ? Cast<AEPlayerController>(Player->Controller) : Controller.Get();
	HUD = (HUD == nullptr) ? Cast<AEHUD>(Controller->GetHUD()) : HUD.Get();
	if (Controller)
	{
		if (HUD)
		{
			FWeaponCrosshairData CrossHairData;
			if (EquippedWeapon)
			{
				CrossHairData.SetCrosshairTexture
				(
					EquippedWeapon->GetWeaponCrosshairData().CrosshairsCenter,
					EquippedWeapon->GetWeaponCrosshairData().CrosshairsLeft,
					EquippedWeapon->GetWeaponCrosshairData().CrosshairsRight,
					EquippedWeapon->GetWeaponCrosshairData().CrosshairsTop,
					EquippedWeapon->GetWeaponCrosshairData().CrosshairsBottom
				);
			}
			else
			{
				CrossHairData.SetCrosshairTexture();
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

			CrosshairShootFactor = FMath::FInterpTo(CrosshairShootFactor, 0.f, DeltaTime, 2.25f);

			CrossHairData.CrosshairSpread = CrosshairBaseFactor + CrosshairVelocityFactor + CrosshairInAirFactor + CrosshairAimFactor + CrosshairShootFactor;
			 
			HUD->SetCrosshairData(CrossHairData);
		}
	}
}




