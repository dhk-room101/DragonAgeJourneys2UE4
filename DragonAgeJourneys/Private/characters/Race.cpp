#include "Race.h"

bool IsDarkspawn(int32 nRace)
{
	if (nRace == RACE_HURLOCK)
	{
		return true;
	}

	if (nRace == RACE_SHRIEK)
	{
		return true;
	}

	if (nRace == RACE_OGRE)
	{
		return true;
	}

	if (nRace == RACE_GENLOCK)
	{
		return true;
	}

	return false;
}

