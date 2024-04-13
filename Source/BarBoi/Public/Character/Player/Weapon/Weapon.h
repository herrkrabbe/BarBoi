// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class BARBOI_API UWeapon : public UMeshComponent
{
	GENERATED_BODY()
	
public:
	/* Constructor for weapon*/
	UWeapon();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALaser> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* WeaponMappingContext;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ActionFire;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ActionRepair;

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	/* Get Heat value of weapon */
	UFUNCTION(BlueprintCallable, Category = "Heat")
	float GetHeat();

	

	/* Function to cool gun. Must be called every tick in parent actor
	
	returns heat removed*/
	UFUNCTION(BlueprintCallable, Category = "Heat")
	float Cooling(float deltaTime);


private:
	/* Float tracking the heat of the weapon. heat starts at 0 */
	UPROPERTY(EditAnywhere, Category="Heat", meta = (AllowPrivateAccess = "true"))
	float Heat = 0;

	/* Float tracking the maximum heat of the weapon. starts at 300 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	float HeatCapacity = 300;

	/* Heat removed per second */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	float CoolingRate = 150;

	/* Heat added per shot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	float HeatPerShot = 62.5;

	/* Bool tracking if the weapon is overheated. starts false */
	UPROPERTY(BlueprintReadWrite, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	bool bIsOverheated = false;

	/* Set Heat value of weapon */
	UFUNCTION(BlueprintCallable, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	void SetHeat(float newHeat);

	/* Add Heat value to weapon */
	UFUNCTION(BlueprintCallable, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	void AddHeat(float deltaHeat);

};
