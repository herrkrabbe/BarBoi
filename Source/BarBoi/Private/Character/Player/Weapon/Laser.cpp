// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Weapon/Laser.h"

// Sets default values
ALaser::ALaser()
{
	
	//enabling ticking for lifespan
	Super::SetLifeSpan(5.f);

 	// initialise Box component used for collision detection
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(20.f, 5.f, 5.f));

	//Rotating Box to be horizontal
	//CollisionComp->AddLocalRotation(FRotator(90.f, 0.f, 0.f));

	//Adding hit event
	CollisionComp->SetCollisionProfileName("Laser");
	CollisionComp->OnComponentHit.AddDynamic(this, &ALaser::OnHit);

	//setting Box as root component
	RootComponent = CollisionComp;


	//Creating ProjectileMovement Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = speedInitial;
	ProjectileMovement->MaxSpeed = speedMax;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // disabling gravity
	
	//LaserMesh->SetupAttachment(CollisionComp);
	
	


	//disable collision of mesh
	//LaserMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}



void ALaser::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) {
		return; // return is used here for prettier code
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("TODO: Laser OnHit implementation%f")));

	Destroy();
}

