#include "Difficulty.h"
#include "Scripting.h"
#include "Party.h"

int32 ModifyMonsterHealth(int32 nHealth)
{
	if (GetParty()->GetDifficulty() == DIFFICULTY_NORMAL)
	{
		return nHealth;
	}
	if (GetParty()->GetDifficulty() == DIFFICULTY_EASY)
	{
		nHealth = nHealth * 0.75;
	}
	else if (GetParty()->GetDifficulty() == DIFFICULTY_HARD)
	{
		nHealth = nHealth * 1.5;
	}
	return FMath::RoundToInt(nHealth);
}

int32 ModifyMonsterDamage(int32 nDamage)
{
	int32 nDifficulty = GetParty()->GetDifficulty();
	
	if (nDifficulty == DIFFICULTY_NORMAL)
	{
		return nDamage;
	}
	
	if (nDifficulty == DIFFICULTY_EASY)
	{
		nDamage = nDamage * 0.75;
	}
	
	if (nDifficulty == DIFFICULTY_HARD)
	{
		nDamage = nDamage * 1.2;
	}

	return FMath::RoundToInt(nDamage);
}

