// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnZone.generated.h"

UCLASS()
class GAMEAI_ZOMBIE_API AItemSpawnZone final : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemSpawnZone();
	
	ABaseItem* SpawnItemOfType(TSubclassOf<ABaseItem> const & ItemType) const
	{
		FActorSpawnParameters SpawnParams{};
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FVector const ZoneDelta{FMath::FRandRange(-Extents.X, Extents.X), FMath::FRandRange(-Extents.Y, Extents.Y), 0.0f};
		FVector const SpawnLocation{GetActorLocation() + ZoneDelta};
		return Cast<ABaseItem>(GetWorld()->SpawnActor(ItemType, &SpawnLocation, &FRotator::ZeroRotator, SpawnParams));
	}
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnZone")
	FVector Extents{100, 100, 10};
};
