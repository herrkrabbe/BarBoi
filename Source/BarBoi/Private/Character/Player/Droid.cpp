// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/Droid.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include <Character/Player/Astronaut.h>

// Sets default values
ADroid::ADroid()
{
	GetCapsuleComponent()->InitCapsuleSize(30.f, 30.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// configure droid movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->MaxFlySpeed = SpeedMax;
	GetCharacterMovement()->BrakingDecelerationFlying = SpeedDeceleration;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	//GRAVITY IS DISABLED AND FLYING ENABLED IN BeginPlay()
	GetCharacterMovement()->GravityScale = 0.f;
	

 	// create springarm 
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);// attach springarm to capsule
	SpringArmComponent->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// create camera
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCameraComponent"));
	ThirdPersonCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);//attach camera to springarm
	ThirdPersonCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	

}

// Called when the game starts or when spawned
void ADroid::BeginPlay()
{
	Super::BeginPlay();

	//enable flying
	//GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying); //Enable the doird to fly
	GetCharacterMovement()->AirControl = 1.f;
	
	//if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
	if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DroidMappingContext, 1);
		}
	}
	else {
	}
	
}

// Called every frame
void ADroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADroid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ADroid::Move);

		// Looking
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ADroid::Look);

		
	}
	else
	{
		//insert error here
	}

}

void ADroid::Move(const FInputActionValue& Value)
{
	FVector MovementVector = Value.Get<FVector>();


	if (Controller != nullptr)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		FString ForwardString = "Forward: ";
		ForwardString.Append(ForwardDirection.ToString());

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		FString RightString = "Right: ";
		RightString.Append(RightDirection.ToString());



		//const FRotator PitchRotation(Rotation.Pitch, 0, Rotation.Pitch);
		//const FVector UpDirection = FRotationMatrix(PitchRotation).GetUnitAxis(EAxis::Z);
		const FVector UpDirection = FVector(0, 0, 1);

		FString UpString = "Up: ";
		UpString.Append(UpDirection.ToString());


		AddMovementInput(UpDirection, MovementVector.Z);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
		
		

		// add movement 
		//AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		//AddMovementInput(GetActorRightVector(), MovementVector.X);
		//AddMovementInput(GetActorUpVector(), MovementVector.Z);
	}

}

void ADroid::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
	else {

	}
}

TScriptInterface<ISwitch> ADroid::GetDroid_Implementation()
{
	return TScriptInterface<ISwitch>(this);
}

TScriptInterface<ISwitch> ADroid::GetAstronaut_Implementation()
{
	AAstronaut* astronaut = Cast<AAstronaut>(Other.GetObject()); //Get object reference

	if (astronaut == nullptr) { //check object reference
		
	}

	return TScriptInterface<ISwitch>(astronaut);
}

TScriptInterface<ISwitch> ADroid::GetOther_Implementation()
{
	return Other;
}

bool ADroid::SetAstronaut_Implementation(const TScriptInterface<ISwitch>& astronaut)
{

	if (Other.GetObject() != nullptr) {
		return false;
	}

	if (AAstronaut* thisOne = Cast<AAstronaut>(this)) return false;

	if (astronaut.GetObject()->Implements<USwitch>())
	{
		Other = TScriptInterface<ISwitch>(astronaut.GetObject());
		return true;
	}
	return false;
}

bool ADroid::SetDroid_Implementation(const TScriptInterface<ISwitch>& droid)
{
	return false;
}
