// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Weapon.generated.h"

UCLASS()
class GAMEAI_ZOMBIE_API AWeapon : public ABaseItem
{
	GENERATED_BODY()

public:
	AWeapon();
	
	int GetDamage() const { return Damage; }
	
protected:
	int Damage{0};
	
	bool Shoot(ASurvivorPawn const & Survivor, FVector const & Direction, APawn*& HitPawnOut) const;
};
