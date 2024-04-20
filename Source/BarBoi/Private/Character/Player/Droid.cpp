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
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Droid can access controller")));
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DroidMappingContext, 1);
		}
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Droid can NOT access controller")));
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
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("INPUT SETUP FAILED")));
		//insert error here
	}

}

void ADroid::Move(const FInputActionValue& Value)
{
	FVector MovementVector = Value.Get<FVector>();


	if (Controller != nullptr)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, MovementVector.ToString());

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		FString ForwardString = "Forward: ";
		ForwardString.Append(ForwardDirection.ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ForwardString);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		FString RightString = "Right: ";
		RightString.Append(RightDirection.ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, RightString);



		//const FRotator PitchRotation(Rotation.Pitch, 0, Rotation.Pitch);
		//const FVector UpDirection = FRotationMatrix(PitchRotation).GetUnitAxis(EAxis::Z);
		const FVector UpDirection = FVector(0, 0, 1);

		FString UpString = "Up: ";
		UpString.Append(UpDirection.ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, UpString);

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, GetCharacterMovement()->GetMovementName());

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
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CANNOT LOOK")));
	}
}

TScriptInterface<ISwitch> ADroid::GetDroid_Implementation()
{


	ADroid* droid = Cast<ADroid>(Other.GetObject()); //Get object reference

	if (droid == nullptr) { //check object reference
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Astronaut cannot find droid")));
	}

	return TScriptInterface<ISwitch>(droid);
}

TScriptInterface<ISwitch> ADroid::GetAstronaut_Implementation()
{
	return TScriptInterface<ISwitch>(this); // convert this to variable holding 
}

TScriptInterface<ISwitch> ADroid::GetOther_Implementation()
{
	return Other;
}

bool ADroid::SetAstronaut_Implementation(const TScriptInterface<ISwitch>& astronaut)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Setting Astronaut in Droid")));

	if (Other.GetObject() != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Other is not null")));
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
