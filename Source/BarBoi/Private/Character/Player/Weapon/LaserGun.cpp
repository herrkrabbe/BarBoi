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
	SetCollisionResponseToAllChannels(ECR_Ignore);

	static ConstructorHelpers::FClassFinder<ALaser> ProjectileFile(TEXT("/Game/Group_11/Blueprints/BP_Laser.BP_Laser_C"));
	
	if (ProjectileFile.Class != nullptr) {
		ProjectileClass = ProjectileFile.Class;
	}
		
}

void ULaserGun::TickWeapon(float deltaTime)
{
	Cooling(deltaTime);
	FireCooldownTick(deltaTime);
}

bool ULaserGun::Fire()
{

	// weapon cannot fire while overheated or has recently fired
	if (FireCooldown != 0 || bIsOverheated) {
		
		return false;

	}
	// Try and fire a projectile. This block is taken from FPS template
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
			if (PlayerController == nullptr) {
				return false;
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
			return true;
		}
	}
	return false;
}

bool ULaserGun::Repair()
{
	// weapon cannot repair while overheated or has recently fired or has no secondary actions remaining
	if(FireCooldown != 0 || bIsOverheated || SecondaryRemaining < 1) return false;
	//if (FireCooldown != 0 || bIsOverheated) return false;

	// get player controller for player camera rotation
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr) {
		return false;
	}

	// Line trace to find repairable object
	FVector StartTrace = GetComponentLocation();
	FVector EndTrace = StartTrace + PlayerController->PlayerCameraManager->GetCameraRotation().Vector() * 500;

	// tracing parameters
	FHitResult HitResult;
	FCollisionQueryParams CollisionsParams;
	CollisionsParams.AddIgnoredActor(GetOwner());


	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, 3.0f, 0, 2.0f);


	// execute tracing
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, CollisionsParams)) {

		// is traced object Reparable
		IRepairable* repairable = Cast<IRepairable>(HitResult.GetActor());
		if (repairable == nullptr) return false;

		// execute repair
		ERepairPhase phase = repairable->Repair();

		bool bConsumesSecondary = false;

		switch (phase) {
		case REPAIR_COMPLETED: // consumes secondary available
			bConsumesSecondary = true;
			SecondaryRemaining -= 1;
			break;
		}


		return bConsumesSecondary;
	}

	
	return false;
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

/*
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
*/

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

void ULaserGun::BeginPlay()
{
	Super::BeginPlay();
	
}



bool ULaserGun::Main()
{
	
	return Fire();
}

bool ULaserGun::Secondary()
{
	return Repair();
}

float ULaserGun::GetAmmoMax()
{
	return GetHeatMax();
}

float ULaserGun::GetAmmo()
{
	return GetHeat();
}

bool ULaserGun::CanFire()
{
	return !bIsOverheated;
}
