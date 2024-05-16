// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Ship.h"

ERepairPhase AShip::Repair()
{
	ProgressRemaining = 0;

	return Repair(0);;
}

ERepairPhase AShip::Repair(float deltaTime)
{
	switch (RepairState) {
	case REPAIRED: //final repair
		WinGame();
		break;
	case REPAIR_COMPLETED: //a repair is complete
		if(RepairsDone >= RepairsNeeded - 1) { //this was the last repair
			RepairState = REPAIRED;
			break;
		}

		RepairState = REPAIRING;

		//intentional fallthrough
		[[fallthrough]];
	default:
		ProgressRemaining -= deltaTime; //progress repair

		if (ProgressRemaining <= 0) {
			RepairState = REPAIR_COMPLETED;
			RepairsDone += 1;
		}
		else { //start repair of ship
			RepairState = REPAIRING;
		}
	}

	return RepairState;
}

AShip::AShip()
{
	ProgressRemaining = ProgressMax;
	RepairState = UNREPAIRED;
}

void AShip::BeginPlay()
{
	Super::BeginPlay();

	ProgressRemaining = ProgressMax;
	RepairState = UNREPAIRED;
}
