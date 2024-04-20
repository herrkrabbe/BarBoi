// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "Character/Player/Astronaut.h"
#include "Character/Player/Weapon/Laser.h"
#include "Weapon.generated.h"

class AAstronaut;
class ALaser;
/**
 * 
 */
UCLASS()
class BARBOI_API UWeapon : public UMeshComponent
{
	GENERATED_BODY()
	
public:
	/* Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALaser> ProjectileClass;

	/* Default Constructor for weapon. If you use this constructor you must then SetupInput*/
	UWeapon();

	/* Constructor for weapon*/
	UWeapon(AAstronaut* character);

	/* Function called to tick weapon*/
	void TickWeapon(float deltaTime);

	

	/* Input Mapping Context used for the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* WeaponMappingContext;

	/** Gun muzzle's offset from the characters location 
	From FPS Template*/
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

	/* Make weapon repair ship */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Repair();

	/* Get Heat value of weapon */
	UFUNCTION(BlueprintCallable, Category = "Heat")
	float GetHeat();

	/* Get max Heat value of weapon */
	UFUNCTION(BlueprintCallable, Category = "Heat")
	float GetHeatMax();

	/* Function to cool gun.
	returns heat removed*/
	UFUNCTION(BlueprintCallable, Category = "Heat")
	float Cooling(float deltaTime);

	/* Function called to setup input of weapon*/
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool SetupInput(AAstronaut* character);

	

private:
	/* Float tracking the heat of the weapon. heat starts at 0 */
	UPROPERTY(EditAnywhere, Category="Heat", meta = (AllowPrivateAccess = "true"))
	float Heat = 0;

	/* Float tracking the maximum heat of the weapon. starts at 300 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	float HeatCapacity = 300;

	/* Heat removed per second */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	float CoolingRate = -150;

	/* Heat added per shot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	float HeatPerShot = 62.5;

	/* Time before weapon can shoot again */
	UPROPERTY(BlueprintReadWrite, Category = "Fire", meta = (AllowPrivateAccess = "true"))
	float FireCooldown = 0;

	/* Fire rate of weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire", meta = (AllowPrivateAccess = "true"))
	float FireRate = 0.25;

	/* Function to tick down fire cooldown */
	void FireCooldownTick(float deltaTime);

	/* Bool tracking if the weapon is overheated. starts false */
	UPROPERTY(BlueprintReadWrite, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	bool bIsOverheated = false;

	/* Set Heat value of weapon */
	UFUNCTION(BlueprintCallable, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	void SetHeat(float newHeat);

	/* Add Heat value to weapon */
	UFUNCTION(BlueprintCallable, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	void AddHeat(float deltaHeat);

	/* Function to apply heat floor and roof */
	void ApplyHeatLimits();
	
protected:
	/* Reference to character holding component */
	AAstronaut* Character;

	/** Ends gameplay for this component.
	From FPS Template*/
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


};
