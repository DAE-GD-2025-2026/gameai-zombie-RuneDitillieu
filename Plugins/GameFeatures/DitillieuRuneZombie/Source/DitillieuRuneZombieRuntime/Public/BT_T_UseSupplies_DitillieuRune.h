// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_T_UseSupplies_DitillieuRune.generated.h"

class UInventoryComponent;
class ABaseItem;
/**
 * 
 */
UCLASS()
class DITILLIEURUNEZOMBIERUNTIME_API UBT_T_UseSupplies_DitillieuRune : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory) override;
	
private:
	void GetFoodAndMedkits(const TArray<ABaseItem*>& Items, TArray<int>& Medkits, TArray<int>& Food);
	void UseBestSupply(const TArray<ABaseItem*>& Items, TArray<int>& Supplies, 
		int StatLoss, int& NumSupplies, UInventoryComponent* InventoryComponent);
};
