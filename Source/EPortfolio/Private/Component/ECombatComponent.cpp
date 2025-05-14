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
#include "ELogHelpers.h"

UECombatComponent::UECombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CrosshairBaseFactor = 2;
	HitDistance = 80000.f;
	bCanFire = true;
}

void UECombatComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemAnimLayer = UnarmedAnimLayer;

	if (Player)
	{
		if (Player->HasAuthority())
		{
			InitializeCarriedAmmo();
		}
	}
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
	DOREPLIFETIME_CONDITION(UECombatComponent, CarriedAmmo, COND_OwnerOnly);
	DOREPLIFETIME(UECombatComponent, CombatState);

}
void UECombatComponent::EquipWeapon(AEWeapon* WeaponToEquip)
{
	if (Player == nullptr || WeaponToEquip == nullptr) return;
	
	if (EquippedWeapon)
	{
		EquippedWeapon->Dropped();
	}
	
	EquippedWeapon = WeaponToEquip;

	EquippedWeapon->SetWeaponState(EWeaponState::WS_Equipped);
	const USkeletalMeshSocket* HandSocket = Player->GetMesh()->GetSocketByName(FName("hand_r_weapon"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Player->GetMesh());
	}
	EquippedWeapon->SetOwner(Player);
	
	EquippedWeapon->SetHUDAmmo();
	EquippedWeapon->ShowPickupWidget(false);

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	if (EquippedWeapon->IsAmmoEmpty())
	{
		Reload();
	}

	Controller = Controller == nullptr ? Cast<AEPlayerController>(Player->Controller) : Controller.Get();
	if (Controller)
	{
		Controller->UpdateCarriedAmmoHUD(CarriedAmmo);
	}
}

void UECombatComponent::PlayHitReactMontage(const FVector2D Direction)
{
	if (Direction.Y > 0)
	{
		Player->PlayAnimMontage(HitReactMontages.Forward);
	}
	else if (Direction.Y < 0)
	{
		Player->PlayAnimMontage(HitReactMontages.Backward);
	}
	else if (Direction.X > 0)
	{
		Player->PlayAnimMontage(HitReactMontages.Right);
	}
	else if (Direction.X < 0)
	{
		Player->PlayAnimMontage(HitReactMontages.Left);
	}

}

void UECombatComponent::PlayDodgeMontage(const FVector2D Direction)
{

}

void UECombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Player)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::WS_Equipped);
		const USkeletalMeshSocket* HandSocket = Player->GetMesh()->GetSocketByName(FName("hand_r_weapon"));
		if (HandSocket)
		{
			HandSocket->AttachActor(EquippedWeapon, Player->GetMesh());
		}
	}
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

		if (bIsAiming)
		{
			CrosshairAimFactor = EquippedWeapon->GetWeaponCrosshairData().CrosshairAimSpread;
			SetAimMouseSensitivity();
		}
		else
		{
			CrosshairAimFactor = 0.f;
			MouseSensitivity = 1.f;
		}
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
	if (EquippedWeapon == nullptr) return;

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

void UECombatComponent::OnFiring()
{
	if (CanFire())
	{
		if (Player == nullptr) return;

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

	if (EquippedWeapon->IsAmmoEmpty())
	{
		Reload();
	}
}

void UECombatComponent::ServerFiring_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastFiring(TraceHitTarget);
}

void UECombatComponent::MulticastFiring_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (CombatState == ECombatState::ECS_Unoccupied && EquippedWeapon != nullptr && Player != nullptr)
	{
		Player->PlayAnimMontage(EquippedWeapon->GetWeaponFireMontage());
		EquippedWeapon->Fire(TraceHitTarget);
	}
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
	if (Controller)
	{
		HUD = (HUD == nullptr) ? Cast<AEHUD>(Controller->GetHUD()) : HUD.Get();
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

bool UECombatComponent::CanFire()
{
	if (EquippedWeapon == nullptr) return false;
	return !EquippedWeapon->IsAmmoEmpty() && bCanFire && CombatState == ECombatState::ECS_Unoccupied;

}

void UECombatComponent::OnRep_CarriedAmmo()
{
	Controller = Controller == nullptr ? Cast<AEPlayerController>(Player->Controller) : Controller.Get();
	if (Controller)
	{
		Controller->UpdateCarriedAmmoHUD(CarriedAmmo);
	}
}

void UECombatComponent::Reload()
{
	if (CarriedAmmo > 0 && CombatState == ECombatState::ECS_Unoccupied && EquippedWeapon->GetAmmo() != EquippedWeapon->GetMagazineCapacity())
	{
		ServerReload();
	}
}

void UECombatComponent::ServerReload_Implementation()
{
	if (Player == nullptr) return;
	CombatState = ECombatState::ECS_Reloading;
	ReloadHandle();
}

void UECombatComponent::InitializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::WT_Rifle, StartingAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::WT_Shotgun, StartingAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::WT_Rifle, StartingAmmo);
}

void UECombatComponent::ReloadHandle()
{
	if (EquippedWeapon)
	{
		Player->PlayAnimMontage(EquippedWeapon->GetWeaponReloadMontage());
	}
}

int32 UECombatComponent::AmountToReload()
{
	if (EquippedWeapon == nullptr) return 0;
	int32 RoomInMag = EquippedWeapon->GetMagazineCapacity() - EquippedWeapon->GetAmmo();

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		int32 Least = FMath::Min(RoomInMag, AmountCarried);
		return FMath::Clamp(RoomInMag, 0, Least);
	}
	return 0;
}


void UECombatComponent::FinsishedReload()
{
	if (Player == nullptr) return;
	if (Player->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied;
		UpdateAmmoValues();
	}

	if (bFiring)
	{
		OnFiring();
	}
}

void UECombatComponent::OnRep_CombatState()
{
	switch (CombatState)
	{
	case ECombatState::ECS_Reloading:
		ReloadHandle();
		break;

	case ECombatState::ECS_Unoccupied:
		if (bFiring)
		{
			OnFiring();
		}
		break;
	}
}

void UECombatComponent::UpdateAmmoValues()
{
	if (Player == nullptr || EquippedWeapon == nullptr) return;
	int32 ReloadAmount = AmountToReload();
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= ReloadAmount;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}
	Controller = Controller == nullptr ? Cast<AEPlayerController>(Player->Controller) : Controller.Get();
	if (Controller)
	{
		Controller->UpdateCarriedAmmoHUD(CarriedAmmo);
	}
	EquippedWeapon->AddAmmo(ReloadAmount);
}

void UECombatComponent::SetAimMouseSensitivity()
{
	switch (EquippedWeapon->GetWeaponType())
	{
		case EWeaponType::WT_Rifle:    MouseSensitivity = 0.5f; break;
		case EWeaponType::WT_Shotgun:  MouseSensitivity = 0.7f; break;
		case EWeaponType::WT_Sniper:   MouseSensitivity = 0.1f; break;
	}
}




