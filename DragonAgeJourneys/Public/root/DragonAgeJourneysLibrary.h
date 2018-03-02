// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DragonAgeJourneysSingleton.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DragonAgeJourneysLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONAGEJOURNEYS_API UDragonAgeJourneysLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UDragonAgeJourneysLibrary(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = Singleton)
		static UDragonAgeJourneysSingleton* GetDragonAgeJourneysData(bool& IsValid);
};
