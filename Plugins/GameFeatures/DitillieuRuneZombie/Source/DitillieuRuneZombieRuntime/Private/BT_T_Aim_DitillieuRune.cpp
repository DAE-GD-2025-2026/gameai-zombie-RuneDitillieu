// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_Aim_DitillieuRune.h"

#include "Zombies/BaseZombie.h"
#include "AIController.h"
#include "Survivor/SurvivorPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"

UBT_T_Aim_DitillieuRune::UBT_T_Aim_DitillieuRune()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBT_T_Aim_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
	// grab survivor
	AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	BlackBoard = AIController->GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;
	
	int AmZombies{ BlackBoard->GetValueAsInt(FName("AmountOfZombiesTracked")) };
	if (AmZombies == 0) return EBTNodeResult::Failed;
	
	UInventoryComponent* InventoryComponent = Survivor->GetComponentByClass<UInventoryComponent>();
	auto Items{ InventoryComponent->GetInventory() };
	
	if (Items.IsEmpty()) return EBTNodeResult::Failed;
	
	TArray<int> Pistols{};
	TArray<int> Shotguns{};
	int Idx{ 0 };
	for (ABaseItem* Item : Items)
	{
		if (Item == nullptr)
		{
			++Idx;
			continue;
		}
		
		if (Item->GetItemType() == EItemType::Pistol)
		{
			Pistols.Add(Idx);
		}
		else if (Item->GetItemType() == EItemType::Shotgun)
		{
			Shotguns.Add(Idx);
		}
		
		++Idx;
	}
	
	// if no weapons available, can't attack
	if (Pistols.IsEmpty() && Shotguns.IsEmpty()) return EBTNodeResult::Failed;
	
	ClosestZombieLoc = GetClosestZombieLocation();
	
	// don't attack if closest zombie is too far to aim reliably
	if ((ClosestZombieLoc - Survivor->GetActorLocation()).Length() > 500.f)
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::InProgress;
}

FVector UBT_T_Aim_DitillieuRune::GetClosestZombieLocation() const
{
	TArray<ABaseZombie*> Zombies{};
	Zombies.Add(Cast<ABaseZombie>(BlackBoard->GetValueAsObject(FName("ClosestZombie1"))));
	Zombies.Add(Cast<ABaseZombie>(BlackBoard->GetValueAsObject(FName("ClosestZombie2"))));
	Zombies.Add(Cast<ABaseZombie>(BlackBoard->GetValueAsObject(FName("ClosestZombie3"))));
	
	// grab location of closest zombie
	FVector LookAtDir{ Survivor->GetActorLocation() + Survivor->GetActorForwardVector() * 1000.f };
	for (ABaseZombie* Zombie : Zombies)
	{
		if (Zombie == nullptr) continue;
		
		if ((Zombie->GetActorLocation() - Survivor->GetActorLocation()).SquaredLength()
			< (LookAtDir - Survivor->GetActorLocation()).SquaredLength())
		{
			LookAtDir = Zombie->GetActorLocation();
		}
	}
	
	return LookAtDir;
}

void UBT_T_Aim_DitillieuRune::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ClosestZombieLoc = GetClosestZombieLocation();
	
	FVector2D DirToTarget{ ClosestZombieLoc - Survivor->GetActorLocation() };
	DirToTarget /= DirToTarget.Length();

	// calc direction to rotate
	const float AgentForwardAngle{ static_cast<float>(Survivor->GetActorRotation().Yaw) / 180.f * PI };
	const float AngleToTarget{ static_cast<float>(atan2(DirToTarget.Y, DirToTarget.X))};

	float AngleDiff{ AngleToTarget - AgentForwardAngle };
	if (abs(AngleDiff) < 0.01f)
		AngleDiff = 0.f;
	else if (AngleDiff >  PI)
		AngleDiff -= 2 * PI;
	else if (AngleDiff < -PI)
		AngleDiff += 2 * PI;
	
	// convert to degrees
	AngleDiff = AngleDiff / PI * 180.f;
	if (abs(AngleDiff) < 7.5f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		// continue rotating
		float DegreesToRotate{ AngleDiff / abs(AngleDiff) * DeltaSeconds * DegreesPerSec };
		Survivor->AddActorWorldRotation(FRotator(0.0f, DegreesToRotate, 0.0f));
	}
}