// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Weapon/Weapon.h"

// Add default functionality here for any IWeapon functions that are not pure virtual.

bool IWeapon::Secondary()
{
	return GetSecondaryRemaining()>0;
}

int IWeapon::GetSecondaryRemaining()
{
	return SecondaryRemaining;
}

bool IWeapon::AddSecondaryRemaining()
{
	SecondaryRemaining += 1;
	return true;
}
