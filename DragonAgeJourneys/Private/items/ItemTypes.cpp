#include "ItemTypes.h"

bool IsArmor(int32 nTypeId)
{
	if (nTypeId >= 300 && nTypeId <= 600)
	{
		return true;
	}
	return false;
}

bool IsWeapon(int32 nTypeId)
{
	if (nTypeId > 0 && nTypeId < 200)
	{
		return true;
	}
	return false;
}

