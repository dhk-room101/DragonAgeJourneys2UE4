// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StreamableManager.h"
#include "DragonAgeJourneysSingleton.generated.h"

class UEmbeddedAssets;
class UParty;
class UDragonMain;
class UWorld;

UCLASS(Blueprintable, BlueprintType)
class UDragonAgeJourneysSingleton : public UObject
{
	GENERATED_BODY()

	UDragonAgeJourneysSingleton(const FObjectInitializer& ObjectInitializer);
	
public:

	UPROPERTY()
		UWorld* DragonAgeJourneysCurrentWorld = nullptr;
	UPROPERTY()
		UDragonMain* DragonAgeJourneysMain = nullptr;
	UPROPERTY()
		UParty* DragonAgeJourneysParty = nullptr;
	UPROPERTY()
		UEmbeddedAssets* ASSETS = nullptr;
		
	FStreamableManager AssetLoader;
};
