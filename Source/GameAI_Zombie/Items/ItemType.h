// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Food = 0 UMETA(DisplayName = "Food"),
	Medkit = 1 UMETA(DisplayName = "Medkit"),
	Shotgun = 2 UMETA(DisplayName = "Shotgun"),
	Pistol = 3 UMETA(DisplayName = "Pistol"),
	Garbage = 4 UMETA(DisplayName = "Garbage")
};
