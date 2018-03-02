#include "Intelligence.h"
#include "GameCharacter.h"
#include "Classes.h"
#include "Effect.h"
#include "DragonAgeJourneysCharacter.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "Battlefield.h"
#include "Hex.h"
#include "AiAction.h"
#include "Skill.h"
#include "GameCharacter.h"
#include "Algo/Reverse.h"

UIntelligence::UIntelligence(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UIntelligence::InitializeIntelligence(UGameCharacter* gameCharacter, int32 nStance /* = 0*/)
{
	charRef = gameCharacter;
	if (nStance > 0)
	{
		stance = nStance;
	}
	else
	{
		switch (charRef->classId)
		{
		case CLASS_GENLOCK_ARCHER:
		case CLASS_HURLOCK_ARCHER:
		case CLASS_GENLOCK_ARCHER_RUNT:
		case CLASS_HURLOCK_ARCHER_RUNT:
		{
			stance = STANCE_RANGED;
			break;
		}
		default:
		{
			stance = STANCE_AGGRESSIVE;
			break;
		}
		}
	}
}

void UIntelligence::TookDamageFrom(int32 nAmount, UGameCharacter* charAttacker)
{
	if (nAmount <= 0)
	{
		return;
	}

	latestAttacker = charAttacker;

	int32 nIndex = 0;
	while (nIndex < damageSources.Num())
	{
		if (damageSources[nIndex].charAttacker == charAttacker)
		{
			damageSources[nIndex].nAmount = damageSources[nIndex].nAmount + nAmount;
			return;
		}
		++nIndex;
	}

	FDamageSource damageSource;
	damageSource.charAttacker = charAttacker;
	damageSource.nAmount = nAmount;
	damageSources.Add(damageSource);
}

void UIntelligence::SkillActionsUntargeted(USkill* skill)
{
	AHex* hex = charRef->avatar->hex;

	if (!skill->IsUsable())
	{
		UE_LOG(LogTemp, Warning, TEXT("skill not usable"));
		return;
	}

	int32 nMovesLeft = charRef->GetMovesLeft();
	TArray<AHex*> arHexRange = GetMain()->Battlefield->GetHexesInRange(nMovesLeft);
	arHexRange.Add(hex);

	for (int32 i = 0; i < arHexRange.Num(); i++)
	{
		AHex* hexRange = arHexRange[i];
		AHex* hexNeighbour = nullptr;

		if (!hexRange->CanEndMoveHere(charRef))
		{
			continue;
		}
		if (skill->spawnsTrap)
		{
			hexNeighbour = hexRange->neighbours[4];
			if (!hexNeighbour || !hexNeighbour->CanEndMoveHere(charRef) || GetMain()->Battlefield->TrapOnHex(hexNeighbour) != nullptr)
			{
				continue;
			}
		}

		UAiAction* aiAction = NewObject<UAiAction>();
		aiAction->InitializeAiAction(charRef);

		if (hexNeighbour)
		{
			aiAction->targetHex = hexNeighbour;
		}
		else
		{
			aiAction->targetHex = hexRange;
		}

		aiAction->moveToHex = hexRange;
		aiAction->skill = skill;

		possibleActions.Add(aiAction);
	}
}

void UIntelligence::SkillActionsForTarget(UGameCharacter* gameCharacter, USkill* skill)
{
	bool loc10 = false;
	bool loc12 = false;
	UBattlefield* loc1 = GetMain()->Battlefield;
	AHex* loc2 = charRef->avatar->hex;

	UE_LOG(LogTemp, Warning, TEXT("building possible skill actions for skill %s on target %s"), *skill->skillname, *gameCharacter->charName);

	if (!skill->IsUsable())
	{
		UE_LOG(LogTemp, Warning, TEXT("skill not usable"));
		return;
	}

	bool loc3 = false;
	TArray<AHex*> loc4;
	int32 loc5 = MaxRangeOfSkill(skill);
	AHex* loc6 = gameCharacter->avatar->hex;
	float loc7 = loc1->GetHexDistanceOccupied(loc6);

	TArray<AHex*> loc8 = loc1->GetHexesInMoveRange();
	loc8.Add(loc2);
	for (int32 i = 0; i < loc8.Num(); i++)
	{
		AHex* loc9 = loc8[i];
		loc10 = true;

		if (!loc9->CanEndMoveHere(charRef))
		{
			continue;
		}

		if (!ValidHexToUseSkillOn(loc9, skill, loc6))
		{
			continue;
		}

		UAiAction* loc11 = NewObject<UAiAction>();
		loc11->InitializeAiAction(charRef);

		if (skill->specialMovementSkill)
		{
			loc12 = false;
			int32 loc13 = 0;

			while (loc13 < 3)
			{
				AHex* loc14;
				if (loc2->GetActorLocation().X > loc6->GetActorLocation().X)
				{
					loc14 = loc6->neighbours[loc13];
				}
				else
				{
					loc14 = loc6->neighbours[loc13 + 3];
				}

				if (loc14 != nullptr)
				{
					if (loc14->CanEndMoveHere(charRef))
					{
						loc12 = true;
						loc4.Add(loc14);
					}
				}
				++loc13;
			}

			if (!loc12)
			{
				loc10 = false;
				break;
			}
			else
			{
				loc11->specialHex = loc4[FMath::RandRange(0, loc4.Num() - 1)];
			}
		}

		if (!loc10)
		{
			continue;
		}

		loc11->targetHex = loc6;
		loc11->moveToHex = loc9;
		loc11->distance = loc7;
		loc11->targetCharacter = gameCharacter;
		loc11->skill = skill;

		possibleActions.Add(loc11);
	}
}

void UIntelligence::MoveActionsTowards(AHex* hex)
{
	/*int32 loc6 = 0;
	TArray<AHex*> loc2;
	TArray<AHex*> loc1 = GetMain()->Battlefield->GetHexesInMoveRange();
	int32 loc3 = 999;

	for (int32 i = 0; i < loc1.Num(); i++)
	{
		AHex* loc4 = loc1[i];
		if (loc4->CanEndMoveHere(charRef))
		{
			loc6 = GetMain()->Battlefield->GetRawDistanceBetweenHexes(loc4, hex);
		}
		if (loc6 < loc3)
		{
			loc2.Empty();
			loc2.Add(loc4);
			loc3 = loc6;
			continue;
		}
		if (loc6 == loc3)
		{
			loc2.Add(loc4);
		}
	}

	for (int32 i = 0; i < loc2.Num(); i++)
	{
		AHex* loc5 = loc2[i];
		if (loc5 != nullptr)
		{
			UAiAction* loc7 = NewObject<UAiAction>();
			loc7->InitializeAiAction(charRef);
			loc7->moveToHex = loc5;
			possibleActions.Add(loc7);
		}
	}*/

	TArray<AHex*> loc1 = GetMain()->Battlefield->GetHexesInMoveRange();
	TArray<AHex*> loc2;
	
	int32 loc3 = 999;
	int32 loc6 = 0;
	
	for (int32 i = 0; i < loc1.Num(); i++)
	{
		AHex* loc4 = loc1[i];
		if (!loc4->CanEndMoveHere(charRef))
		{
			continue;
		}

		loc6 = GetMain()->Battlefield->GetRawDistanceBetweenHexes(loc4, hex);
		if (loc6 < loc3)
		{
			loc2.Empty(); loc2.Add(loc4); /*loc2 = [loc4]*/
			loc3 = loc6;
			continue;
		}

		if (loc6 != loc3)
		{
			continue;
		}

		loc2.Add(loc4);
	}

	for (int32 i = 0; i < loc2.Num(); i++)
	{
		AHex* loc5 = loc2[i];
		
		if (loc5 == nullptr)
		{
			continue;
		}
		
		UAiAction* loc7 = NewObject<UAiAction>();
		loc7->InitializeAiAction(charRef);
		loc7->moveToHex = loc5;
		possibleActions.Add(loc7);
	}
}

int32 UIntelligence::GetHateFor(UGameCharacter* gameCharacter)
{
	int32 nHate = 0;

	if (latestAttacker == gameCharacter)
	{
		nHate = nHate + 50;
		if (gameCharacter->IsBerserk())
		{
			nHate = nHate + 5000;
		}
	}

	nHate = nHate + GetDamageFrom(gameCharacter);

	nHate = nHate + FMath::RandRange(0, 20);

	for (int32 i = 0; i < gameCharacter->effects.Num(); i++)
	{
		UEffect* effect = gameCharacter->effects[i];
		nHate = nHate + effect->enemyHateModifier;
	}

	return nHate;
}

bool UIntelligence::ValidHexToUseSkillOn(AHex* hexStart, USkill* skill, AHex* hexEnd, int32 arg4 /* = -1*/)
{
	if (arg4 == -1)
	{
		arg4 = GetMain()->Battlefield->GetRawDistanceBetweenHexes(hexStart, hexEnd);
	}

	if (arg4 > skill->GetRange())
	{
		return false;
	}

	if (arg4 < skill->GetMinimumRange())
	{
		return false;
	}

	if (skill->requiresHorizontalLine)
	{
		if (hexStart->gridY != hexEnd->gridY)
		{
			return false;
		}
	}

	if (skill->requiresBehindTarget)
	{
		if (hexStart->GetActorLocation().X < hexEnd->GetActorLocation().X)
		{
			if (hexEnd->avatar->GetFacingLeft())
			{
				return false;
			}
		}
		else if (hexStart->GetActorLocation().X > hexEnd->GetActorLocation().X)
		{
			if (!hexEnd->avatar->GetFacingLeft())
			{
				return false;
			}
		}
	}

	if (skill->rangedAttack && !skill->directAttack)
	{
		if (!GetMain()->Battlefield->LineOfSightBetween(hexStart, hexEnd, charRef))
		{
			return false;
		}
	}
	return true;
}

UAiAction* UIntelligence::SelectAction()
{
	//possibleActions = possibleActions.sortOn("value", Array.NUMERIC | Array.DESCENDING);
	possibleActions.Sort(ActionValueSorter);
	Algo::Reverse(possibleActions);
	UAiAction* possibleAction = possibleActions[0];

	UE_LOG(LogTemp, Warning, TEXT("============= ACTION ================"));
	UE_LOG(LogTemp, Warning, TEXT("AI has chosen action with value %i"), possibleAction->GetActionValue());

	if (possibleAction->skill)
	{
		UE_LOG(LogTemp, Warning, TEXT(" %s (cast count: %i, limit: %i) "), *possibleAction->skill->skillname, possibleAction->skill->aiCastCount, possibleAction->skill->aiCastLimit);
	}

	if (possibleAction->targetCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("  on %s"), *possibleAction->targetCharacter->charName);
	}
	UE_LOG(LogTemp, Warning, TEXT("============= END ACTION ================"));

	return possibleAction;
}

UIntelligence::~UIntelligence()
{
}

int32 UIntelligence::GetDamageFrom(UGameCharacter* gameCharacter)
{
	int32 nIndex = 0;

	while (nIndex < damageSources.Num())
	{
		if (damageSources[nIndex].charAttacker == gameCharacter)
		{
			return damageSources[nIndex].nAmount;
		}

		++nIndex;
	}

	return 0;
}

int32 UIntelligence::MaxRangeOfSkill(USkill* skill)
{
	if (skill->IsUsable() == false)
	{
		return 0;
	}

	return skill->GetRange() + charRef->GetMovesLeft();
}
