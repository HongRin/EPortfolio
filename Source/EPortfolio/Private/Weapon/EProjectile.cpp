// Copyright EPortfolio


#include "Weapon/EProjectile.h"
#include "EPortfolio/EPortfolio.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Character/Player/EPlayer.h"

#include "Kismet/GameplayStatics.h"

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
	ProjecttileCollision->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);

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
	if (AEPlayer* Player = Cast<AEPlayer>(OtherActor))
	{

		FVector Location = Player->GetActorLocation();
		FVector HitDirection = (Hit.ImpactPoint - Location).GetSafeNormal();
		FVector Forward = Player->GetActorForwardVector();

		FVector2D PlayDirection = CalculateHitDirection(Forward, HitDirection);

		Player->MulticastHit(PlayDirection);

		if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			if (AController* OwnerController = Character->Controller)
			{
				UGameplayStatics::ApplyDamage(Player, 20.f, OwnerController, this, UDamageType::StaticClass());
			}
		}
	}

	Destroy();
}

FVector2D AEProjectile::CalculateHitDirection(const FVector& Forward, const FVector& HitDirection)
{
	FVector2D Direction(0.f, 0.f);
	float Dot = FVector::DotProduct(Forward, HitDirection);
	FVector Cross = FVector::CrossProduct(Forward, HitDirection);

	if (Dot > 0.5f)
		Direction.Y = 1;
	else if (Dot < -0.5f)
		Direction.Y = -1;

	if (FMath::Abs(Dot) < 0.707f)
	{
		if (Cross.Z > 0.f)
			Direction.X = 1;
		else if (Cross.Z < 0.f)
			Direction.X = -1;
	}

	return Direction;
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

