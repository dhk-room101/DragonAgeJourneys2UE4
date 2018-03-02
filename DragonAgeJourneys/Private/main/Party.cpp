#include "Party.h"
#include "Quest.h"
#include "Group.h"
#include "GameCharacter.h"

UParty::UParty(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

int32 UParty::GetLevel()
{
	int32 nLevel = 1;
	for (int32 i = 0; i < members.Num(); i++)
	{
		if (members[i] != nullptr)
			nLevel = FMath::Max(nLevel, members[i]->level);
	}
	return nLevel;
}

int32 UParty::GetSneaking()
{
	return _sneaking;
}

int32 UParty::GetDifficulty()
{
	return _difficulty;
}

bool UParty::HasQuest(int32 nQuestId, bool bRepeat)
{
	for (int32 i = 0; i < quests.Num(); i++)
	{
		UQuest* quest = quests[i];
		if (quest->questId == nQuestId && (!quest->completed || bRepeat))
		{
			return true;
		}
	}
	return false;
}

UGameCharacter* UParty::GetCompanionByClass(int32 nClassId, bool bMonster /*= true*/)
{
	if (members.Num() > 1)
	{
		for (int32 i = 0; i < members.Num(); i++)
		{
			if (!bMonster && i > 0) /*//TODO doublecheck (!(!bMonster && i == 0))*/
			{
				UGameCharacter* gameCharacter = members[i];
				if (gameCharacter->classId == nClassId)
				{
					return gameCharacter;
				}
			}
		}
	}

	return nullptr;
}

UGameCharacter* UParty::GetHeroCharacter()
{
	return members[0];
}

UGroup* UParty::GetGroup()
{
	UGroup* group = NewObject<UGroup>();
	group->player = true;

	for (int32 i = 0; i < members.Num(); i++)
	{
		group->chars.Add(members[i]);
	}

	return group;
}

bool UParty::HasFinishedQuest(int32 nQuestId)
{
	for (int32 i = 0; i < quests.Num(); i++)
	{
		UQuest* quest = quests[i];
		if (quest->questId == nQuestId && quest->completed)
		{
			return true;
		}
	}
	return false;
}

bool UParty::AddToInventory(UItem* item)
{
	//TODO AddToInventory
	return false;
}

bool UParty::RemoveFromInventory(UItem* item)
{
	//TODO RemoveFromInventory
	return false;
}

UParty::~UParty()
{
}
