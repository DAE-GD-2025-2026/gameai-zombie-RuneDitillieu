// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_T_PickupItem_DitillieuRune.generated.h"

class ASurvivorPawn;
class UInventoryComponent;
class ABaseItem;
/**
 * 
 */
UCLASS()
class DITILLIEURUNEZOMBIERUNTIME_API UBT_T_PickupItem_DitillieuRune : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_T_PickupItem_DitillieuRune();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	ABaseItem* GetBestItemToPickup(TArray<ABaseItem*> KnownItems);
	void RemoveTripledItems();
	
private:
	ABaseItem* ItemToPickup{ nullptr };
	FNavLocation ItemLocation{};
	
	AAIController* AIController{ nullptr };
	ASurvivorPawn* Survivor{ nullptr };
	UInventoryComponent* InventoryComponent{ nullptr };
	UBlackboardComponent* BlackBoard{ nullptr };
	int NumItems{ 0 };
};
