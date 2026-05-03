// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PurgeZone.generated.h"

// Stub, most of the implementation is in BP
UCLASS()
class GAMEAI_ZOMBIE_API APurgeZone : public AActor
{
	GENERATED_BODY()

public:
	friend class AZombieGameMode;
	
	APurgeZone();
	virtual void Tick(float DeltaTime) override;
	
	void SetDiameter(float NewDiameter){Diameter = NewDiameter;}
	void SetTimeTillPurge(float NewDiameter){Diameter = NewDiameter;}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PurgeZone")
	float Diameter{100};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PurgeZone")
	float TimeTillPurge{5};
	UPROPERTY(BlueprintReadOnly, Category="PurgeZone")
	float TimePassed{0};
	
	virtual void BeginPlay() override;
};
