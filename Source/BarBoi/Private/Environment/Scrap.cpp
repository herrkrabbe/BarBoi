// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Scrap.h"

// Sets default values
AScrap::AScrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AScrap::BeginPlay()
{
	Super::BeginPlay();
	
}

PickupType AScrap::Pickup()
{
	Destroy();

	return type;
}

