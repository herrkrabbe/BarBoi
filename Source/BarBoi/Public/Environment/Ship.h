// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Repairable.h"
#include "Ship.generated.h"

/**
 * 
 */
UCLASS()
class BARBOI_API AShip : public AActor, public IRepairable
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Repair")
	float ProgressRemaining;

	UPROPERTY(Editanywhere, BlueprintReadOnly, Category = "Repair")
	float ProgressMax = 5;

	UPROPERTY(BlueprintReadWrite, Category = "Repair")
	int RepairsDone = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Repair")
	int RepairsNeeded = 10;

public:
	// Inherited via IRepairable
	ERepairPhase Repair() override;

	ERepairPhase Repair(float deltaTime) override;

	AShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category="Win")
	void WinGame();

};
