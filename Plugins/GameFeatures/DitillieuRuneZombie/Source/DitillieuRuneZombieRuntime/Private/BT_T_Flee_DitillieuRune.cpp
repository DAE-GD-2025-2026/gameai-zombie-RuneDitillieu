// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_Flee_DitillieuRune.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "PurgeZones/PurgeZone.h"
#include "Zombies/BaseZombie.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

EBTNodeResult::Type UBT_T_Flee_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
	// grab survivor
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;
	
	FVector FleeDirection{};
	ShouldRun = false;
	
	// calculate flee direction
	FleeDirection += FleeFromPurgeZone(BlackBoard, Survivor);
	FleeDirection += FleeFromZombies(BlackBoard, Survivor);
	
	// reset blackboard vals
	if (BlackBoard->GetValueAsInt(FName("AmountOfZombiesTracked")) == 0 && BlackBoard->GetValueAsObject(FName("ClosestPurgeZone")) == nullptr)
	{
		Survivor->StopRunning();
		ShouldRun = false;
		
		UpdateBlackboardValues(BlackBoard);
	}
	
	// flee
	FleeDirection.Normalize();
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()); //make sure not in wall
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation ProjectedLocation;
	bool bFoundValidSpot = NavSys->ProjectPointToNavigation(Survivor->GetActorLocation() + FleeDirection * 1000.f, 
		ProjectedLocation, FVector(100.f, 100.f, 100.f));

	if (bFoundValidSpot)
	{
		float CurrentStamina{ Survivor->GetComponentByClass<UStaminaComponent>()->GetCurrentStamina() };
		if (!Survivor->IsRunning() && ShouldRun && CurrentStamina >= 3)
		{
			Survivor->StartRunning();
		}
		
		// save energy if almost depleted
		if (Survivor->IsRunning() && CurrentStamina < 3)
		{
			Survivor->StopRunning();
		}
		
		EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(ProjectedLocation.Location,
			50.0f, false, true, true, true, 0, true);
        
		if (MoveResult != EPathFollowingRequestResult::Type::Failed)
		{
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}

FVector UBT_T_Flee_DitillieuRune::FleeFromPurgeZone(UBlackboardComponent* BlackBoard, ASurvivorPawn* Survivor)
{
	APurgeZone* PurgeZone = Cast<APurgeZone>(BlackBoard->GetValueAsObject(FName("ClosestPurgeZone")));
	FVector FleeDirection{};
	if (PurgeZone)
	{
		FleeDirection = (Survivor->GetActorLocation() - PurgeZone->GetActorLocation());
		float Dist{ float(FleeDirection.Length()) };
		
		if (Dist > 500.f)
		{
			BlackBoard->SetValueAsObject(FName("ClosestPurgeZone"), nullptr);
			PurgeZone = nullptr;
			FleeDirection = FVector(0, 0, 0);
		}
		else
		{
			FleeDirection /= Dist;
		}
	}
	
	return FleeDirection;
}

FVector UBT_T_Flee_DitillieuRune::FleeFromZombies(UBlackboardComponent* BlackBoard, ASurvivorPawn* Survivor)
{
	FVector FleeDirection{};
	
	int AmZombies{ BlackBoard->GetValueAsInt(FName("AmountOfZombiesTracked")) };
	if (AmZombies > 0)
	{
		ShouldRun = true;
		
		TArray<ABaseZombie*> Zombies{};
		Zombies.Add(Cast<ABaseZombie>(BlackBoard->GetValueAsObject(FName("ClosestZombie1"))));
		Zombies.Add(Cast<ABaseZombie>(BlackBoard->GetValueAsObject(FName("ClosestZombie2"))));
		Zombies.Add(Cast<ABaseZombie>(BlackBoard->GetValueAsObject(FName("ClosestZombie3"))));
		
		int Idx{ 0 };
		float ClosestZombieDist{ 2000.f };
		for (ABaseZombie* Zombie : Zombies)
		{
			if (Zombie == nullptr)
			{
				++Idx;
				continue;
			}
			
			FVector FleeDir = (Survivor->GetActorLocation() - Zombie->GetActorLocation());
			float Dist{ float(FleeDir.Length()) };
			if (Dist < ClosestZombieDist)
			{
				ClosestZombieDist = Dist;
			}
		
			if (Dist > 1500.f)
			{
				switch (Idx)
				{
				case 0:
					BlackBoard->SetValueAsObject(FName("ClosestZombie1"), nullptr);
					break;
				case 1:
					BlackBoard->SetValueAsObject(FName("ClosestZombie2"), nullptr);
					break;
				case 2:
					BlackBoard->SetValueAsObject(FName("ClosestZombie3"), nullptr);
					break;
				}
				--AmZombies;
			}
			else
			{
				FleeDir /=  Dist * 1.5f;
				FleeDirection += FleeDir;
			}
			++Idx;
		}
		
		// only sprint if a zombie is actually close
		if (ClosestZombieDist > 500.f)
		{
			Survivor->StopRunning();
			ShouldRun = false;
		}
		
		BlackBoard->SetValueAsInt(FName("AmountOfZombiesTracked"), AmZombies);
	}
	
	return FleeDirection;
}

void UBT_T_Flee_DitillieuRune::UpdateBlackboardValues(UBlackboardComponent* BlackBoard) const
{
	BlackBoard->SetValueAsBool(FName("SensedDanger"), false);
		
	if (BlackBoard->GetValueAsBool(FName("SensedItem")) == false 
		&& BlackBoard->GetValueAsBool(FName("SensedVillage")) == false)
	{
		BlackBoard->SetValueAsBool(FName("SensedSomething"), false);
	}
}
