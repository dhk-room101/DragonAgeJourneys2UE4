// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "DragonAgeJourneysLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONAGEJOURNEYS_API ADragonAgeJourneysLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ADragonAgeJourneysLevelScriptActor();
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

};
