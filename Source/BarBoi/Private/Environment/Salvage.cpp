// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Salvage.h"
#include <Components/BoxComponent.h>

// Sets default values
ASalvage::ASalvage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	type = EPickupType::SALVAGE;

	//setting root component
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	RootComponent = OverlapBox;

	if (OverlapBox != nullptr) {
		//enable overlap event for picking up
		OverlapBox->SetGenerateOverlapEvents(true);

	

		//Setting collision rules
		OverlapBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}
	

}

// Called when the game starts or when spawned
void ASalvage::BeginPlay()
{
	Super::BeginPlay();
}

EPickupType ASalvage::Pickup()
{
	if (!Destroy()) {
		
	}
	return type;
}

