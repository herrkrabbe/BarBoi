// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"

#include <Components/BoxComponent.h>
#include "Laser.generated.h"


class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class BARBOI_API ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();

	virtual void BeginPlay() override;

public:	


	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnLaserBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float speedMax = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float speedInitial = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage = 1.0f;

protected:
	/* Projectile Movement Component of Laser*/
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collission")
	UBoxComponent* CollisionComp;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	//UMeshComponent* LaserMesh;


};
