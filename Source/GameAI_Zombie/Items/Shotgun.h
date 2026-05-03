// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Shotgun.generated.h"

UCLASS()
class GAMEAI_ZOMBIE_API AShotgun : public AWeapon
{
	GENERATED_BODY()

public:
	AShotgun();
	virtual void UseItem(ASurvivorPawn& Survivor) override;
	
private:
	int const ShotsPerAmmo{3};
	float MaxSprayDelta{10.0f};
};
