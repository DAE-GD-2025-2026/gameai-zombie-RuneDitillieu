// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_LookAround_DitillieuRune.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UBT_T_LookAround_DitillieuRune::UBT_T_LookAround_DitillieuRune()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBT_T_LookAround_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
	// grab survivor
	AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	BlackBoard = AIController->GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;
	
	DegreesRotated = 0.f;
	return EBTNodeResult::InProgress;
}

void UBT_T_LookAround_DitillieuRune::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	float DegreesToRotate{ DegreesPerSec * DeltaSeconds };
	Survivor->AddActorWorldRotation(FRotator(0, DegreesToRotate, 0));
	DegreesRotated += DegreesToRotate;
	
	if (DegreesRotated >= 360.f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}