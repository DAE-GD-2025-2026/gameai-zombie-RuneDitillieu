// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ItemSpawnZone.h"
#include "Components/SceneComponent.h"
#include "ItemManager.generated.h"


UCLASS()
class GAMEAI_ZOMBIE_API AItemManager final : public AActor
{
	GENERATED_BODY()

public:
	AItemManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	void RegisterHouses();
	void UntrackItem(ABaseItem* Item);
	
protected:
	UPROPERTY()
	TArray<ABaseItem*> ItemsInWorld{};
	UPROPERTY()
	TArray<AItemSpawnZone*> SpawnZones{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<ABaseItem>> TypesToSpawn{};
	int MaxItemsInWorld{30};
};
