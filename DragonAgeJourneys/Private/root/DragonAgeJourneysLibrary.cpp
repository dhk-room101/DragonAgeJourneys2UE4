// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonAgeJourneysLibrary.h"
#include <Engine.h>

UDragonAgeJourneysLibrary::UDragonAgeJourneysLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

//get singleton
UDragonAgeJourneysSingleton* UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(bool& IsValid)
{
	IsValid = false;
	UDragonAgeJourneysSingleton* DataInstance = Cast<UDragonAgeJourneysSingleton>(GEngine->GameSingleton);

	if (!DataInstance) return NULL;
	if (!DataInstance->IsValidLowLevel()) return NULL;

	IsValid = true;
	return DataInstance;
}



