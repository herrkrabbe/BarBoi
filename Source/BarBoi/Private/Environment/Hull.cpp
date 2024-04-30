// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Hull.h"

// Sets default values
AHull::AHull()
{
	// create components
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	RootComponent = OverlapBox;

	

}

// Called when the game starts or when spawned
void AHull::BeginPlay()
{
	Super::BeginPlay();

	// disable gravity
	OverlapBox->SetEnableGravity(false);
	RepairState = UNREPAIRED;
}

RepairPhase AHull::Repair()
{
	progressRemaining = 0;
	return Repair(0);
}

RepairPhase AHull::Repair(float deltaTime)
{
	switch (RepairState) {
	case REPAIRED: //already repaired
		break;
	case REPAIR_COMPLETED: //Update repair completed
		RepairState = REPAIRED;
		break;
	default: //handling repairing
		progressRemaining -= deltaTime;
		if (progressRemaining <= 0) {
			RepairState = REPAIR_COMPLETED;
			progressRemaining = 0;
		}
		else {
			RepairState = REPAIRING;
		}
	}

	return RepairState;
}

