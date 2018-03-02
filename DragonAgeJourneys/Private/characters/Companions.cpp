#include "Companions.h"

FString GetCompanionName(int32 nClassId)
{
	switch (nClassId)
	{
	case COMPANION_BERSERKER:
	{
		return "Gheren";
	}
	case COMPANION_BLOODMAGE:
	{
		return "Elbeth";
	}
	case COMPANION_GREY_WARDEN:
	{
		return "Martine";
	}
	case COMPANION_SPIRIT_HEALER:
	{
		return "Ardum";
	}
	case COMPANION_BARD:
	{
		return "Ryanth";
	}
	case COMPANION_ASSASSIN:
	{
		return "Maeriss";
	}
	default:
	{
		break;
	}
	}
	return "Mystery Companion";
}

