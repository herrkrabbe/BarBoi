// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Astronaut.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class BARBOI_API AAstronaut : public ACharacter
{
	GENERATED_BODY()

	//Components
	//Note that Character already includes a SkeletalMeshComponent: Mesh

	/* Weapon for astronaut */
	UPROPERTY(VisibleDefaultsOnly, Category="Mesh")
	USkeletalMeshComponent* AstronautWeapon;

	/* Camera for astronaut */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* AstronautCamera;

	/*IMC for astronaut*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* AstronautMapppingContext;

	//Input Actions

	/* Action for Actions. Fire, repair etc. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionAction;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;


public:
	// Sets default values for this character's properties
	AAstronaut();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
