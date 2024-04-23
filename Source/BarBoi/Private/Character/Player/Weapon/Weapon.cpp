// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Weapon/Weapon.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Environment/Repairable.h>

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


	// This block is taken from FPS template
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// weapon cannot fire while overheated or has recently fired
	if (FireCooldown != 0 || bIsOverheated) {
		return;
	}
	// Try and fire a projectile. This block is taken from FPS template
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			if (PlayerController == nullptr) {
				return;
			}

			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			FString rotationString = SpawnRotation.ToString();
			FString locationString = SpawnLocation.ToString();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn the projectile at the muzzle
			World->SpawnActor<ALaser>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			// Setting cooldown and adding heat
			FireCooldown = FireRate;
			AddHeat(HeatPerShot);
		}
	}
}

void UWeapon::Repair()
{

	// weapon cannot repair while overheated or has recently fired
	if(FireCooldown != 0 || bIsOverheated) return;


	FVector StartTrace = GetComponentLocation();
	FVector EndTrace = StartTrace + GetComponentRotation().Vector() * 100;

	FHitResult HitResult;
	FCollisionQueryParams CollisionsParams;
	CollisionsParams.AddIgnoredActor(GetOwner());
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, CollisionsParams)) {
		IRepairable* repairable = Cast<IRepairable>(HitResult.GetActor());
		if (repairable == nullptr) return;

		repairable->Repair();
	}

}

float UWeapon::GetHeat()
{
	return Heat;
}

float UWeapon::GetHeatMax()
{
	return HeatCapacity;
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
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, cooldownString);

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
