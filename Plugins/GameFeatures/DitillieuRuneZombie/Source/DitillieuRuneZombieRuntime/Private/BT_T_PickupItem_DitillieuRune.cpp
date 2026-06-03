// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_PickupItem_DitillieuRune.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

EBTNodeResult::Type UBT_T_PickupItem_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, 
	FString::Printf(TEXT("Pickup")));
	// grab survivor
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	// can't pickup if inventory is full
	UInventoryComponent* InventoryComponent = Survivor->GetComponentByClass<UInventoryComponent>();
	if (!InventoryComponent->GetInventory().Contains(nullptr)) return EBTNodeResult::Failed;
	
	UBlackboardComponent* blackBoard = AIController->GetBlackboardComponent();
	if (!blackBoard) return EBTNodeResult::Failed;
	
	// grab items we've seen
	TArray<ABaseItem*> VisibleItems{};
	ABaseItem* Medkit = Cast<ABaseItem>(blackBoard->GetValueAsObject(FName("ClosestMedkit")));
	if (Medkit) VisibleItems.Add(Medkit);
	ABaseItem* Food = Cast<ABaseItem>(blackBoard->GetValueAsObject(FName("ClosestFood")));
	if (Food) VisibleItems.Add(Food);
	ABaseItem* Pistol = Cast<ABaseItem>(blackBoard->GetValueAsObject(FName("ClosestPistol")));
	if (Pistol) VisibleItems.Add(Pistol);
	ABaseItem* Shotgun = Cast<ABaseItem>(blackBoard->GetValueAsObject(FName("ClosestShotgun")));
	if (Shotgun) VisibleItems.Add(Shotgun);
	
	// can't pickup if we don't see anything
	if (VisibleItems.Num() == 0) return EBTNodeResult::Failed;
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()); //make sure not in wall
	if (!NavSys) return EBTNodeResult::Failed;

	// grab location of first visible item
	FNavLocation ProjectedLocation;
	bool bFoundValidSpot = NavSys->ProjectPointToNavigation(VisibleItems[0]->GetActorLocation(), ProjectedLocation, FVector(200.f, 200.f, 200.f));
	Survivor->StopRunning();

	if (bFoundValidSpot)
	{
		EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(ProjectedLocation.Location, 50.0f, false, true, true, true, 0, true);
        
		// go to item
		if (MoveResult == EPathFollowingRequestResult::Type::RequestSuccessful)
		{
			return EBTNodeResult::InProgress;
		}
		
		// pickup item
		if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			int Idx{ 0 };
			for (auto Item : InventoryComponent->GetInventory())
			{
				if (Item == nullptr)
				{
					switch (VisibleItems[0]->GetItemType())
					{
					case EItemType::Medkit:
						blackBoard->SetValueAsObject(FName("ClosestMedkit"), nullptr);
						break;
					case EItemType::Food:
						blackBoard->SetValueAsObject(FName("ClosestFood"), nullptr);
						break;
					case EItemType::Pistol:
						blackBoard->SetValueAsObject(FName("ClosestPistol"), nullptr);
						break;
					case EItemType::Shotgun:
						blackBoard->SetValueAsObject(FName("ClosestShotgun"), nullptr);
						break;
					}
					
					if (VisibleItems.Num() == 1)
					{
						blackBoard->SetValueAsBool(FName("SensedItem"), false);
						
						if (blackBoard->GetValueAsBool(FName("SensedDanger")) == false 
							&& blackBoard->GetValueAsBool(FName("SensedVillage")) == false)
						{
							blackBoard->SetValueAsBool(FName("SensedSomething"), false);
						}
					}
					
					InventoryComponent->GrabItem(Idx, VisibleItems[0]);
				}
				++Idx;
			}
			
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, 
	FString::Printf(TEXT("Pickup completed")));
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
