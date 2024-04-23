// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"




#include "Switch.generated.h"



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USwitch : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface to help astronaut and droid switch possession of the player controller
 */
class BARBOI_API ISwitch
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	TScriptInterface<ISwitch> Other;
public:
	
	// returns the other character the player can switch to
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Get")
	TScriptInterface<ISwitch> GetOther();
	
	// 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Get")
	TScriptInterface<ISwitch> GetAstronaut();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Get")
	TScriptInterface<ISwitch> GetDroid();

	/* Set Droid Value. also sets Other to droid
	
	returns true if droid is set
	returns false if drone has been set
	returns false if this is not an AAstronaut*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Set")
	bool SetDroid(const TScriptInterface<ISwitch>& droid);

	/* Set Astronaut Value. also sets Other to astronaut

	returns true if astronaut is set
	returns false if astronaut has been set
	returns false if this is not an ADroid*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Set")
	bool SetAstronaut(const TScriptInterface<ISwitch>& astronaut);


	/*Makes Controller possess the other ISwitch pawn*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Input")
	void Switch();
};
