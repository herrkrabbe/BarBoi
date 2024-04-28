#pragma once

#include "CoreMinimal.h"
#include "Containers/UnrealString.h"

class FString;

/* Enumerator to tell classes using Repairable interface the state of the object they are repairing
Unrepaired - repair has not started
Repairing - repair has started but is not completed
Repair_completed - repair finished this call of repair
Repaired - repair was already complete*/
enum RepairPhase {
	UNREPAIRED = 0,
	REPAIRING = 1,
	REPAIR_COMPLETED = 2,
	REPAIRED = 3
};

/* Function to turn RepairPhase enumerator to FString value
returns name of the Enum value

UNREPAIRED
REPAIRING
REPAIR_COMPLETED
REPAIRED

or ERROR*/
FString RepairPhaseToString(RepairPhase phase);