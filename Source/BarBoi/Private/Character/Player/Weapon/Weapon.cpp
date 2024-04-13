// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Weapon/Weapon.h"

UWeapon::UWeapon()
{
}

void UWeapon::Fire()
{
}

float UWeapon::GetHeat()
{
	return Heat;
}

float UWeapon::Cooling(float deltaTime)
{
	return 0.0f;
}

void UWeapon::SetHeat(float newHeat)
{
}

void UWeapon::AddHeat(float deltaHeat)
{
}
