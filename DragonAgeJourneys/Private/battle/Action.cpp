#include "Action.h"
#include "DragonAgeJourneys.h"
#include "Skill.h"
#include "GameCharacter.h"

UAction::UAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UAction::InitializeAction(int32 nActionType, UGameCharacter* gameChar)
{
	gameCharacter = gameChar;
	actionType = nActionType;
}

UAction::~UAction()
{
}
