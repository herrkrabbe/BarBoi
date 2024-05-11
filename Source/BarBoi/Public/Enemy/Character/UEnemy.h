// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "UEnemy.generated.h"

UCLASS()
class BARBOI_API AUEnemy : public ACharacter, public IEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUEnemy();

	//Setting up classes I need to create the Enemy logic
	bool PlayerDetected;
	bool CanAttackPlayer;

	//Added bool Variable. To access "CanDealDamage" in Blueprint
	UPROPERTY(BlueprintReadWrite)
		bool CanDealDamage;
	

	//
	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerCollisionDetection;

	//
	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerAttackCollisionDetection;

	// 
	UPROPERTY(EditAnywhere)
		class UBoxComponent* DamageCollision;
	
	class AEnemyAIController* EnemyAIController;

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

	//With Edit Anywhere makes it possible to edit this variable on the blueprint instance
	UPROPERTY(EditAnywhere)
		float StoppingDistance = 100.f;

	FTimerHandle SeekPlayerTimerHandle;
	
	//Commands AI to move towards player location
	UFUNCTION()
		void MoveToPlayer();

	//Begins cycle of AI looking for Player
	UFUNCTION()
		void SeekPlayer();

	//Stops AI from seeking the player
	UFUNCTION()
		void StopSeekingPlayer();

	//Triggers when the player enters designated collision Area
	UFUNCTION()
		void OnPlayerDetectedOverLapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			 bool bFromSweep, const FHitResult& SweepResult);

	//Triggered when player leaves designated collision Area
	UFUNCTION()
		void OnPlayerDetectedOverLapEnd(class UPrimitiveComponent* OverLappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Triggered when AI detects player entering the attack range
	UFUNCTION()
		void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverLappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Triggered when Player  leaves AI attack collision Area
	UFUNCTION()
		void OnPlayerAttackOverLapEnd(class UPrimitiveComponent*OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	//Triggers when AI`s attack makes contact with player, dealing damage
	UFUNCTION()
		void OnDealDamageOverLapBegin(class UPrimitiveComponent* OverLappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool SetTarget_Implementation(AAstronaut* newTarget) override;

	void DamageThis_Implementation(float DamageTaken) override;

	float DamageTarget_Implementation(float DamageDealt) override;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AAstronaut* GetTarget_Implementation() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
