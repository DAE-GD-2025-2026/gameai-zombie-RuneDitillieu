// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGameInstance.h"

#include "ZombieGameMode.h"
#include "Interfaces/IPluginManager.h"


void UZombieGameInstance::Init()
{
	Super::Init();
	
	GameFeatures = GetGameFeaturesPlugins();

	if (GameFeatures.Num() == 1)
	{
		LoadStudentPlugin(GameFeatures[0]->GetName());
	}
}

void UZombieGameInstance::Shutdown()
{
	auto& GFS = UGameFeaturesSubsystem::Get();
	for (auto Plugin : GameFeatures)
	{
		GFS.UnloadGameFeaturePlugin(GetGameFeatureURL(Plugin), true);
	}
	
	Super::Shutdown();
}

TArray<FString> UZombieGameInstance::GetGameFeatureNames() const
{
	TArray<FString> GameFeatureNames{};
	for (auto const & GameFeature : GameFeatures)
	{
		GameFeatureNames.Add(GameFeature->GetName());
	}
	return GameFeatureNames;
}

TArray<TSharedRef<IPlugin>> UZombieGameInstance::GetGameFeaturesPlugins() const
{
	auto& PluginManager = IPluginManager::Get();
	auto& GFS = UGameFeaturesSubsystem::Get();
	
	TArray<TSharedRef<IPlugin>> FoundGameFeatures{};
	for (auto Plugin : PluginManager.GetEnabledPlugins())
	{
		if (Plugin->GetType() == EPluginType::Project)
		{
			if (FGameFeaturePluginDetails OutDetails{}; 
				GFS.GetBuiltInGameFeaturePluginDetails(Plugin, OutDetails))
			{
				FoundGameFeatures.Add(Plugin);
			}
		}
	}
	return FoundGameFeatures;
}

void UZombieGameInstance::LoadStudentPlugin(FString const & PluginName)
{
	UGameFeaturesSubsystem& GFS = UGameFeaturesSubsystem::Get();
	
	// Load and activate the plugin
	UE_LOG(LogTemp, Log, TEXT("Loading plugin: %s"), *GetGameFeatureURL(PluginName));
	auto OnComplete = FGameFeaturePluginLoadComplete::CreateUObject(this, 
		&UZombieGameInstance::OnGameFeatureLoadComplete);
	GFS.LoadAndActivateGameFeaturePlugin(GetGameFeatureURL(PluginName), OnComplete);
}

void UZombieGameInstance::UnloadStudentPlugin(FString const& PluginName)
{
	UGameFeaturesSubsystem& GFS = UGameFeaturesSubsystem::Get();
	
	GFS.UnloadGameFeaturePlugin(GetGameFeatureURL(PluginName), true);
}

FString UZombieGameInstance::GetGameFeatureURL(TSharedRef<IPlugin> const Plugin) const
{
	return GetGameFeatureURL(Plugin->GetName());
}

FString UZombieGameInstance::GetGameFeatureURL(FString const & PluginName) const
{
	return FString::Printf(TEXT("file:%s.uplugin"), *PluginName);
}

void UZombieGameInstance::OnGameFeatureLoadComplete(const UE::GameFeatures::FResult& LoadResult)
{
	if (!LoadResult.HasError())
	{
		UE_LOG(LogTemp, Log, TEXT("Student plugin loaded successfully"));
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, 
	FString::Printf(TEXT("Student plugin loaded successfully")));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load student plugin! Result: %s"), *LoadResult.GetError());
		GEngine->AddOnScreenDebugMessage(1, 50.f, FColor::Red, 
			FString::Printf(TEXT("Failed to load student plugin! Result: %s"), *LoadResult.GetError()));
	}
}
