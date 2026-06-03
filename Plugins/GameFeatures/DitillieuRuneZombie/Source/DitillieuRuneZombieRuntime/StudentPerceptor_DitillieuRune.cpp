// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor_DitillieuRune.h"


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
}
