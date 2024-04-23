// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Switch.h"

#include <Components/SphereComponent.h>
#include "Astronaut.generated.h"
class UInputComponent;
class USkeletalMeshComponent;
class UWeapon;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;


UCLASS()
class BARBOI_API AAstronaut : public ACharacter, public ISwitch
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Oxygen", meta = (AllowPrivateAccess = "true"))
	float HPMax = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oxygen", meta = (AllowPrivateAccess = "true"))
	float HP = 150.f;

public:

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
	UInputMappingContext* AstronautMappingContext;

	/* Sphere collision component to register overlapping */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
	USphereComponent* OverlapSphere;

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

	/* Get the vector from Astronaut to Droid
	returns a zero vector if Drone is not set*/
	UFUNCTION(BlueprintCallable, Category = "Towing")
	FVector VectorToDroid();

	/* Get length of vector from Astronaut to Droid
	returns zero if drone is not set*/
	UFUNCTION(BlueprintCallable, Category = "Towing")
	float LengthToDroid();

	/* Move Astronaut to Droid
	This function does not test for length between them*/
	UFUNCTION(BlueprintCallable, Category = "Towing")
	void MoveToDroid();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Returns the oxygen or HP the austronaut has remaining*/
	float GetOxygen();

	/* Returns the maximum oxygen or HP the austronaut can have*/
	float GetOxygenMax();

	/* Damage done to this character */
	bool DamageThis(float damageDone);

	/* Get ammunition of gun */
	float GetAmmo();

	/* Get maximum ammunition of gun */
	float GetAmmoMax();

	TScriptInterface<ISwitch> GetDroid_Implementation() override;
	TScriptInterface<ISwitch> GetAstronaut_Implementation() override;
	TScriptInterface<ISwitch> GetOther_Implementation() override;
	bool SetAstronaut_Implementation(const TScriptInterface<ISwitch>& astronaut) override;
	bool SetDroid_Implementation(const TScriptInterface<ISwitch>& droid) override;

protected:
	void PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
