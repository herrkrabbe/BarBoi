// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UEnemy.generated.h"

UCLASS()
class BARBOI_API AUEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUEnemy();

	//Setting up classes i need to create the Enemy logic
	
	bool PLayerDetected;
	bool CanAttackPlayer;

	//Added bool Variable. To Access in the BP_Enemy Aniamtion to set it true or false on the state of the attack animation;
	UPROPERTY(BlueprintReadWrite)
		bool CanDealDamage;
	
	class AEnemy_AICharacther* PlayerREF;

	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerCollisionDetection;

	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerAttackCollisionDetection;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* DamageCollision;

	
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
