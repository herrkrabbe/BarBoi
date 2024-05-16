// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Repairable.h"

#include <Components/BoxComponent.h>
#include "Hull.generated.h"

UCLASS()
class BARBOI_API AHull : public AActor, public IRepairable
{
	GENERATED_BODY()
	
	float progressRemaining = 5.0f;
	
public:	
	// Sets default values for this actor's properties
	AHull();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
	UBoxComponent* OverlapBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/* Function to instantly repair object*/
	virtual ERepairPhase Repair() override;

	/* Function that adds deltaTime to repair progress*/
	virtual ERepairPhase Repair(float deltaTime) override;

};
