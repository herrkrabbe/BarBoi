// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Scrap.h"
#include <Components/BoxComponent.h>

// Sets default values
AScrap::AScrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	//creating mesh and collision box
	//UBoxComponent* CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	

	//setting root component
	SetRootComponent(CollisionBox);
	//Mesh->SetupAttachment(CollisionBox);

	if (CollisionBox != nullptr) {
		//enable overlap event for picking up
		CollisionBox->SetGenerateOverlapEvents(true);

	

		//Setting collision rules
		//Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}
	

}

// Called when the game starts or when spawned
void AScrap::BeginPlay()
{
	Super::BeginPlay();
}

PickupType AScrap::Pickup()
{
	if (!Destroy()) {
		//giving error message if the scrap object failed to delete
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAILED DELETE"));
	}
	return type;
}

