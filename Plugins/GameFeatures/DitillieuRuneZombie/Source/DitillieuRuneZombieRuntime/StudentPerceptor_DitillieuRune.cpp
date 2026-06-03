// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor_DitillieuRune.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "PurgeZones/PurgeZone.h"
#include "Items/Food.h"
#include "Items/Medkit.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
#include "Zombies/BaseZombie.h"
#include "Village/House/House.h"

UStudentPerceptor_DitillieuRune::UStudentPerceptor_DitillieuRune()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor_DitillieuRune::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor_DitillieuRune::OnPerceptionUpdated);
	}
}

void UStudentPerceptor_DitillieuRune::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw Something!")));
	
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;
	
	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	if (!AIController) return;
	
	UBlackboardComponent* blackBoard = AIController->GetBlackboardComponent();
	if (!blackBoard) return;
	
	
	FAISenseID DamageSenseID = UAISense::GetSenseID<UAISense_Damage>();
	if (Stimulus.WasSuccessfullySensed() && Stimulus.Type == DamageSenseID)
	{
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Red, 
FString::Printf(TEXT("Sensing Damage")));
	}
	
	if (Stimulus.WasSuccessfullySensed())												
	{
		if (APurgeZone * sensedPurgeZone = Cast<APurgeZone>(Actor))
		{
			blackBoard->SetValueAsBool(FName("SensedDanger"), true);
			blackBoard->SetValueAsObject(FName("ClosestPurgeZone"),sensedPurgeZone);
		}
		
		if (ABaseZombie* SensedZombie = Cast<ABaseZombie>(Actor))
		{
			blackBoard->SetValueAsBool(FName("SensedDanger"), true);
			
			ABaseZombie* zombie1 = Cast<ABaseZombie>(blackBoard->GetValueAsObject(FName(("ClosestZombie1"))));
			ABaseZombie* zombie2 = Cast<ABaseZombie>(blackBoard->GetValueAsObject(FName(("ClosestZombie2"))));
			ABaseZombie* zombie3 = Cast<ABaseZombie>(blackBoard->GetValueAsObject(FName(("ClosestZombie3"))));
			
			// if not already aware of this zombie
			if (SensedZombie != zombie1 && SensedZombie != zombie2 && SensedZombie != zombie3)
			{
				int amZombies{ blackBoard->GetValueAsInt(FName("AmountOfZombiesTracked")) };
				if (amZombies < 3)
				{
					blackBoard->SetValueAsInt(FName("AmountOfZombiesTracked"), amZombies + 1);
				}
				
				// if not already aware of 3 zombies, add this one
				if (zombie1 == nullptr)
				{
					blackBoard->SetValueAsObject(FName("ClosestZombie1"), SensedZombie);
				}
				else if (zombie2 == nullptr)
				{
					blackBoard->SetValueAsObject(FName("ClosestZombie2"), SensedZombie);
				}
				else if (zombie3 == nullptr)
				{
					blackBoard->SetValueAsObject(FName("ClosestZombie3"), SensedZombie);
				}
				else
				{
					// replace furthest zombie with this one
					float dist{ float((OwnerPawn->GetActorLocation() - Actor->GetActorLocation()).SquaredLength()) };
					float dist1{ float((OwnerPawn->GetActorLocation() - zombie1->GetActorLocation()).SquaredLength()) };
					float dist2{ float((OwnerPawn->GetActorLocation() - zombie2->GetActorLocation()).SquaredLength()) };
					float dist3{ float((OwnerPawn->GetActorLocation() - zombie3->GetActorLocation()).SquaredLength()) };
					
					if (dist < dist1 || dist < dist2 || dist < dist3)
					{
						if (dist1 > dist2 && dist1 > dist3)
						{
							blackBoard->SetValueAsObject(FName("ClosestZombie1"), SensedZombie);
						}
						else if (dist2 > dist1 && dist2 > dist3)
						{
							blackBoard->SetValueAsObject(FName("ClosestZombie2"), SensedZombie);
						}
						else if (dist3 > dist1 && dist3 > dist2)
						{
							blackBoard->SetValueAsObject(FName("ClosestZombie3"), SensedZombie);
						}
					}
				}
			}
		}
	
		
		if (AFood* SensedFood = Cast<AFood>(Actor))
		{
			blackBoard->SetValueAsBool(FName("SensedItem"), true);
			blackBoard->SetValueAsObject(FName("ClosestFood"), SensedFood);
		}
		
		if (AMedkit* SensedMedkit = Cast<AMedkit>(Actor))
		{
			blackBoard->SetValueAsBool(FName("SensedItem"), true);
			blackBoard->SetValueAsObject(FName("ClosestMedkit"), SensedMedkit);
		}
		
		if (APistol* SensedPistol = Cast<APistol>(Actor))
		{
			blackBoard->SetValueAsBool(FName("SensedItem"), true);
			blackBoard->SetValueAsObject(FName("ClosestPistol"), SensedPistol);
		}
		
		if (AShotgun* SensedShotGun = Cast<AShotgun>(Actor))
		{
			blackBoard->SetValueAsBool(FName("SensedItem"), true);
			blackBoard->SetValueAsObject(FName("ClosestShotgun"), SensedShotGun);
		}
		
		if (AHouse* SensedHouse = Cast<AHouse>(Actor)) 
		{
			blackBoard->SetValueAsBool(FName("SensedVillage"), true);
			blackBoard->SetValueAsObject(FName("ClosestHouse"), SensedHouse);
		}
	}
}
