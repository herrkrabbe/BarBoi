// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "MyBTAIController.generated.h"

/**
 * 
 */
UCLASS()
class BARBOI_API AMyBTAIController : public AAIController
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;
	
};
