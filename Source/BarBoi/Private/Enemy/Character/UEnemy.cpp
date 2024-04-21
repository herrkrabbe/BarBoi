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
	EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject
	(this, &AUEnemy::OnAIMoveCompleted);

	PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
		&AUEnemy::OnPlayerDetectedOverLapBegin);

	PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
		&AUEnemy::OnPlayerDetectedOverLapEnd);

	PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
		&AUEnemy::OnPlayerAttackOverlapBegin);

	PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
		&AUEnemy::OnPlayerAttackOverLapEnd);

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this,
		&AUEnemy::OnDealDamageOverLapBegin);

	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if(PlayerController == nullptr) return;
	
	ACharacter* chara = PlayerController->GetCharacter();

	if(chara == nullptr) return;

	ISwitch* controlledCharacter = Cast<ISwitch>(chara);

	if(controlledCharacter == nullptr) return;

	UObject* astro = controlledCharacter->GetAstronaut_Implementation().GetObject();

	AAstronaut* newTarget = Cast<AAstronaut>(astro);

	if(newTarget == nullptr) return;
	
	SetTarget_Implementation(newTarget);
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

void AUEnemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	//Checks if player has not been detected
	if (!PlayerDetected)
	{
		//If player not detected continue patrolling
		EnemyAIController->Patrol();
	}
	else if (PlayerDetected && CanAttackPlayer)
	{
		//If conditions are met for the attack it will stop seeking
		StopSeekingPlayer();

		// Checks if player is attacked (Not Implemented yet)
		UE_LOG(LogTemp, Warning, TEXT("Player got ATTACKED"));
	}
}

void AUEnemy::MoveToPlayer()
{
	EnemyAIController->MoveToLocation(GetTarget()->GetActorLocation(), StoppingDistance, true);
}

void AUEnemy::SeekPlayer()
{
	MoveToPlayer();
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this,
	                                       &AUEnemy::SeekPlayer, 0.25f, true);
}

void AUEnemy::StopSeekingPlayer()
{
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);
}

void AUEnemy::OnPlayerDetectedOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetTarget())
	{
		PlayerDetected = true;
		SeekPlayer();
	}
}

void AUEnemy::OnPlayerDetectedOverLapEnd(UPrimitiveComponent* OverLappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetTarget())
	{
		PlayerDetected = false;
		StopSeekingPlayer();
		EnemyAIController->Patrol();
	}
}

void AUEnemy::OnPlayerAttackOverlapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetTarget())
	{
		CanAttackPlayer = true;
	}
}


void AUEnemy::OnPlayerAttackOverLapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetTarget())
	{
		CanAttackPlayer = false;

		SeekPlayer();
	}
}

void AUEnemy::OnDealDamageOverLapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetTarget() && CanDealDamage)
	{
		//Test to see Enemy deal damage to player
		UE_LOG(LogTemp, Warning, TEXT("Player took Damage"));
	}
}

bool AUEnemy::SetTarget_Implementation(AAstronaut* newTarget)
{
	if(target != nullptr) return false;

	target = newTarget;

	return true;
}

float AUEnemy::DamageThis_Implementation(float DamageTaken)
{
	return IEnemy::DamageThis_Implementation(DamageTaken);
}

float AUEnemy::DamageTarget_Implementation(float DamageDealt)
{
	GetTarget()->DamageThis(DamageDealt);
	return GetTarget()->GetOxygen();
}






