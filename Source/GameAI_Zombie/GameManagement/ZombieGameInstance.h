// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFeaturesSubsystem.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ZombieGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEAI_ZOMBIE_API UZombieGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetGameFeatureNames() const;
	
	UFUNCTION(BlueprintCallable)
	void LoadStudentPlugin(FString const & PluginName);
	UFUNCTION(BlueprintCallable)
	void UnloadStudentPlugin(FString const & PluginName);
	
	// Helpers
	FString GetGameFeatureURL(TSharedRef<IPlugin> const Plugin) const;
	FString GetGameFeatureURL(FString const & PluginName) const;
	
private:
	TArray<TSharedRef<IPlugin>> GameFeatures{};
	
	TArray<TSharedRef<IPlugin>> GetGameFeaturesPlugins() const;
	void OnGameFeatureLoadComplete(const UE::GameFeatures::FResult& LoadResult);
};
