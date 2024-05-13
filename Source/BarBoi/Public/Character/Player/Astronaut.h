// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Switch.h"
#include "Weapon/Weapon.h"

#include <Components/SphereComponent.h>
#include "Astronaut.generated.h"
class UInputComponent;
class USkeletalMeshComponent;
class ULaserGun;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USphereComponent;
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
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	//ULaserGun* AstronautWeapon;

	/* Weapon for astronaut */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TScriptInterface<IWeapon> AstronautWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* WeaponMesh;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionMain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionSecondary;

public:
	// Sets default values for this character's properties
	AAstronaut();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category=Oxygen)
	void OnDeath();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/* Calls the Main action of the Astronaut's weapon */
	void Main();

	/* Calls the Secondary action of the Astronaut's weapon */
	void Secondary();

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
	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	float GetOxygen();

	/* Returns the maximum oxygen or HP the austronaut can have*/
	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	float GetOxygenMax();

	/* Adds oxygen, and applies restraint for max Oxygen. 
	returns how much oxygen was added. 
	Negative amounts returns 0*/
	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	float AddOxygen(float amount);

	/* Damage done to this character */
	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	bool DamageThis(float damageDone);

	/* Get ammunition of gun */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetAmmo();

	/* Get maximum ammunition of gun */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetAmmoMax();

	/* Can the gun fire? */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool CanFire();

	/* Get secondary actions left of the weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int GetSecondaryRemaining();

	TScriptInterface<ISwitch> GetDroid_Implementation() override;
	TScriptInterface<ISwitch> GetAstronaut_Implementation() override;
	TScriptInterface<ISwitch> GetOther_Implementation() override;
	bool SetAstronaut_Implementation(const TScriptInterface<ISwitch>& astronaut) override;
	bool SetDroid_Implementation(const TScriptInterface<ISwitch>& droid) override;

protected:
	/* Function for picking up a general pickupable actor*/
	UFUNCTION()
	void PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	/* function for picking up salvage */
	UFUNCTION(BlueprintCallable, Category = "Repair", meta = (AllowPrivateAccess = "true"))
	void PickupSalvage(int amount);

	/* function for picking up oxygen pickup */
	UFUNCTION(BlueprintCallable, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	void PickupOxygen(float amount);


};
