// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Weapon/Weapon.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UWeapon::UWeapon()
{
}

UWeapon::UWeapon(AAstronaut* character)
{
	SetupInput(character);

}

void UWeapon::TickWeapon(float deltaTime)
{
	Cooling(deltaTime);
	FireCooldownTick(deltaTime);

}

void UWeapon::Fire()
{
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Start Fire%f")));


	// This block is taken from FPS template
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Fail Fire, No Character%f")));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Has Character Character%f")));

	// weapon cannot fire while overheated or has recently fired
	if (FireCooldown != 0 || bIsOverheated) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Fail Fire, Overheated%f")));
		return;
	}
	// Try and fire a projectile. This block is taken from FPS template
	if (ProjectileClass != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Has ProjectileClass%f")));
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Has World%f")));
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			if (PlayerController == nullptr) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Fail Fire, No PlayerController%f")));
				return;
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Has PlayerController%f")));
			}
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset) + 100.0f;

			FString rotationString = SpawnRotation.ToString();
			FString locationString = SpawnLocation.ToString();

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, rotationString);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, locationString);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn the projectile at the muzzle
			World->SpawnActor<ALaser>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			// Setting cooldown and adding heat
			FireCooldown = FireRate;
			AddHeat(HeatPerShot);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Fail Fire, No World%f")));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Fail Fire, No Projectile Class%f")));
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Fire End%f")));
}

void UWeapon::Repair()
{

	// weapon cannot repair while overheated or has recently fired
	if(FireCooldown != 0 || bIsOverheated) return;


	//TODO
}

float UWeapon::GetHeat()
{
	return Heat;
}

float UWeapon::Cooling(float deltaTime)
{
	//calculating heat removed
	float heatRemoved = deltaTime * CoolingRate;

	//applying cooling
	AddHeat(heatRemoved);

	return heatRemoved;
}

bool UWeapon::SetupInput(AAstronaut* character)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(character->GetController()))
	{
		Character = character;

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(WeaponMappingContext, 3);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(ActionFire, ETriggerEvent::Triggered, this, &UWeapon::Fire);
			EnhancedInputComponent->BindAction(ActionRepair, ETriggerEvent::Ongoing, this, &UWeapon::Repair);
		}
	}
	return false;
}

void UWeapon::FireCooldownTick(float deltaTime)
{
	if (FireCooldown == 0) return;

	FireCooldown -= deltaTime;

	FString cooldownString = FString::Printf(TEXT("Cooldown: %f"), FireCooldown);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, cooldownString);

	if (FireCooldown < 0) {
		FireCooldown = 0;
	}
}

void UWeapon::SetHeat(float newHeat)
{
	Heat = newHeat;

	ApplyHeatLimits();
}

void UWeapon::AddHeat(float deltaHeat)
{
	//adding heat
	Heat += deltaHeat;

	//testing for head limits
	ApplyHeatLimits();
	
	
}

void UWeapon::ApplyHeatLimits()
{
	if (Heat <= 0) {
		//applying floor
		Heat = 0;
		bIsOverheated = false;
		return;
	}

	if (Heat >= HeatCapacity) {
		// applying roof
		Heat = HeatCapacity;
		bIsOverheated = true;
		return;
	}
}

void UWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(WeaponMappingContext);
		}
	}
}
