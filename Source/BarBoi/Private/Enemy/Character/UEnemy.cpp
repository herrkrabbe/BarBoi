// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Character/UEnemy.h"
#include "Enemy/EnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AUEnemy::AUEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Creating components where its attaching Damage Collision variable to the mesh
	PlayerCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));

	PlayerCollisionDetection->SetupAttachment(RootComponent);

	PlayerAttackCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));
	
	PlayerCollisionDetection->SetupAttachment(RootComponent);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(GetMesh(),TEXT("RightHandSocket"));
}



// Called when the game starts or when spawned
void AUEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIController = Cast<AEnemyAIController>(GetController());
	EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AUEnemy::OnAIMoveCompleted);
	
}

void AUEnemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) const -> void
	{
	EnemyAIController->Patrol();
}
// Called every frame
void AUEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

