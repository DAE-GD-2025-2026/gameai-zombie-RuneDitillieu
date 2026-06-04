// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_Flee_DitillieuRune.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "PurgeZones/PurgeZone.h"
#include "Zombies/BaseZombie.h"

EBTNodeResult::Type UBT_T_Flee_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, 
	FString::Printf(TEXT("Flee")));
	
	// grab survivor
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;
	
	FVector FleeDirection{};
	bool ShouldRun{ false };
	
	// flee from purgezone
	APurgeZone* PurgeZone = Cast<APurgeZone>(BlackBoard->GetValueAsObject(FName("ClosestPurgeZone")));
	if (PurgeZone)
	{
		FleeDirection = (Survivor->GetActorLocation() - PurgeZone->GetActorLocation());
		float Dist{ float(FleeDirection.Length()) };
		
		if (Dist > 1500.f)
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
	
	// flee from zombies
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
		
			if (Dist > 2000.f)
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
	
	if (AmZombies == 0 && PurgeZone == nullptr)
	{
		Survivor->StopRunning();
		ShouldRun = false;
		BlackBoard->SetValueAsBool(FName("SensedDanger"), false);
		
		if (BlackBoard->GetValueAsBool(FName("SensedItem")) == false 
			&& BlackBoard->GetValueAsBool(FName("SensedVillage")) == false)
		{
			BlackBoard->SetValueAsBool(FName("SensedSomething"), false);
		}
	}
	
	// flee
	TArray<FVector> pathPoints = Survivor->CalculatePath(Survivor->GetActorLocation() + FleeDirection * 1000.f);

	if (pathPoints.Num() != 0)
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
		
		AIController->MoveToLocation(pathPoints[1], 50.0f, 
			false, true, true, true, 0, true);
		return EBTNodeResult::Succeeded; 
	}
	
	return EBTNodeResult::Failed;
}
