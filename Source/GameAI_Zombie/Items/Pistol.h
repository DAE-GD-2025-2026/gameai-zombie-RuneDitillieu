// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Weapon.h"
#include "Pistol.generated.h"

UCLASS()
class GAMEAI_ZOMBIE_API APistol final : public AWeapon
{
	GENERATED_BODY()

public:
	APistol();
	
	virtual void UseItem(ASurvivorPawn& Survivor) override;
};
