// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_Attack_DitillieuRune.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Items/BaseItem.h"
#include "Zombies/BaseZombie.h"

EBTNodeResult::Type UBT_T_Attack_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
	//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, 
	//FString::Printf(TEXT("Attack")));
	
	// grab survivor
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
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
	
	int WeaponToUseIdx = Pistols.IsEmpty() ? Shotguns[0] : Pistols[0];
	
	FVector ClosestZombieLoc = GetClosestZombieLocation(BlackBoard, Survivor);
	
	// if there's multiple enemies and they're nearby, or you only have a shotgun,
	// use the shotgun with the least ammo
	if ((AmZombies > 1 && (ClosestZombieLoc - Survivor->GetActorLocation()).Length() < 150.f) || Pistols.IsEmpty())
	{
		for (int Shotgun : Shotguns)
		{
			if (Items[WeaponToUseIdx]->GetItemType() == EItemType::Pistol
				|| Items[WeaponToUseIdx]->GetValue() > Items[Shotgun]->GetValue())
			{
				WeaponToUseIdx = Shotgun;
			}
		}
	}
	// otherwise use the pistol with the least ammo
	else
	{
		for (int Pistol : Pistols)
		{
			if (Items[WeaponToUseIdx]->GetValue() > Items[Pistol]->GetValue())
			{
				WeaponToUseIdx = Pistol;
			}
		}
	}
	
	// shoot & remove weapon if it has no ammo left
	InventoryComponent->UseItem(WeaponToUseIdx);
	if (InventoryComponent->GetInventory()[WeaponToUseIdx]->GetValue() == 0)
	{
		InventoryComponent->RemoveItem(WeaponToUseIdx);
	}
		
	RemoveDeadZombies(BlackBoard);
	
	return EBTNodeResult::Succeeded;
}

FVector UBT_T_Attack_DitillieuRune::GetClosestZombieLocation(UBlackboardComponent* BlackBoard, ASurvivorPawn* Survivor) const
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

void UBT_T_Attack_DitillieuRune::RemoveDeadZombies(UBlackboardComponent* BlackBoard) const
{
	// remove killed zombies from tracked list
	ABaseZombie* Zombie1 = Cast<ABaseZombie>(BlackBoard->GetValueAsObject(FName("ClosestZombie1")));
	ABaseZombie* Zombie2 = Cast<ABaseZombie>(BlackBoard->GetValueAsObject(FName("ClosestZombie2")));
	ABaseZombie* Zombie3 = Cast<ABaseZombie>(BlackBoard->GetValueAsObject(FName("ClosestZombie3")));
	
	if (Zombie1 != nullptr && Zombie1->GetComponentByClass<UHealthComponent>()->GetHealth() <= 0)
	{
		Zombie1 = nullptr;
		BlackBoard->SetValueAsObject(FName("ClosestZombie1"), nullptr);
	}
	if (Zombie2 != nullptr && Zombie2->GetComponentByClass<UHealthComponent>()->GetHealth() <= 0)
	{
		Zombie2 = nullptr;
		BlackBoard->SetValueAsObject(FName("ClosestZombie2"), nullptr);
	}
	if (Zombie3 != nullptr && Zombie3->GetComponentByClass<UHealthComponent>()->GetHealth() <= 0)
	{
		Zombie3 = nullptr;
		BlackBoard->SetValueAsObject(FName("ClosestZombie3"), nullptr);
	}
		
	// set updated amount of zombies tracked
	int AmZombies{ 0 };
	if (Zombie1) AmZombies += 1;
	if (Zombie2) AmZombies += 1;
	if (Zombie3) AmZombies += 1;
		
	BlackBoard->SetValueAsInt(FName("AmountOfZombiesTracked"), AmZombies);
	
	// remove danger state if applicable
	if (AmZombies <= 0 && !BlackBoard->GetValueAsObject(FName("ClosestPurgeZone")))
	{
		BlackBoard->SetValueAsBool(FName("SensedDanger"), false);
	}
}
