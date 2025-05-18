// Copyright EPortfolio


#include "PickupActor/EPickupSpawnPoint.h"
#include "PickupActor/EPickupActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AEPickupSpawnPoint::AEPickupSpawnPoint()
{
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEPickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		SpawnPadEffect,
		GetActorLocation() + FVector(0, 0, 20),
		GetActorRotation()
	);

	StartSpawnTimer((AActor*)nullptr);

	if (!HasAuthority())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			SpawnLoadingEffect,
			GetActorLocation() + FVector(0, 0, 20),
			GetActorRotation()
		);
	}
}

void AEPickupSpawnPoint::SpawnPickupActor()
{
	int32 NumPickupClasses = PickupActorClasses.Num();

	if (NumPickupClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);

		FVector Location = GetActorLocation() + FVector(0.f, 0.f, 100.f);

		FTransform Transform = GetActorTransform();
		Transform.SetLocation(Location);

		SpawnedPickupActor = GetWorld()->SpawnActor<AEPickupActor>(PickupActorClasses[Selection], Transform);

		if (HasAuthority() && SpawnedPickupActor)
		{
			SpawnedPickupActor->OnDestroyed.AddDynamic(this, &AEPickupSpawnPoint::StartSpawnTimer);
		}
	}
}

void AEPickupSpawnPoint::SpawnTimerFinished()
{
	if (HasAuthority())
	{
		SpawnPickupActor();
	}
}

void AEPickupSpawnPoint::StartSpawnTimer(AActor* DestroyedActor)
{
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			SpawnTimer,
			this,
			&AEPickupSpawnPoint::SpawnTimerFinished,
			SpawnDelay
		);

		MulticastPlayEffect();
	}
}

void AEPickupSpawnPoint::MulticastPlayEffect_Implementation()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		SpawnLoadingEffect,
		GetActorLocation() + FVector(0, 0, 20),
		GetActorRotation()
	);
}

