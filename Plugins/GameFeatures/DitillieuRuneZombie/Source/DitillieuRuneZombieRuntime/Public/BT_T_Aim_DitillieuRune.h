// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_T_Aim_DitillieuRune.generated.h"

class ASurvivorPawn;
/**
 * 
 */
UCLASS()
class DITILLIEURUNEZOMBIERUNTIME_API UBT_T_Aim_DitillieuRune : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_T_Aim_DitillieuRune();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FVector GetClosestZombieLocation() const;
	FVector ClosestZombieLoc{};
	float DegreesPerSec{ 720.f };
	
	AAIController* AIController{ nullptr };
	ASurvivorPawn* Survivor{ nullptr };
	UBlackboardComponent* BlackBoard{ nullptr };
};
