// Copyright EPortfolio


#include "Weapon/EWeapon.h"
#include "Character/Player/EPlayer.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/ECasing.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "LegacyCameraShake.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceConstant.h"

AEWeapon::AEWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	{
		WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
		
		SetRootComponent(WeaponMesh);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	{
		AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
		AreaSphere->SetupAttachment(WeaponMesh);
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	{
		PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
		PickupWidget->SetupAttachment(WeaponMesh);
	}

	{
		Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	}
}

void AEWeapon::BeginPlay()
{
	Super::BeginPlay();
	


	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);

	}

	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
	FOnTimelineFloat timeline;
	timeline.BindUFunction(this, "OnAiming");
	Timeline->AddInterpFloat(WeaponDatas.AimCurve, timeline);
	Timeline->SetLooping(false);
	Timeline->SetPlayRate(WeaponDatas.AimingSpeed);
}

void AEWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEWeapon, WeaponState);
}

void AEWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

void AEWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	switch (WeaponState)
	{
	case EWeaponState::WS_Equipped:
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AEWeapon::Fire(const FVector& HitTarget)
{
	if (IsValid(WeaponDatas.AnimDatas.WeaponFireAnimantion))
	{
		WeaponMesh->PlayAnimation(WeaponDatas.AnimDatas.WeaponFireAnimantion, false);
	}

	if (WeaponDatas.ClassDatas.CasingClass)
	{
		const USkeletalMeshSocket* EjectSocket = WeaponMesh->GetSocketByName(FName("Eject"));
		if (EjectSocket)
		{
			FTransform SocketTransform = EjectSocket->GetSocketTransform(WeaponMesh);

			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<AECasing>(
					WeaponDatas.ClassDatas.CasingClass,
					SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator()
				);
			}
		}
	}

	if (WeaponDatas.CameraShakeClass)
	{
		APlayerController* Controller = Cast<AEPlayer>(GetOwner())->GetController<APlayerController>();
	
		if (Controller)
			Controller->PlayerCameraManager->StartCameraShake(WeaponDatas.CameraShakeClass);
	}


	Cast<AEPlayer>(GetOwner())->AddControllerPitchInput(-WeaponDatas.RecoilRate * UKismetMathLibrary::RandomFloatInRange(0.8f, 1.2f));
}

void AEWeapon::Aimimg(bool bAiming)
{
	if (bAiming)
	{
		Timeline->PlayFromStart();
		SetAimData(WeaponDatas.AimDatas);
	}
	else
	{
		Timeline->ReverseFromEnd();
		SetAimData(WeaponDatas.BaseDatas);
	}
}

void AEWeapon::DrawDecal(const FVector InLocation, const FRotator InRotator)
{
	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(5), InLocation, InRotator, 10);
	Decal->SetFadeScreenSize(0);
}

void AEWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEPlayer* Player = Cast<AEPlayer>(OtherActor);
	
	if (Player)
	{
		Player->SetOverlappingWeapon(this);
	}
}

void AEWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEPlayer* Player = Cast<AEPlayer>(OtherActor);
	if (Player)
	{
		Player->SetOverlappingWeapon(nullptr);
	}
}

void AEWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::WS_Equipped:
		ShowPickupWidget(false);
		break;
	}
}

void AEWeapon::OnAiming(float Output)
{
	float FOV = FMath::Lerp(WeaponDatas.AimDatas.FieldOfView, WeaponDatas.BaseDatas.FieldOfView, Output);
	Cast<AEPlayer>(GetOwner())->GetFollowCamera()->SetFieldOfView(FOV);
}

void AEWeapon::SetAimData(const FEWeaponAimData& InWeaponAimData)
{
	USpringArmComponent* SpringArm = Cast<AEPlayer>(GetOwner())->GetCameraBoom();
	SpringArm->TargetArmLength = InWeaponAimData.TargetArmLength;
	SpringArm->SocketOffset = InWeaponAimData.SocketOffset;
	SpringArm->bEnableCameraLag = InWeaponAimData.bEnableCameraLag;
}



