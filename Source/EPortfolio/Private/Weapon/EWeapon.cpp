// Copyright EPortfolio


#include "Weapon/EWeapon.h"
#include "Character/Player/EPlayer.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

AEWeapon::AEWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	{
		WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
		
		SetRootComponent(WeaponMesh);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
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
}

void AEWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	}

	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

}

void AEWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEPlayer* Player = Cast<AEPlayer>(OtherActor);

	if (Player && PickupWidget)
	{
		PickupWidget->SetVisibility(true);
	}
}

