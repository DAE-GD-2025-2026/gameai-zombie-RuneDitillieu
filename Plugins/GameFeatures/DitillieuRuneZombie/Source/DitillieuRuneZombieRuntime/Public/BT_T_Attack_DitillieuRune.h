// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_T_Attack_DitillieuRune.generated.h"

class UInventoryComponent;
class ASurvivorPawn;

/**
 * 
 */
UCLASS()
class DITILLIEURUNEZOMBIERUNTIME_API UBT_T_Attack_DitillieuRune : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_T_Attack_DitillieuRune();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FVector GetClosestZombieLocation() const;
	void RemoveDeadZombies() const;
	FVector ClosestZombieLoc{};
	float DegreesPerSec{ 720.f };
	
	int WeaponToUseIdx{ -1 };
	
	AAIController* AIController{ nullptr };
	ASurvivorPawn* Survivor{ nullptr };
	UInventoryComponent* InventoryComponent{ nullptr };
	UBlackboardComponent* BlackBoard{ nullptr };
};
