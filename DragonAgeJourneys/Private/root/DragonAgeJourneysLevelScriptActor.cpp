// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonAgeJourneysLevelScriptActor.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "Map.h"
#include "Battlefield.h"

ADragonAgeJourneysLevelScriptActor::ADragonAgeJourneysLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADragonAgeJourneysLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("Level BeginPlay"));
#endif // DEBUG

}

void ADragonAgeJourneysLevelScriptActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetMain()->LevelMap && !GetMain()->Battlefield)
	{
		GetMain()->LevelMap->DeltaSeconds = DeltaSeconds;
		GetMain()->LevelMap->MapTick();
	}
	
	if (GetMain()->Battlefield)
	{
		GetMain()->Battlefield->DeltaSeconds = DeltaSeconds;
		GetMain()->Battlefield->BattlefieldTick();
	}
}