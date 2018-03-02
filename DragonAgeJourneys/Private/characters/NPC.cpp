#include "NPC.h"
#include "GameCharacter.h"

bool IsValidNPCId(FString sID)
{
	return Names.Contains(sID);
}

UGameCharacter* GetTutorialAlly(int32 nPartyIndex)
{
	//TODO GetTutorialAlly
	return NewObject<UGameCharacter>();
}
