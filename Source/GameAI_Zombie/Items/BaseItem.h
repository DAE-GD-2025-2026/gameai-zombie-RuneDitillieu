// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemType.h"
#include "GameFramework/Actor.h"
#include "Survivor/SurvivorPawn.h"
#include "BaseItem.generated.h"

UCLASS()
class GAMEAI_ZOMBIE_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	ABaseItem();
	
	virtual void UseItem(ASurvivorPawn& Survivor);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetValue() const {return Value;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EItemType GetItemType() const {return ItemType;}

protected:
	EItemType ItemType{EItemType::Garbage};
	int Value{0};
};
