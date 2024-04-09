// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Astronaut.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AAstronaut::AAstronaut()
{

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Sets initial firing mode
	SetGunState(FIRE);

	//Set Size for CapsuleComponent
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	//Creating CameraComponent
	AstronautCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AstronautCamera"));
	AstronautCamera->SetupAttachment(GetCapsuleComponent());
	AstronautCamera->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position the camera
	AstronautCamera->bUsePawnControlRotation = true;

	//Hide Astronaut Mesh Component
	//GetMesh()->SetOwnerNoSee(true);

	//Create Gun Mesh
	//AstronautWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AstronautWeapon"));
	//AstronautWeapon->SetupAttachment(GetMesh());
	//AstronautWeapon->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	//AstronautWeapon->SetOwnerNoSee(false);

}

// Called when the game starts or when spawned
void AAstronaut::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAstronaut::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAstronaut::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


AAstronaut::gunState AAstronaut::getGunState()
{
	return eGunState;
}


void AAstronaut::SetGunState(gunState fireMode)
{
	eGunState = fireMode;
}


