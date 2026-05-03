// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"

#include "ItemSpawnZone.h"
#include "Kismet/GameplayStatics.h"


AItemManager::AItemManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItemManager::BeginPlay()
{
	Super::BeginPlay();
}

void AItemManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ItemsInWorld.Num() < MaxItemsInWorld && SpawnZones.Num() > 0)
	{
		int const AmountToSpawn = MaxItemsInWorld - ItemsInWorld.Num();
		for (int i = 0; i < AmountToSpawn; ++i)
		{
			ItemsInWorld.Add(SpawnZones[FMath::RandRange(0, SpawnZones.Num() - 1)]
				->SpawnItemOfType(TypesToSpawn[FMath::RandRange(0, TypesToSpawn.Num() - 1)]));
		}
	}
}

void AItemManager::RegisterHouses()
{
	TArray<AActor*> FoundActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawnZone::StaticClass(), FoundActors);
	for (AActor* FoundActor : FoundActors)
	{
		SpawnZones.Add(Cast<AItemSpawnZone>(FoundActor));
	}
}

void AItemManager::UntrackItem(ABaseItem* Item)
{
	ItemsInWorld.Remove(Item);
}

