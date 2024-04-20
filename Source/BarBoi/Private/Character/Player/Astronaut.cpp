// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Astronaut.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Character/Player/Weapon/Weapon.h>



// Sets default values
AAstronaut::AAstronaut()
{

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set Size for CapsuleComponent
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	//Creating CameraComponent
	AstronautCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AstronautCamera"));
	AstronautCamera->SetupAttachment(GetCapsuleComponent());
	AstronautCamera->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position the camera
	AstronautCamera->bUsePawnControlRotation = true;

	//Hide Astronaut Mesh Component
	GetMesh()->SetOwnerNoSee(true);

	//Create Gun Mesh
	AstronautWeapon = CreateDefaultSubobject<UWeapon>(TEXT("AstronautWeapon"));
	
	AstronautWeapon->SetupAttachment(GetMesh());
	AstronautWeapon->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	AstronautWeapon->SetOwnerNoSee(false);

}

// Called when the game starts or when spawned
void AAstronaut::BeginPlay()
{
	Super::BeginPlay();

	//Constrain HP
	if (GetOxygen() > GetOxygenMax()) //more than max
	{
		HP = GetOxygenMax();
	}
	else if (GetOxygen() < 0) //less than 0
	{
		HP = 1;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Oxygen is less than 0"));
	}

	//Add Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(AstronautMapppingContext, 0);
		}
	}
	
}

//This function is sourced from the FPS tempalte
void AAstronaut::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Called every frame
void AAstronaut::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Damage(DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Oxygen: %f"), GetOxygen()));

	//Tick Weapon
	AstronautWeapon->TickWeapon(DeltaTime);

}

// Called to bind functionality to input
void AAstronaut::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Looking
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &AAstronaut::Look);

		AstronautWeapon->SetupInput(this);
	}
	else
	{
		//insert error here
	}
}

float AAstronaut::GetOxygen()
{
	return HP;
}

float AAstronaut::GetOxygenMax()
{
	return HPMax;
}

bool AAstronaut::Damage(float damageDone)
{
	float newHP = GetOxygen() - damageDone;

	if (newHP <= 0) { //test if dead
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PlayerCharacter is dead"));
		RequestEngineExit("PlayerCharacter is dead");
		return false;
	}

	HP = newHP;

	return true;
}



