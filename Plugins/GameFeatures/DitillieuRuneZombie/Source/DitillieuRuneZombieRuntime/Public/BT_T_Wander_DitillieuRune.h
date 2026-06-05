// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_T_Wander_DitillieuRune.generated.h"

/**
 * 
 */
UCLASS()
class DITILLIEURUNEZOMBIERUNTIME_API UBT_T_Wander_DitillieuRune : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory) override;
	
private:
	float m_OffsetDistance{ 160.f };
	float m_Radius{ 100.f };	
	int m_MaxAngleChange{ 15 };			
	float m_WanderAngle{ 0.f };	
};
