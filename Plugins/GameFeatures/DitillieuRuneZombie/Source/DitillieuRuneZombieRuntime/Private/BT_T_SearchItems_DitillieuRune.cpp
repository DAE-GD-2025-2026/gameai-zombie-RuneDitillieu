// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_SearchItems_DitillieuRune.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Village/House/House.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UBT_T_SearchItems_DitillieuRune::UBT_T_SearchItems_DitillieuRune()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBT_T_SearchItems_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, 
	FString::Printf(TEXT("Search")));
	
	// grab survivor
	AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	BlackBoard = AIController->GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()); //make sure not in wall
	if (!NavSys) return EBTNodeResult::Failed;
	
	// don't visit houses we already entered
	TargetHouse = Cast<AHouse>(BlackBoard->GetValueAsObject(FName("ClosestHouse")));
	if (VisitedHouses.Contains(TargetHouse))
	{
		BlackBoard->SetValueAsBool(FName("SensedVillage"), false);
		return EBTNodeResult::Failed;
	}
	
	// grab house position
	FVector HousePos{ TargetHouse->GetActorLocation() };
	FNavLocation ProjectedLocation;
	bool bFoundValidSpot = NavSys->ProjectPointToNavigation(HousePos, ProjectedLocation, 
		FVector(200.f, 200.f, 200.f));
	
	if (bFoundValidSpot)
	{
		HouseLocation = ProjectedLocation;
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UBT_T_SearchItems_DitillieuRune::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (BlackBoard->GetValueAsBool(FName("SensedItem")))
	{
		VisitedHouses.Add(TargetHouse);
		BlackBoard->SetValueAsBool(FName("SensedVillage"), false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	// move towards item
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(HouseLocation.Location, 
		25.0f, false, true, true, true, 0, true);

	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	
	if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal 
		|| (Survivor->GetActorLocation() - HouseLocation.Location).Length() < 50.f)
	{
		AHouse* House{ Cast<AHouse>(BlackBoard->GetValueAsObject(FName("ClosestHouse"))) };
		if (House == TargetHouse)
		{
			BlackBoard->SetValueAsObject(FName("ClosestHouse"), nullptr);
			BlackBoard->SetValueAsBool(FName("SensedVillage"), false);
		}
		
		VisitedHouses.Add(TargetHouse);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}