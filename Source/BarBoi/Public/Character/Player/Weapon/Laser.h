// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"

#include <Components/CapsuleComponent.h>
#include "Laser.generated.h"


class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS()
class BARBOI_API ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();

public:	


	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	/* Projectile Movement Component of Laser*/
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collission")
	UCapsuleComponent* CollisionComp;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UMeshComponent* LaserMesh;


};
