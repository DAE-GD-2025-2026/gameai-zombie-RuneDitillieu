// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_T_PickupItem_DitillieuRune.generated.h"

/**
 * 
 */
UCLASS()
class DITILLIEURUNEZOMBIERUNTIME_API UBT_T_PickupItem_DitillieuRune : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory) override;
};
