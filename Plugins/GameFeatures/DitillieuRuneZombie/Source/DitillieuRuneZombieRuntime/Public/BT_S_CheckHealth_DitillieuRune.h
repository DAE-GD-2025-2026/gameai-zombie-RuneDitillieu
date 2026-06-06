// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BT_S_CheckHealth_DitillieuRune.generated.h"

class UStaminaComponent;
class UHealthComponent;
class ASurvivorPawn;
/**
 * 
 */
UCLASS()
class DITILLIEURUNEZOMBIERUNTIME_API UBT_S_CheckHealth_DitillieuRune : public UBTService
{
	GENERATED_BODY()
	
public:
	UBT_S_CheckHealth_DitillieuRune();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override; 
	
private:
	UHealthComponent* HealthComponent{ nullptr };
	UStaminaComponent* StaminaComponent{ nullptr };
	UBlackboardComponent* BlackBoard{ nullptr };
	
	int PrevHealth{ 10 };
	bool TookDamageLastTick{ false };
};
