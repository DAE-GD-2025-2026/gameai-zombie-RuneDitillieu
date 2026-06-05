// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_S_CheckHealth_DitillieuRune.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Common/HealthComponent.h"
#include "Common/StaminaComponent.h"

UBT_S_CheckHealth_DitillieuRune::UBT_S_CheckHealth_DitillieuRune()
{
	bNotifyTick = true;
}

void UBT_S_CheckHealth_DitillieuRune::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (BlackBoard == nullptr)
	{
		AAIController* AIController{ OwnerComp.GetAIOwner() };
		if (!AIController) return;
		ASurvivorPawn* Survivor{ Cast<ASurvivorPawn>(AIController->GetPawn()) };
		if (!Survivor) return;
		BlackBoard = AIController->GetBlackboardComponent();
		if (!BlackBoard) return;
		HealthComponent = Survivor->GetComponentByClass<UHealthComponent>();
		if (!HealthComponent) return;
		StaminaComponent = Survivor->GetComponentByClass<UStaminaComponent>();
		if (!StaminaComponent) return;
	}
	
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, 
	FString::Printf(TEXT("check")));
	
	if (HealthComponent->GetHealth() < 5 || StaminaComponent->GetCurrentStamina() < 4.5f)
	{
		BlackBoard->SetValueAsBool(FName("IsOnLifeSupport"), true);
	}
}