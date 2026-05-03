// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Medkit.generated.h"

UCLASS()
class GAMEAI_ZOMBIE_API AMedkit final : public ABaseItem
{
	GENERATED_BODY()

public:
	AMedkit();
	
	virtual void UseItem(ASurvivorPawn& Survivor) override;
};
