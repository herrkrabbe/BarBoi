// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Ship.h"

RepairPhase AShip::Repair()
{
	ProgressRemaining = 0;

	return Repair(0);;
}

RepairPhase AShip::Repair(float deltaTime)
{
	switch (RepairState) {
	case REPAIRED:
		WinGame();
		break;
	case REPAIR_COMPLETED:
		if(RepairsDone >= RepairsNeeded) {
			RepairState = REPAIRED;
			break;
		}
		RepairState = REPAIRING;
		//intentional fallthrough
		[[fallthrough]];
	default:
		ProgressRemaining -= deltaTime;
		if (ProgressRemaining <= 0) {
			RepairState = REPAIR_COMPLETED;
			RepairsDone += 1;
		}
		else {
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
