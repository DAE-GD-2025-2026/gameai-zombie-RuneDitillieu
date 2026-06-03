// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_UseSupplies_DitillieuRune.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"
#include "Common/InventoryComponent.h"
#include "Common/HealthComponent.h"
#include "Common/StaminaComponent.h"

EBTNodeResult::Type UBT_T_UseSupplies_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
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
	
	if (HealthLoss < 2 && StaminaLoss < 4.f) return EBTNodeResult::Failed;
	
	
	// grab medkits and food
	UInventoryComponent* InventoryComponent = Survivor->GetComponentByClass<UInventoryComponent>();
	
	auto Items = InventoryComponent->GetInventory();
	TArray<int> Medkits{};
	TArray<int> Food{};
	
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
	
	// can't use any supplies if we don't have any
	if (Medkits.Num() == 0 && Food.Num() == 0) return EBTNodeResult::Failed;
		
	// if we have a medkit, use the one that would heal the most without overflowing
	if (Medkits.Num() > 0)
	{
		int BestMedkitIdx{ Medkits[0] };
		for (int MedkitIdx : Medkits)
		{
			int Value{ Items[MedkitIdx]->GetValue() };
			if (Value <= HealthLoss && Value > Items[BestMedkitIdx]->GetValue())
			{
				BestMedkitIdx = MedkitIdx;
			}
		}
	
		InventoryComponent->UseItem(BestMedkitIdx);
		InventoryComponent->RemoveItem(BestMedkitIdx);
	}
	
	// if we have food, use the one that would refresh the most without overflowing
	if (Food.Num() > 0)
	{
		int BestFoodIdx{ Food[0] };
		for (int FoodIdx : Food)
		{
			int Value{ Items[FoodIdx]->GetValue() };
			if (Value <= HealthLoss && Value > Items[BestFoodIdx]->GetValue())
			{
				BestFoodIdx = FoodIdx;
			}
		}
	
		InventoryComponent->UseItem(BestFoodIdx);
		InventoryComponent->RemoveItem(BestFoodIdx);
	}
	
	return EBTNodeResult::Succeeded;
}
