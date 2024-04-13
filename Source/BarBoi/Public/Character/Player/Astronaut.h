// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Astronaut.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UWeapon;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;


UCLASS()
class BARBOI_API AAstronaut : public ACharacter
{
	GENERATED_BODY()

public:
	enum gunState {
		FIRE = 0,
		REPAIR = 1
	};

	//Components
	//Note that Character already includes a SkeletalMeshComponent: Mesh

	/* Weapon for astronaut */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	UWeapon* AstronautWeapon;

	/* Camera for astronaut */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* AstronautCamera;


	/*IMC for astronaut*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* AstronautMapppingContext;

	//Input Actions

	/* Action for Actions. Fire, repair etc. */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	//UInputAction* ActionAction;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionLook;

public:
	// Sets default values for this character's properties
	AAstronaut();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	gunState eGunState;

public:
	/* Get state of Astronaut Gun
FIRE = 0
REPAIR = 1*/
	gunState getGunState();

private:
	/* Set state of Astronaut Gun
FIRE
REPAIR
*/
	void SetGunState(gunState fireMode);

};
