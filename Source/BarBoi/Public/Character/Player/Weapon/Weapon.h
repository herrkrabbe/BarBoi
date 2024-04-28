// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Weapon.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeapon : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for weapons.
 * A weapon has a main action Main() and a secondary action Secondary().
 * The weapon also has a counter for how many times the secondary action can be used successfully.
 */
class BARBOI_API IWeapon
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* Main action of the weapon.
	returns true if action was successful and false otehrwise*/
	virtual bool Main() = 0;

	/* Secondary action of the weapon.
	returns true if action was complete and successful
	and returns false if action was not completed*/
	virtual bool Secondary();

	/* Get secondary actions remaining.
	It is the weapons responsibility to check if it can perform any secondary actions*/
	virtual int GetSecondaryRemaining();

	/* Add 1 secondary action use to the weapon
	returns false only if the weapon has reached its maximum secondary actions remaining*/
	virtual bool AddSecondaryRemaining();

	/* Returns the maximum amount of ammo the weapon can hold */
	virtual float GetAmmoMax() = 0;

	/* Returns the amount of ammo remaining */
	virtual float GetAmmo() = 0;

	/* Returns if the weapon can fire */
	virtual bool CanFire() = 0;

protected:
	/* Number successful of secondary actions remaining*/
	int SecondaryRemaining = 0;
};
