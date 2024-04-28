// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Weapon/LaserGun.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Environment/Repairable.h>

ULaserGun::ULaserGun()
{
}

ULaserGun::ULaserGun(AAstronaut* character)
{
	SetupInput(character);

}

void ULaserGun::TickWeapon(float deltaTime)
{
	Cooling(deltaTime);
	FireCooldownTick(deltaTime);

}

void ULaserGun::Fire()
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
			//const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
			const FVector SpawnLocation = GetComponentLocation();

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

void ULaserGun::Repair()
{
	// weapon cannot repair while overheated or has recently fired
	if(FireCooldown != 0 || bIsOverheated) return;

	// get player controller for player camera rotation
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController == nullptr) {
		return;
	}

	// Line trace to find repairable object
	FVector StartTrace = GetComponentLocation();
	FVector EndTrace = StartTrace + PlayerController->PlayerCameraManager->GetCameraRotation().Vector() * 500;

	// tracing parameters
	FHitResult HitResult;
	FCollisionQueryParams CollisionsParams;
	CollisionsParams.AddIgnoredActor(GetOwner());
	
	// execute tracing
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, CollisionsParams)) {

		// is traced object Reparable
		IRepairable* repairable = Cast<IRepairable>(HitResult.GetActor());
		if (repairable == nullptr) return;

		// execute repair
		RepairPhase phase = repairable->Repair();
	}

	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, 3.0f, 0, 2.0f);

}

float ULaserGun::GetHeat()
{
	return Heat;
}

float ULaserGun::GetHeatMax()
{
	return HeatCapacity;
}

float ULaserGun::Cooling(float deltaTime)
{
	//calculating heat removed
	float heatRemoved = deltaTime * CoolingRate;

	//applying cooling
	AddHeat(heatRemoved);

	return heatRemoved;
}

bool ULaserGun::SetupInput(AAstronaut* character)
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
			EnhancedInputComponent->BindAction(ActionFire, ETriggerEvent::Triggered, this, &ULaserGun::Fire);
			EnhancedInputComponent->BindAction(ActionRepair, ETriggerEvent::Triggered, this, &ULaserGun::Repair);
		}
	}
	return false;
}

void ULaserGun::FireCooldownTick(float deltaTime)
{
	if (FireCooldown == 0) return;

	FireCooldown -= deltaTime;

	FString cooldownString = FString::Printf(TEXT("Cooldown: %f"), FireCooldown);

	if (FireCooldown < 0) {
		FireCooldown = 0;
	}
}

void ULaserGun::SetHeat(float newHeat)
{
	Heat = newHeat;

	ApplyHeatLimits();
}

void ULaserGun::AddHeat(float deltaHeat)
{
	//adding heat
	Heat += deltaHeat;

	//testing for head limits
	ApplyHeatLimits();
	
	
}

void ULaserGun::ApplyHeatLimits()
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

void ULaserGun::EndPlay(const EEndPlayReason::Type EndPlayReason)
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
