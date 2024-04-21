// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"

#include "NavigationSystem.h"




void AEnemyAIController::BeginPlay()
{
	//Have to use super otwerwise Characther wont move
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	Patrol();
}

void AEnemyAIController::Patrol()
{
	if (NavArea)
	{
		// k2 takes the world as the first parameter.
		// Second searches for a reaceable Location
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 15000.0f);

		//Makes AI MoveToLocation
		MoveToLocation(RandomLocation);
	}
}


