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

	Health = HealthMax;
	
	EnemyAIController = Cast<AEnemyAIController>(GetController());
	if (EnemyAIController != nullptr) {
		EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject
		(this, &AUEnemy::OnAIMoveCompleted);
	}

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

AAstronaut* AUEnemy::GetTarget_Implementation()
{
	return target;
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

		// Checks if player is attacked(Supposed to add for animation when we get it)
		UE_LOG(LogTemp, Warning, TEXT("Player got ATTACKED"));
	}
}

// moves Enemy to player
void AUEnemy::MoveToPlayer()
{
	EnemyAIController->MoveToLocation(GetTarget_Implementation()->GetActorLocation(), StoppingDistance, true);
}
// seeks Astronaut
void AUEnemy::SeekPlayer()
{
	MoveToPlayer();
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this,
	                                       &AUEnemy::SeekPlayer, 0.25f, true);
}

//stops seeking Astronaut
void AUEnemy::StopSeekingPlayer()
{
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);
}

//Detects if astronaut is in sphere
void AUEnemy::OnPlayerDetectedOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetTarget_Implementation() == OtherActor)
	{
		//tests and if true it start going to Astronaut
		PlayerDetected = true;
		SeekPlayer();
	}
}
//Detects if Astronaut moves out of sphere
void AUEnemy::OnPlayerDetectedOverLapEnd(UPrimitiveComponent* OverLappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetTarget_Implementation()== OtherActor)
	{
		//if this is true it stop seeking and starts patroling
		PlayerDetected = false;
		StopSeekingPlayer();
		EnemyAIController->Patrol();
	}
}

//Detects when player is in sphere to start the attack
void AUEnemy::OnPlayerAttackOverlapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetTarget_Implementation())
	{
		CanAttackPlayer = true;
	}
}

//Detects if players has moved out of sphere
void AUEnemy::OnPlayerAttackOverLapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetTarget_Implementation())
	{
		//if player is not inside stops attack and start following Astronaut
		CanAttackPlayer = false;

		SeekPlayer();
	}
}
//Starts attacking with damage collision box
void AUEnemy::OnDealDamageOverLapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetTarget_Implementation() && CanDealDamage)
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


void AUEnemy::DamageThis_Implementation(float DamageTaken)
{
	Health -= DamageTaken;
	
	if (Health <= 0)
	{
		GetWorld()->DestroyActor(this);
	}
}

//Damages target to remove oxygen
float AUEnemy::DamageTarget_Implementation(float DamageDealt)
{
	GetTarget_Implementation()->DamageThis(DamageDealt);
	return GetTarget_Implementation()->GetOxygen();
}






