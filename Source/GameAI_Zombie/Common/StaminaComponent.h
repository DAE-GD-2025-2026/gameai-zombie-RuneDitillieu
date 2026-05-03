// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


class ASurvivorPawn;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEAI_ZOMBIE_API UStaminaComponent final : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStaminaComponent();
	
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	void AddStamina(float Amount);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Stamina")
	float GetCurrentStamina() const {return Stamina;}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Stamina")
	float GetMaxStamina() const {return MaxStamina;}
	

protected:
	UPROPERTY(EditDefaultsOnly, Category="Stamina")
	float MaxStamina{10};
	float Stamina{MaxStamina};
	
	UPROPERTY(EditDefaultsOnly, Category="Stamina")
	float DefaultStaminaDrainPerSecond{0.1f};
	UPROPERTY(EditDefaultsOnly, Category="Stamina")
	float RunningStaminaDrainPerSecond{0.5f};
	
	UPROPERTY()
	TObjectPtr<ASurvivorPawn> SurvivorOwner{};
};
