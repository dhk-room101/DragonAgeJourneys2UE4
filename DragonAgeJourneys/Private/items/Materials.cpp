#include "Materials.h"
#include "ItemTypes.h"
#include "Scripting.h"

int32 GetRandomMaterial(int32 nTypeId, int32 nLevel)
{
	if (GetList(nTypeId).Num() > 0)
	{
		if (nLevel <= 3)
		{
			return 0;
		}
		
		if (nLevel >= 6 && FMath::RandRange(0, 100) < 25)
		{
			return 2;
		}
		
		return 1;
	}
	
	return -1;
}

FString MaterialGetName(int32 nMaterial, int32 nTypeId)
{
	if (GetList(nTypeId).Num() > 0)
	{
		FString sString = GetList(nTypeId)[nMaterial];
		return sString;
	}

	return "Error";
}

TArray<FString> GetList(int32 nTypeId)
{
	switch (nTypeId)
	{
	case ITEMTYPE_AMULET:
	case ITEMTYPE_RING:
	{
		return GetMain()->precious;
	}
	case ITEMTYPE_AXE_CHOPPER:
	case ITEMTYPE_AXE_BATTLE:
	case ITEMTYPE_SWORD_LONG:
	case ITEMTYPE_SWORD_GLADIUS:
	case ITEMTYPE_SWORD_RELIC:
	case ITEMTYPE_GREATAXE_DWARVEN:
	case ITEMTYPE_GREATAXE_DARKSPAWN:
	case ITEMTYPE_GREATAXE_ORNATE:
	case ITEMTYPE_GREATSWORD_EVIL:
	case ITEMTYPE_GREATSWORD_NICE:
	case ITEMTYPE_GREATSWORD_DARKSPAWN:
	case ITEMTYPE_DAGGER_KNIFE:
	case ITEMTYPE_DAGGER_KRIS:
	case ITEMTYPE_MACE_FLANGED:
	case ITEMTYPE_MACE_MORNING:
	case ITEMTYPE_HEAVY_HEAD:
	case ITEMTYPE_HEAVY_ARMOR_PLATE:
	case ITEMTYPE_HEAVY_ARMOR_SCALE:
	case ITEMTYPE_HEAVY_ARMOR_SPLINT:
	case ITEMTYPE_SHIELD_TOWER:
	case ITEMTYPE_SHIELD_STEEL:
	{
		return GetMain()->metal;
	}
	case ITEMTYPE_BOW:
	case ITEMTYPE_CROSSBOW:
	case ITEMTYPE_STAFF_STRAIGHT:
	case ITEMTYPE_STAFF_HELIX:
	case ITEMTYPE_STAFF_TRIDENT:
	case ITEMTYPE_SHIELD_BUCKLER:
	{
		return GetMain()->wood;
	}
	case ITEMTYPE_LIGHT_HEAD:
	case ITEMTYPE_ROBE_BODICE:
	case ITEMTYPE_ROBE_DRAPE:
	case ITEMTYPE_ROBE_NORMAL:
	{
		return GetMain()->cloth;
	}
	case ITEMTYPE_BELT:
	case ITEMTYPE_MEDIUM_HEAD:
	case ITEMTYPE_MEDIUM_ARMOR_LEATHER:
	case ITEMTYPE_MEDIUM_ARMOR_REINFORCED:
	case ITEMTYPE_MEDIUM_ARMOR_CHAIN:
	{
		return GetMain()->leather;
	}
	default:
	{
		break;
	}
	}

	return GetMain()->debug;
}