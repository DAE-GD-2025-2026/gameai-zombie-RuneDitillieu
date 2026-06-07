// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_T_Flee_DitillieuRune.generated.h"

class ASurvivorPawn;
/**
 * 
 */
UCLASS()
class DITILLIEURUNEZOMBIERUNTIME_API UBT_T_Flee_DitillieuRune : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory) override;
	
private:
	void UpdateBlackboardValues(UBlackboardComponent* BlackBoard) const;
	FVector FleeFromZombies(UBlackboardComponent* BlackBoard, ASurvivorPawn* Survivor);
	FVector FleeFromPurgeZone(UBlackboardComponent* BlackBoard, ASurvivorPawn* Survivor);
	
	bool ShouldRun{ false };
};
