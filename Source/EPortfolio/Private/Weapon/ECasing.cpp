// Copyright EPortfolio


#include "Weapon/ECasing.h"

AECasing::AECasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);

	CasingMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CasingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CasingMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 10.f;
}

void AECasing::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.0f);
	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
}



