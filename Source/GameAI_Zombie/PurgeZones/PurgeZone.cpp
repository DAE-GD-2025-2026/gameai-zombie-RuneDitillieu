// Fill out your copyright notice in the Description page of Project Settings.


#include "PurgeZone.h"

APurgeZone::APurgeZone()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APurgeZone::BeginPlay()
{
	Super::BeginPlay();
}

void APurgeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimePassed += DeltaTime;
}

