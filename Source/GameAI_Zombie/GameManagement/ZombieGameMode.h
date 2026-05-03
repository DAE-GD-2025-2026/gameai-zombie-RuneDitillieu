// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFeaturesSubsystem.h"
#include "GameFeaturePluginOperationResult.h" 	

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PurgeZones/PurgeZone.h"
#include "Survivor/SurvivorPawn.h"
#include "Village/House/House.h"
#include "Zombies/BaseZombie.h"
#include "ZombieGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAMEAI_ZOMBIE_API AZombieGameMode final : public AGameModeBase
{
	GENERATED_BODY()
	
	public:
	AZombieGameMode();
	
	virtual void BeginPlay() override;
	
	protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Worldgen")
	TArray<TSubclassOf<AHouse>> HouseTemplates{};
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Worldgen")
	float VillageSpawnRadius{3000.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Worldgen")
	float MinVillageSpacing{3000.f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Worldgen")
	int MinVillagesSpawned{3};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Worldgen")
	int MaxVillagesSpawned{5};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Worldgen")
	int MinHousesPerVillage{2};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Worldgen")
	int MaxHousesPerVillage{4};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Worldgen")
	float InitialHouseOffSetRadius{300.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Worldgen")
	float HouseSeparation{150.f};
	
	// Gameplay
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Zombies")
	TArray<ABaseZombie*> Zombies{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Zombies")
	TArray<TSubclassOf<ABaseZombie>> ZombieTypes{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Zombies")
	float MinZombieSpawnInterval{5.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Zombies")
	float MaxZombieSpawnInterval{10.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Zombies")
	float SpawnIntervalLerpDuration{120.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Zombies")
	float TimeSinceLastSpawn{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Zombies")
	float ZombieSpawnRadius{1000.0f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	TSubclassOf<APurgeZone> PurgeClass{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float PurgeGracePeriod{60.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float MinPurgeDuration{5.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float MaxPurgeDuration{5.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float MinPurgeDiameter{200.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float MaxPurgeDiameter{1000.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float MinPurgeSpawnDistance{100.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float MaxPurgeSpawnDistance{600.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float MinPurgeInterval{5.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float MaxPurgeInterval{15.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float PurgeLerpDuration{240.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay|Purges")
	float TimeSinceLastPurgeSpawn{};
	
	UPROPERTY(BlueprintreadWrite, EditDefaultsOnly, Category="Gameplay")
	ASurvivorPawn* Survivor{};

	virtual void Tick(float DeltaTime) override;
	
	private:
	void SpawnVillages() const;
	TArray<AActor*> SpawnVillageAt(FVector const & Location) const;
	void SpreadHouses(TArray<AActor*> const & Houses) const;
	bool IsVillageSpacingOk(FVector const & ProposedLocation, TArray<FVector> const & ExistingVillages) const;
	
	void SpawnZombieAround(const FVector& SurvivorLocation);
	float GetCurrentZombieSpawnInterval() const;
	
	void SpawnPurgeAround(const FVector& SurvivorLocation);
	float GetCurrentPurgeAlpha() const;
	bool ShouldPurgeZonesSpawn() const;
};
