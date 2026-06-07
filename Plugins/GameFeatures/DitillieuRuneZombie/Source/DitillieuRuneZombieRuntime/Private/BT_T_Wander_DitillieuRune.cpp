// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_Wander_DitillieuRune.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Survivor/SurvivorPawn.h"

EBTNodeResult::Type UBT_T_Wander_DitillieuRune::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* TaskMemory)
{
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!survivor) return EBTNodeResult::Failed;
	
	const FVector2D CenterWanderCircle{ survivor->GetActorLocation() + survivor->GetVelocity().GetSafeNormal() * m_OffsetDistance };
	const float AngleRad{ ((rand() % (m_MaxAngleChange * 2)) - m_MaxAngleChange) / 180.f * PI };
	m_WanderAngle += AngleRad;

	const FVector WanderPos{ CenterWanderCircle.X + cos(m_WanderAngle) * m_Radius, 
						 CenterWanderCircle.Y + sin(m_WanderAngle) * m_Radius, 0 };
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()); //make sure not in wall
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation ProjectedLocation;
	bool bFoundValidSpot = NavSys->ProjectPointToNavigation(WanderPos, ProjectedLocation, FVector(200.f, 200.f, 200.f));
	survivor->StopRunning();

	if (bFoundValidSpot)
	{
		EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(ProjectedLocation.Location, 50.0f, false, true, true, true, 0, true);
        
		if (MoveResult != EPathFollowingRequestResult::Type::Failed)
		{
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}