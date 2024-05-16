// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Astronaut.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Character/Player/Weapon/LaserGun.h>
#include <Character/Player/Droid.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Environment/Pickupable.h>



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

	//Create Weapon
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	ULaserGun* weapon = CreateDefaultSubobject<ULaserGun>(TEXT("AstronautWeapon"));
	AstronautWeapon = TScriptInterface<IWeapon>(weapon);
	
	WeaponMesh->SetupAttachment(GetMesh());
	weapon->SetupAttachment(WeaponMesh);
	weapon->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	weapon->SetOwnerNoSee(false);

	//change physics
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	GetCharacterMovement()->GravityScale = 0.f;
	

	//creating itneraction sphere
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(GetCapsuleComponent());
	OverlapSphere->SetSphereRadius(85.f);

	//enable overlap event
	OverlapSphere->SetGenerateOverlapEvents(true);


	//disable camera collisions
	OverlapSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	

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
	}

	

	//Add Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(AstronautMappingContext, 0);
		}
	}

	// Add overlap event
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AAstronaut::PickupItem);
	
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

void AAstronaut::Main()
{
	
	AstronautWeapon->Main();
}

void AAstronaut::Secondary()
{
	AstronautWeapon->Secondary();
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
		EnhancedInputComponent->BindAction(ActionMain, ETriggerEvent::Triggered, this, &AAstronaut::Main);
		EnhancedInputComponent->BindAction(ActionSecondary, ETriggerEvent::Triggered, this, &AAstronaut::Secondary);
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

float AAstronaut::AddOxygen(float amount)
{
	if (amount <= 0) return 0.f;

	float overcapped = HP + amount - HPMax;

	HP += amount;

	if (HP > HPMax) {
		HP = HPMax;
		return overcapped;
	}
	
	return amount;
}

bool AAstronaut::DamageThis(float damageDone)
{
	float newHP = GetOxygen() - damageDone;

	if (newHP <= 0) { //test if dead

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
		OnDeath();


		return false;
	}

	HP = newHP;

	return true;
}

float AAstronaut::GetAmmo()
{
	return AstronautWeapon->GetAmmo();
}

float AAstronaut::GetAmmoMax()
{
	return AstronautWeapon->GetAmmoMax();
}

bool AAstronaut::CanFire()
{
	return AstronautWeapon->CanFire();
}

int AAstronaut::GetSecondaryRemaining()
{
	return AstronautWeapon->GetSecondaryRemaining();
}

TScriptInterface<ISwitch> AAstronaut::GetDroid_Implementation()
{

	
	ADroid* droid = Cast<ADroid>(Other.GetObject()); //Get object reference

	if (droid == nullptr) { //check object reference
		return TScriptInterface<ISwitch>(nullptr);
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

	if (Other.GetObject() != nullptr) return false; // is droid set

	if (ADroid* thisOne = Cast<ADroid>(this)) return false; // is parameter a droid

	if (droid.GetObject()->Implements<USwitch>()) // sets droid
	{
		Other = TScriptInterface<ISwitch>(droid.GetObject());
		return true;
	}
	return false; // fallback false return
}

void AAstronaut::PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//checking if overlapped actor is valid
	if( (OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;


	//casting overlap to pickupable interface
	IPickupable* pickup = Cast<IPickupable>(OtherActor);
	if (pickup == nullptr) {
		return;
	}

	//checking pickup type
	EPickupType type = pickup->Pickup();

	switch(type) {
	case SALVAGE: // Overlapped actor is Scrap metal
		PickupSalvage(1);
		break;
	case OXYGEN: // Overlapped actor is Oxygen containet
		PickupOxygen(40);
		break;
	default:
		break;
	}

}

void AAstronaut::PickupSalvage(int amount)
{
	for (int i = 0; i < amount; i++) {
		AstronautWeapon->AddSecondaryRemaining();
	}
}

void AAstronaut::PickupOxygen(float amount)
{
	AddOxygen(amount);
}
