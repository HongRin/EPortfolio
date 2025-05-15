// Copyright EPortfolio


#include "PickupActor/EPickupActor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Character/Player/EPlayer.h"
#include "Component/EBuffComponent.h"

AEPickupActor::AEPickupActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(RootComponent);
	OverlapSphere->SetSphereRadius(40.f);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(OverlapSphere);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	Mesh->SetRenderCustomDepth(true);
	Mesh->SetCustomDepthStencilValue(252);
}

void AEPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Mesh)
	{
		Mesh->AddWorldRotation(FRotator(0.f, BaseTurnRate * DeltaTime, 0.f));
	}
}

void AEPickupActor::Destroyed()
{
	Super::Destroyed();
	
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}

	if (Player && PickupEffect)
	{
		if (Player->GetBuffComponent())
		{
			Player->GetBuffComponent()->PlayPickupEffect(PickupEffect);
		}
	}
}

void AEPickupActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			OverlapTimer,
			this,
			&AEPickupActor::OverlapTimerFinished,
			OverlapTime
		);
	}
}

void AEPickupActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<AEPlayer>(OtherActor);
}

void AEPickupActor::OverlapTimerFinished()
{
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AEPickupActor::OnSphereOverlap);
}


