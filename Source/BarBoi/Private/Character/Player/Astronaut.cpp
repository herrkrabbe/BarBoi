// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Astronaut.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Character/Player/Weapon/Weapon.h>
#include <Character/Player/Droid.h>
#include <Kismet/KismetSystemLibrary.h>



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


	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	GetCharacterMovement()->GravityScale = 0.f;
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
			Subsystem->AddMappingContext(AstronautMappingContext, 0);
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

FVector AAstronaut::VectorToDroid()
{
	if (GetDroid_Implementation().GetObject() == nullptr) return FVector::Zero(); // Is droid set?

	ADroid* droid = Cast<ADroid>(GetDroid_Implementation().GetObject()); //Get droid reference

	if (droid == nullptr) return FVector::Zero(); //check cast

	FVector ToDroid = droid->GetActorLocation() - GetActorLocation();

	return ToDroid;
}

float AAstronaut::LengthToDroid()
{
	return VectorToDroid().Length();
}

void AAstronaut::MoveToDroid()
{
	ADroid* droid = Cast<ADroid>(GetDroid_Implementation().GetObject()); //Get droid reference

	if(droid == nullptr) return; //check cast

	double speed = droid->GetVelocity().Length();

	FVector force = VectorToDroid() * 500;

	GetCharacterMovement()->AddForce(force);
}

// Called every frame
void AAstronaut::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DamageThis(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Oxygen: %f"), GetOxygen()));

	//Tick Weapon
	AstronautWeapon->TickWeapon(DeltaTime);

	MoveToDroid();

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

bool AAstronaut::DamageThis(float damageDone)
{
	float newHP = GetOxygen() - damageDone;

	if (newHP <= 0) { //test if dead
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PlayerCharacter is dead"));

		APlayerController* PlayerController;

		if (Controller == nullptr) {
			//Because this is not controlled, it means the droid is controlled
			//therefore we get the droid's controller for the function
			
			PlayerController = Cast<APlayerController>(Cast<ADroid>(GetDroid_Implementation().GetObject())->Controller);
		}
		else {
			PlayerController = Cast<APlayerController>(Controller);
		}

		//attemt quit game
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);


		return false;
	}

	HP = newHP;

	return true;
}

float AAstronaut::GetAmmo()
{
	return AstronautWeapon->GetHeat();
}

float AAstronaut::GetAmmoMax()
{
	return AstronautWeapon->GetHeatMax();
}

TScriptInterface<ISwitch> AAstronaut::GetDroid_Implementation()
{

	
	ADroid* droid = Cast<ADroid>(Other.GetObject()); //Get object reference

	if (droid == nullptr) { //check object reference
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Astronaut cannot find droid")));
	}

	return TScriptInterface<ISwitch>(droid);
}

TScriptInterface<ISwitch> AAstronaut::GetAstronaut_Implementation()
{
	return TScriptInterface<ISwitch>(this); // convert this to variable holding 
}

TScriptInterface<ISwitch> AAstronaut::GetOther_Implementation()
{
	return Other;
}

bool AAstronaut::SetAstronaut_Implementation(const TScriptInterface<ISwitch>& astronaut)
{
	return false;
}

bool AAstronaut::SetDroid_Implementation(const TScriptInterface<ISwitch>& droid)
{

	if (Other.GetObject() != nullptr) return false;

	if (ADroid* thisOne = Cast<ADroid>(this)) return false;

	if (droid.GetObject()->Implements<USwitch>())
	{
		Other = TScriptInterface<ISwitch>(droid.GetObject());
		return true;
	}
	return false;
}