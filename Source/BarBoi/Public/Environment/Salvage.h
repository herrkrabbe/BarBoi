// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Pickupable.h"
#include "Salvage.generated.h"

class UBoxComponent;


/* ASalvage is a class representing scrap the player can pick up.
the mesh and collision is implemented in Blueprints*/
UCLASS()
class BARBOI_API ASalvage : public AActor, public IPickupable
{
	GENERATED_BODY()

	
	
public:	
	// Sets default values for this actor's properties
	ASalvage();

	UPROPERTY(VisibleAnywhere, Category = "Overlap")
	UBoxComponent* OverlapBox;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	//UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/* Function handles operations before picking up item.
	returns the PickupType of the class*/
	virtual PickupType Pickup() override;

};
