// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RepairPhase.h"
#include "Repairable.generated.h"

/* Enumerator to tell classes using Repairable interface the state of the object they are repairing
Unrepaired - repair has not started
Repairing - repair has started but is not completed
Repair_completed - repair finished this call of repair
Repaired - repair was already complete*/
//enum RepairPhase {
//	UNREPAIRED = 0,
//	REPAIRING = 1,
//	REPAIR_COMPLETED = 2,
//	REPAIRED = 3
//};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URepairable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface to ahndle repair of objects.
 */
class BARBOI_API IRepairable
{
	GENERATED_BODY()
protected:
	RepairPhase RepairState = UNREPAIRED;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* Function to instantly repair object*/
	virtual RepairPhase Repair() = 0;

	/* Function that adds deltaTime to repair progress*/
	virtual RepairPhase Repair(float deltaTime) = 0;
};
