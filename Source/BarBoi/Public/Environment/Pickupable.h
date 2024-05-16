// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pickupable.generated.h"

enum EPickupType {
	NONE = 0,
	SALVAGE = 1,
	OXYGEN = 2,
	AMMO = 3
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BARBOI_API IPickupable
{
	GENERATED_BODY()

protected:
	/* Type of pickup. Is set by children */
	EPickupType type = NONE;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* Function handles operations before picking up item.
	returns the EPickupType of the class*/
	virtual EPickupType Pickup() = 0;

	
};
