// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pickupable.generated.h"

enum PickupType {
	NONE = 0,
	SCRAP = 1,
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

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* Function handles operations before picking up item.
	returns the PickupType of the class*/
	virtual PickupType Pickup() = 0;

	
};
