// Copyright EPortfolio


#include "Weapon/EProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"

AEProjectile::AEProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjecttileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjecttileCollision"));
	SetRootComponent(ProjecttileCollision);
	ProjecttileCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ProjecttileCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjecttileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjecttileCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	ProjecttileCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);


	ProjecttileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjecttileMesh"));
	ProjecttileMesh->SetupAttachment(ProjecttileCollision);
	ProjecttileMesh->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
	ProjecttileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjecttileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;


	ProjectileMovementComponent->InitialSpeed = 2e+4f;
	ProjectileMovementComponent->MaxSpeed = 2e+4f;

}

void AEProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		ProjecttileCollision->OnComponentHit.AddDynamic(this, &AEProjectile::OnHit);
	}

	SetLifeSpan(3);
}

void AEProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AEProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEProjectile::Destroyed()
{
	Super::Destroyed();

	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

