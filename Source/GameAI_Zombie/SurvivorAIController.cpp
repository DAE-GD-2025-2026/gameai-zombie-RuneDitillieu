// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorAIController.h"
#include "Components/GameFrameworkComponentManager.h"

// Sets default values
ASurvivorAIController::ASurvivorAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASurvivorAIController::BeginPlay()
{
	Super::BeginPlay();
	UGameFrameworkComponentManager* ComponentManager = GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>();
	ComponentManager->AddReceiver(this);
}

// Called every frame
void ASurvivorAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

