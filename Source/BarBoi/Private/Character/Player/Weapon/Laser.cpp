// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Weapon/Laser.h"
#include <Enemy.h>

// Sets default values
ALaser::ALaser()
{
	//enabling ticking for lifespan
	Super::SetLifeSpan(5.f);

 	// initialise Box component used for collision detection
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(20.f, 5.f, 5.f));

	//setting Box as root component
	RootComponent = CollisionComp;

	//Adding hit event
	CollisionComp->SetCollisionProfileName("Laser");
	//CollisionComp->OnComponentHit.AddDynamic(this, &ALaser::OnHit);
	


	//Creating ProjectileMovement Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = speedInitial;
	ProjectileMovement->MaxSpeed = speedMax;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // disabling gravity

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ALaser::OnLaserBeginOverlap);
}

void ALaser::BeginPlay()
{
	Super::BeginPlay();


	//Enable overlap event
	
}

void ALaser::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//checking if the other actor is valid
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr) || (!OtherActor->Implements<UEnemy>())) {
		return; // return is used here for prettier code
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hit"));

	IEnemy* enemy = Cast<IEnemy>(OtherActor);
	//applying damage to enemy
	if (OtherActor->Implements<UEnemy>()) {

		IEnemy::Execute_DamageTarget(OtherActor, Damage);
		OtherActor->Destroy();
		Destroy();
	}
	else if (enemy != nullptr) { //fallback method if previous if statement does not work for C++
		enemy->DamageTarget_Implementation(Damage);
		Destroy();
	}
	
}

void ALaser::OnLaserBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnHit(OverlappedComp, OtherActor, OtherComp, FVector(0.f, 0.f, 0.f), SweepResult);
}


