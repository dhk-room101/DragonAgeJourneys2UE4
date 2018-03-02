#include "AiAction.h"
#include "DragonAgeJourneys.h"
#include "Skill.h"
#include "GameCharacter.h"
#include "Hex.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "Battlefield.h"
#include "DragonAgeJourneysCharacter.h"
#include "Intelligence.h"
#include "Effect.h"
#include "Party.h"

UAiAction::UAiAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UAiAction::InitializeAiAction(UGameCharacter* gameChar)
{
	owner = gameChar;
}

int32 UAiAction::GetActionValue() const
{
	int32 nYDistance = 0;
	int32 loc9 = 0;
	int32 nIndex = 0;
	int32 nDamage = 0;
	int32 nValue = 0;
	UBattlefield* battlefield = GetMain()->Battlefield;
	AHex* hexFrom = owner->avatar->hex;
	AHex* hexTo = moveToHex;

	if (!hexTo)
	{
		hexTo = hexFrom;
	}

	TArray<ADragonAgeJourneysCharacter*> loc5 = hexTo->GetNeighbourAvatars();
	int32 loc6 = GetMain()->Battlefield->GetRawDistanceBetweenHexes(hexFrom, hexTo);
	int32 loc7 = GetRightmostEnemyPosition();

	if (owner->ai->stance == STANCE_DEFENSIVE || owner->ai->stance == STANCE_RANGED)
	{
		if (hexTo->gridX < hexFrom->gridX)
		{
			nValue = nValue - loc6 * 2;
		}
		else if (hexTo->gridX > hexFrom->gridX)
		{
			nValue = nValue + loc6 * 2;
		}
	}
	else if (owner->ai->stance == STANCE_AGGRESSIVE)
	{
		if (targetCharacter == nullptr)
		{
			nValue = nValue + loc6;
		}
	}

	if (battlefield->TrapOnHex(hexTo))
	{
		nValue = nValue - 10;
	}

	if (!skill)
	{
		return nValue;
	}

	if (skill->aiNotWithMove && hexTo != hexFrom)
	{
		return -999;
	}

	if (targetHex != nullptr)
	{
		nYDistance = FMath::Abs(targetHex->gridY - hexTo->gridY);
		loc9 = 0;

		if (nYDistance == 0)
		{
			loc9 = 3;
		}

		if (nYDistance == 1)
		{
			loc9 = 2;
		}

		if (nYDistance == 2)
		{
			loc9 = 1;
		}

		if (skill->rangedAttack)
		{
			if (!loc9)
			{
				loc9 = -4;
			}
			else
			{
				loc9 = loc9 * 2;
			}
		}

		nValue = nValue + loc9;
	}

	nValue = nValue + skill->aiPriority * 10;

	if (skill->aiTargetOnlyLowHealth && targetCharacter && targetCharacter->health > 40)
	{
		return -50;
	}

	if (skill->aiAlwaysUse)
	{
		return 9999;
	}

	if (skill->aiDesireAllyProximity)
	{
		nValue = nValue + loc5.Num() * 10;
	}

	if (skill->spawnsTrap)
	{
		if (loc7 > targetHex->gridX)
		{
			nValue = nValue - 30;
		}
		else
		{
			//TODO traps
			/*for (int32 i = 0; i < loc2->traps.Num(); i++)
			{
				UTrap* loc10 = loc2->traps[i];
				if (!(loc10 && loc10.active && loc10.hex.gridY == targetHex->gridY))
				{
					continue;
				}
				nValue = nValue - 10;
			}*/

			nValue = nValue + FMath::Max(0, 15 - targetHex->gridX);
			if (DoesEnemyHaveThisVerticalPosition(targetHex->gridY))
			{
				nValue = nValue + 5;
			}

			nIndex = 3;

			while (nIndex <= 5)
			{
				AHex* loc12 = targetHex->neighbours[nIndex];
				if (loc12 != nullptr && loc12->avatar && loc12->avatar->GetCharRef()->IsMonster())
				{
					nValue = nValue + 5;
				}

				++nIndex;
			}
		}
	}

	if (targetCharacter)
	{
		if (targetCharacter->hateRanking != 1)
		{
			nValue = nValue + 10;
		}
		else
		{
			nValue = nValue + 25;
		}

		nDamage = skill->GetDamage();

		if (skill->critsAgainstTargetsWithEffectId)
		{
			for (int32 i = 0; i < targetCharacter->effects.Num(); i++)
			{
				UEffect* loc14 = targetCharacter->effects[i];
				if (loc14->effectId != skill->critsAgainstTargetsWithEffectId)
				{
					nValue = nValue + 50;
					break;
				}
			}
		}

		if (nDamage)
		{
			nValue = nValue + 15;
			if (nDamage >= targetCharacter->health)
			{
				nValue = nValue + 25;
			}
			if (battlefield->IsFlankAttack(owner, targetCharacter, skill, hexTo))
			{
				nValue = nValue + 15;
			}
			if (battlefield->IsBackstab(owner, targetCharacter, skill))
			{
				nValue = nValue + 50;
			}
		}
	}

	if (skill->targetEffectId)
	{
		if (targetCharacter->HasEffectWithId(skill->targetEffectId))
		{
			nValue = nValue - 15;
		}
	}

	nValue = nValue + FMath::RandRange(0, 6);

	if (owner->power - skill->powerCost < owner->GetPowerMax() * 0.2)
	{
		nValue = nValue - 10;
	}

	return nValue;
}

UAiAction::~UAiAction()
{
}

int32 UAiAction::GetRightmostEnemyPosition() const
{
	int32 nPos = 0;

	for (int32 i = 0; i < GetParty()->members.Num(); i++)
	{
		UGameCharacter* member = GetParty()->members[i];
		if (member->avatar)
		{
			nPos = FMath::Max(nPos, member->avatar->battlefieldX);
		}
	}

	return nPos;
}

bool UAiAction::DoesEnemyHaveThisVerticalPosition(int32 nGridY) const
{
	for (int32 i = 0; i < GetParty()->members.Num(); i++)
	{
		UGameCharacter* member = GetParty()->members[i];
		if (member->avatar && member->avatar->battlefieldY == nGridY)
		{
			return true;
		}
	}

	return false;
}
