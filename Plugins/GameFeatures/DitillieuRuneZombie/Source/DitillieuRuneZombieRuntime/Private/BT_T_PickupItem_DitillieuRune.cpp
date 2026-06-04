// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_PickupItem_DitillieuRune.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UBT_T_PickupItem_DitillieuRune::UBT_T_PickupItem_DitillieuRune()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBT_T_PickupItem_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, 
	FString::Printf(TEXT("Pickup")));
	
	// grab survivor
	AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	// can't pickup if inventory is full
	InventoryComponent = Survivor->GetComponentByClass<UInventoryComponent>();
	if (!InventoryComponent->GetInventory().Contains(nullptr)) return EBTNodeResult::Failed;
	
	BlackBoard = AIController->GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;
	
	// grab items we've seen
	TArray<ABaseItem*> VisibleItems{};
	ABaseItem* Medkit = Cast<ABaseItem>(BlackBoard->GetValueAsObject(FName("ClosestMedkit")));
	if (Medkit) VisibleItems.Add(Medkit);
	ABaseItem* Food = Cast<ABaseItem>(BlackBoard->GetValueAsObject(FName("ClosestFood")));
	if (Food) VisibleItems.Add(Food);
	ABaseItem* Pistol = Cast<ABaseItem>(BlackBoard->GetValueAsObject(FName("ClosestPistol")));
	if (Pistol) VisibleItems.Add(Pistol);
	ABaseItem* Shotgun = Cast<ABaseItem>(BlackBoard->GetValueAsObject(FName("ClosestShotgun")));
	if (Shotgun) VisibleItems.Add(Shotgun);
	
	// can't pickup if we don't see anything
	if (VisibleItems.Num() == 0) return EBTNodeResult::Failed;
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()); //make sure not in wall
	if (!NavSys) return EBTNodeResult::Failed;

	// grab location of first visible item
	FNavLocation ProjectedLocation;
	bool bFoundValidSpot = NavSys->ProjectPointToNavigation(VisibleItems[0]->GetActorLocation(), ProjectedLocation, FVector(200.f, 200.f, 200.f));
	Survivor->StopRunning();

	// if valid, set item to pickup in the coming frames
	if (bFoundValidSpot)
	{
		ItemToPickup = VisibleItems[0];
		ItemLocation = ProjectedLocation;
		NumItems = VisibleItems.Num();
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UBT_T_PickupItem_DitillieuRune::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// move towards item
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(ItemLocation.Location, 
		25.0f, false, true, true, true, 0, true);

	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
		
	// pickup item if close enough
	if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal
		|| (Survivor->GetActorLocation() - ItemLocation.Location).Length() < 50.f)
	{
		int Idx{ 0 };
		for (auto Item : InventoryComponent->GetInventory())
		{
			if (Item == nullptr)
			{
				// remove item from blackboard
				switch (ItemToPickup->GetItemType())
				{
				case EItemType::Medkit:
					BlackBoard->SetValueAsObject(FName("ClosestMedkit"), nullptr);
					BlackBoard->SetValueAsBool(FName("HasSupplies"), true);
					break;
				case EItemType::Food:
					BlackBoard->SetValueAsObject(FName("ClosestFood"), nullptr);
					BlackBoard->SetValueAsBool(FName("HasSupplies"), true);
					break;
				case EItemType::Pistol:
					BlackBoard->SetValueAsObject(FName("ClosestPistol"), nullptr);
					break;
				case EItemType::Shotgun:
					BlackBoard->SetValueAsObject(FName("ClosestShotgun"), nullptr);
					break;
				default:
					break;
				}
					
				// if this was the last registered item,
				// unset blackboard values
				if (NumItems == 1)
				{
					BlackBoard->SetValueAsBool(FName("SensedItem"), false);
						
					if (BlackBoard->GetValueAsBool(FName("SensedDanger")) == false 
						&& BlackBoard->GetValueAsBool(FName("SensedVillage")) == false)
					{
						BlackBoard->SetValueAsBool(FName("SensedSomething"), false);
					}
				}
					
				InventoryComponent->GrabItem(Idx, ItemToPickup);
			}
			++Idx;
		}
			
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}