// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DragonAgeJourneysGameMode.h"
#include "DragonAgeJourneysPlayerController.h"
#include "DragonAgeJourneysCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "DragonAgeJourneysLibrary.h"
#include "DragonAgeJourneysSingleton.h"

ADragonAgeJourneysGameMode::ADragonAgeJourneysGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADragonAgeJourneysPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Data/Blueprints/BPAvatar"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

//reset singleton when PIE ends
void ADragonAgeJourneysGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::EndPlayInEditor)
	{
		bool IsValid;
		UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysCurrentWorld = nullptr;
		UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysMain = nullptr;
		UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysParty = nullptr;
	}
}
