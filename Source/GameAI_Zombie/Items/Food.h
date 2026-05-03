// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Food.generated.h"

UCLASS()
class GAMEAI_ZOMBIE_API AFood final : public ABaseItem
{
	GENERATED_BODY()

public:
	AFood();

	virtual void UseItem(ASurvivorPawn& Survivor) override;
};
