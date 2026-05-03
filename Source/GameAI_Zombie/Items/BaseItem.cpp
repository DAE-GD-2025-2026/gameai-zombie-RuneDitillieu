// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


// Sets default values
ABaseItem::ABaseItem()
{
	auto AIStimuli = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AIPerceptionStimuliSource");
	AddOwnedComponent(AIStimuli);
	AIStimuli->RegisterForSense(UAISense_Sight::StaticClass());
	AIStimuli->RegisterWithPerceptionSystem();
}

void ABaseItem::UseItem(ASurvivorPawn& Survivor)
{
}

