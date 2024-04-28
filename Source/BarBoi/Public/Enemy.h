// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include <Character/Player/Astronaut.h>
#include "Enemy.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BARBOI_API IEnemy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	AAstronaut* target;

	float HP;

	float HPMax;

	float Damage;

public:

	float GetHP() { return HP; }

	float GetHPMax() { return HPMax; }

	/* Deal damage to this instance. Function is responsible for destroying actor if its hp reaches 0 or less.
	returns new HP.
	returns HP remaining*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void DamageThis(float DamageTaken);
	
	/* Set the target for the enemy AI
	returns true if target is set
	returns false otherwise*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI")
	bool SetTarget(AAstronaut* newTarget);


protected:
	
	void SetHP(float value) { HP = value; }

	void SetHPMax(float value) { HPMax = value; }


	void SetDamage(float value) { Damage = value; }


	AAstronaut* GetTarget() { return target; }

	/* Rudementary function for dealing damage to target. Does not check for conditions
	 * returns HP remaining of target
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	float DamageTarget(float DamageDealt);

	
};
