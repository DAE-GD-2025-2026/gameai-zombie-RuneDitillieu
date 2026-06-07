// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_UseSupplies_DitillieuRune.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Common/InventoryComponent.h"
#include "Common/HealthComponent.h"
#include "Common/StaminaComponent.h"

EBTNodeResult::Type UBT_T_UseSupplies_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, 
	FString::Printf(TEXT("UseSupplies")));
	
	// grab survivor
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	// grab components
	UHealthComponent* HealthComponent = Survivor->GetComponentByClass<UHealthComponent>();
	UStaminaComponent* StaminaComponent = Survivor->GetComponentByClass<UStaminaComponent>();
	
	// if health and stamina are almost at max, no need to use any supplies
	int HealthLoss{ HealthComponent->GetMaxHealth() - HealthComponent->GetHealth() };
	float StaminaLoss{ StaminaComponent->GetMaxStamina() - StaminaComponent->GetCurrentStamina() };
	
	if (HealthLoss < 1 && StaminaLoss < 1.f) return EBTNodeResult::Failed;
	
	
	// grab medkits and food
	UInventoryComponent* InventoryComponent = Survivor->GetComponentByClass<UInventoryComponent>();
	
	auto Items = InventoryComponent->GetInventory();
	TArray<int> Medkits{};
	TArray<int> Food{};
	GetFoodAndMedkits(Items, Medkits, Food);
	
	// can't use any supplies if we don't have any
	if (Medkits.Num() == 0 && Food.Num() == 0) return EBTNodeResult::Failed;
	
	int NumSupplies{ Medkits.Num() + Food.Num() };
	
	// if we have supplies, use the one that would replenish the most without overflowing
	UseBestSupply(Items, Medkits, HealthLoss, NumSupplies, InventoryComponent);
	UseBestSupply(Items, Food, StaminaLoss, NumSupplies, InventoryComponent);
	
	if (HealthComponent->GetHealth() > 4 && StaminaComponent->GetCurrentStamina() > 4)
	{
		UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
		if (!BlackBoard) return EBTNodeResult::Failed;
		
		BlackBoard->SetValueAsBool(FName("IsOnLifeSupport"), false);
	}
	
	if (NumSupplies <= 0)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("HasSupplies"), false);
	}
	
	return EBTNodeResult::Succeeded;
}

void UBT_T_UseSupplies_DitillieuRune::GetFoodAndMedkits(const TArray<ABaseItem*>& Items, TArray<int>& Medkits, TArray<int>& Food)
{
	int Idx{ 0 };
	for (ABaseItem* Item : Items)
	{
		if (Item != nullptr && Item->GetItemType() == EItemType::Medkit)
		{
			Medkits.Add(Idx);
		}
		else if (Item != nullptr && Item->GetItemType() == EItemType::Food)
		{
			Food.Add(Idx);
		}
		
		++Idx;
	}
}

void UBT_T_UseSupplies_DitillieuRune::UseBestSupply(const TArray<ABaseItem*>& Items, TArray<int>& Supplies,
	int StatLoss, int& NumSupplies, UInventoryComponent* InventoryComponent)
{
	if (Supplies.Num() > 0)
	{
		int BestSupplyIdx{ Supplies[0] };
		for (int SupplyIdx : Supplies)
		{
			int Value{ Items[SupplyIdx]->GetValue() };
			if (Value <= StatLoss && Value > Items[BestSupplyIdx]->GetValue())
			{
				BestSupplyIdx = SupplyIdx;
			}
		}
	
		// only use if it won't overflow
		if (Items[BestSupplyIdx]->GetValue() <= StatLoss)
		{
			InventoryComponent->UseItem(BestSupplyIdx);
			InventoryComponent->RemoveItem(BestSupplyIdx);
			--NumSupplies;
		}
	}
}
