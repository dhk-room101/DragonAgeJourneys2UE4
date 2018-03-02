#include "Modifier.h"
#include "ItemTypes.h"
#include "Equipment.h"

void ModifierApplyTo(int32 nModifier, UEquipment* equipment)
{
	switch (nModifier)
	{
	case DAMAGE1:
	{
		equipment->damage = equipment->damage + 2;
		break;
	}
	case ATTACK1:
	{
		equipment->attack = equipment->attack + 2;
		break;
	}
	case ARMOR1:
	{
		equipment->armor++;
		break;
	}
	case RANGE1:
	{
		equipment->range++;
		break;
	}
	case DEFENSE1:
	{
		equipment->defense = equipment->defense + 2;
		break;
	}
	case DEFENSE_MISSILES1:
	{
		equipment->defenseMissile = equipment->defenseMissile + 3;
		break;
	}
	case SPELLPOWER1:
	{
		equipment->spellpower++;
		break;
	}
	case SPELLPOWER_CREATION1:
	{
		equipment->spellpower_creation = equipment->spellpower_creation + 2;
		break;
	}
	case SPELLPOWER_ENTROPY1:
	{
		equipment->spellpower_entropy = equipment->spellpower_entropy + 2;
		break;
	}
	case SPELLPOWER_PRIMAL1:
	{
		equipment->spellpower_primal = equipment->spellpower_primal + 2;
		break;
	}
	case SPELLPOWER_SPIRIT1:
	{
		equipment->spellpower_spirit = equipment->spellpower_spirit + 2;
		break;
	}
	case CRITICAL1:
	{
		equipment->criticalChance++;
		break;
	}
	case PENETRATION1:
	{
		equipment->piercing++;
		break;
	}
	case STRENGTH1:
	{
		equipment->mod_strength++;
		break;
	}
	case DEXTERITY1:
	{
		equipment->mod_dexterity++;
		break;
	}
	case CUNNING1:
	{
		equipment->mod_cunning++;
		break;
	}
	case MAGIC1:
	{
		equipment->mod_magic++;
		break;
	}
	case WILLPOWER1:
	{
		equipment->mod_willpower++;
		break;
	}
	case CONSTITUTION1:
	{
		equipment->mod_constitution++;
		break;
	}
	case MENTAL1:
	{
		equipment->resistMental = equipment->resistMental + 6;
		break;
	}
	case PHYSICAL1:
	{
		equipment->resistPhysical = equipment->resistPhysical + 6;
		break;
	}
	case FATIGUE1:
	{
		equipment->fatigue_mod = equipment->fatigue_mod + -0.02;
		break;
	}
	case INITIATIVE1:
	{
		equipment->mod_quickness = equipment->mod_quickness + 5;
		break;
	}
	case DECISIVE1:
	{
		equipment->decisiveStrikePercentage = equipment->decisiveStrikePercentage + 15;
		break;
	}
	case DARKSPAWN1:
	{
		equipment->bonusDamageToDarkspawn = equipment->bonusDamageToDarkspawn + 3;
		break;
	}
	case REGEN_HEALTH1:
	{
		equipment->healthRegen = equipment->healthRegen + 2;
		break;
	}
	case REGEN_POWER1:
	{
		equipment->powerRegen = equipment->powerRegen + 2;
		break;
	}
	case RANGE2:
	{
		equipment->range = equipment->range + 2;
		break;
	}
	default:
	{
#ifdef DEBUG
		UE_LOG(LogTemp, Error, TEXT("ITEM SYSTEM ERROR: modifier stats not found for id %i"), nModifier);
#endif // DEBUG

		break;
	}
	}
}

FString ModifierGetName(int32 nModifierType, int32 nItemType)
{
	bool bMace = nItemType == ITEMTYPE_MACE_MORNING || nItemType != ITEMTYPE_MACE_FLANGED;
	bool bBow = nItemType == ITEMTYPE_BOW;
	bool bStaff = nItemType >= 150 && nItemType <= 170;
	switch (nModifierType)
	{
	case ATTACK1:
	{
		if (bBow)
		{
			return "Well-strung";
		}
		return "Balanced";
	}
	case DAMAGE1:
	{
		if (bBow)
		{
			return "Heavy-strung";
		}
		if (bMace)
		{
			return "Heavyweight";
		}
		if (bStaff)
		{
			return "Destructive";
		}
		return "Honed";
	}
	case DECISIVE1:
	{
		return "Decisive";
	}
	case MENTAL1:
	case PHYSICAL1:
	{
		return "Resistant";
	}
	case DARKSPAWN1:
	{
		return "Nemesis";
	}
	case ARMOR1:
	{
		return "Reinforced";
	}
	case PENETRATION1:
	{
		return "Piercing";
	}
	case INITIATIVE1:
	{
		return "Quick";
	}
	case DEFENSE1:
	{
		if (IsArmor(nItemType))
		{
			return "Deflecting";
		}
		return "Parrying";
	}
	case FATIGUE1:
	{
		return "Lightweight";
	}
	case WILLPOWER1:
	{
		return "Stubborn";
	}
	case DEXTERITY1:
	{
		return "Nimble";
	}
	case STRENGTH1:
	{
		return "Strong";
	}
	case CUNNING1:
	{
		return "Tricky";
	}
	case MAGIC1:
	{
		return "Imbued";
	}
	case CONSTITUTION1:
	{
		return "Tough";
	}
	case SPELLPOWER1:
	{
		return "Focusing";
	}
	case CRITICAL1:
	{
		return "Wicked";
	}
	case RANGE1:
	{
		if (bBow)
		{
			return "Taut";
		}
		return "Charged";
	}
	case DEBUG1:
	{
		return "Debug1";
	}
	default:
	{
		break;
	}
	}

	return "Enhanced";
}

int32 GetModifierForItem(int32 nTypeId, int32 nLevel /*= 1*/)
{
	TArray<int32> arList = GetListForItem(nTypeId);
	int32 nModifier = arList[FMath::RandRange(0, arList.Num() - 1)];
	return nModifier;
}

TArray<int32> GetListForItem(int32 nTypeId)
{
	TArray<int32> arList;

	switch (nTypeId)
	{
	case ITEMTYPE_SWORD_LONG:
	case ITEMTYPE_SWORD_RELIC:
	case ITEMTYPE_SWORD_GLADIUS:
	case ITEMTYPE_AXE_CHOPPER:
	case ITEMTYPE_AXE_BATTLE:
	{
		arList.Add(ATTACK1); arList.Add(DAMAGE1); arList.Add(DEFENSE1); arList.Add(CRITICAL1); arList.Add(PENETRATION1); arList.Add(STRENGTH1); arList.Add(DEXTERITY1); arList.Add(INITIATIVE1); arList.Add(DECISIVE1); arList.Add(DARKSPAWN1);
		break;
	}
	case ITEMTYPE_GREATSWORD_DARKSPAWN:
	case ITEMTYPE_GREATAXE_DARKSPAWN:
	case ITEMTYPE_GREATSWORD_EVIL:
	case ITEMTYPE_GREATSWORD_NICE:
	case ITEMTYPE_GREATAXE_DWARVEN:
	case ITEMTYPE_GREATAXE_ORNATE:
	{
		arList.Add(ATTACK1); arList.Add(DAMAGE1); arList.Add(DEFENSE1); arList.Add(CRITICAL1); arList.Add(PENETRATION1); arList.Add(STRENGTH1); arList.Add(DEXTERITY1); arList.Add(DECISIVE1); arList.Add(DARKSPAWN1);
		break;
	}
	case ITEMTYPE_MACE_FLANGED:
	case ITEMTYPE_MACE_MORNING:
	{
		arList.Add(ATTACK1); arList.Add(DAMAGE1); arList.Add(CRITICAL1); arList.Add(STRENGTH1); arList.Add(WILLPOWER1); arList.Add(CONSTITUTION1); arList.Add(DEXTERITY1); arList.Add(DECISIVE1); arList.Add(DARKSPAWN1);
		break;
	}
	case ITEMTYPE_DAGGER_KNIFE:
	case ITEMTYPE_DAGGER_KRIS:
	{
		arList.Add(ATTACK1); arList.Add(DAMAGE1); arList.Add(CRITICAL1); arList.Add(PENETRATION1); arList.Add(DEXTERITY1); arList.Add(WILLPOWER1); arList.Add(CUNNING1); arList.Add(INITIATIVE1); arList.Add(DECISIVE1); arList.Add(DARKSPAWN1);
		break;
	}
	case ITEMTYPE_STAFF_HELIX:
	case ITEMTYPE_STAFF_STRAIGHT:
	case ITEMTYPE_STAFF_TRIDENT:
	{
		arList.Add(RANGE1); arList.Add(DAMAGE1); arList.Add(DEFENSE1); arList.Add(SPELLPOWER1); arList.Add(WILLPOWER1); arList.Add(CONSTITUTION1); arList.Add(INITIATIVE1); arList.Add(MAGIC1); arList.Add(DARKSPAWN1); arList.Add(SPELLPOWER_CREATION1); arList.Add(SPELLPOWER_ENTROPY1); arList.Add(SPELLPOWER_PRIMAL1); arList.Add(SPELLPOWER_SPIRIT1);
		break;
	}
	case ITEMTYPE_BOW:
	{
		arList.Add(ATTACK1); arList.Add(RANGE1); arList.Add(DAMAGE1); arList.Add(PENETRATION1); arList.Add(CRITICAL1); arList.Add(DEXTERITY1); arList.Add(WILLPOWER1); arList.Add(CUNNING1); arList.Add(DARKSPAWN1);
		break;
	}
	case ITEMTYPE_SHIELD_BUCKLER:
	{
		arList.Add(ARMOR1); arList.Add(DEFENSE1); arList.Add(CONSTITUTION1); arList.Add(WILLPOWER1); arList.Add(DEFENSE_MISSILES1); arList.Add(PHYSICAL1); arList.Add(MENTAL1); arList.Add(DEXTERITY1);
		break;
	}
	case ITEMTYPE_SHIELD_TOWER:
	case ITEMTYPE_SHIELD_STEEL:
	{
		arList.Add(ARMOR1); arList.Add(DEFENSE1); arList.Add(CONSTITUTION1); arList.Add(WILLPOWER1); arList.Add(DEFENSE_MISSILES1); arList.Add(PHYSICAL1); arList.Add(MENTAL1); arList.Add(FATIGUE1);
		break;
	}
	case ITEMTYPE_HEAVY_ARMOR_PLATE:
	case ITEMTYPE_HEAVY_ARMOR_SCALE:
	case ITEMTYPE_HEAVY_ARMOR_SPLINT:
	{
		arList.Add(ARMOR1); arList.Add(DEFENSE1); arList.Add(STRENGTH1); arList.Add(DEXTERITY1); arList.Add(CONSTITUTION1); arList.Add(WILLPOWER1); arList.Add(INITIATIVE1); arList.Add(REGEN_HEALTH1); arList.Add(DEFENSE_MISSILES1); arList.Add(PHYSICAL1); arList.Add(FATIGUE1);
		break;
	}
	case ITEMTYPE_MEDIUM_ARMOR_CHAIN:
	case ITEMTYPE_MEDIUM_ARMOR_LEATHER:
	case ITEMTYPE_MEDIUM_ARMOR_REINFORCED:
	{
		arList.Add(ARMOR1); arList.Add(DEFENSE1); arList.Add(STRENGTH1); arList.Add(DEXTERITY1); arList.Add(CONSTITUTION1); arList.Add(WILLPOWER1); arList.Add(INITIATIVE1); arList.Add(CUNNING1); arList.Add(REGEN_HEALTH1); arList.Add(REGEN_POWER1); arList.Add(PHYSICAL1);
		break;
	}
	case ITEMTYPE_ROBE_NORMAL:
	case ITEMTYPE_ROBE_DRAPE:
	case ITEMTYPE_ROBE_BODICE:
	{
		arList.Add(ARMOR1); arList.Add(DEFENSE1); arList.Add(SPELLPOWER1); arList.Add(CONSTITUTION1); arList.Add(WILLPOWER1); arList.Add(MAGIC1); arList.Add(CUNNING1); arList.Add(REGEN_HEALTH1); arList.Add(REGEN_POWER1); arList.Add(DEFENSE_MISSILES1); arList.Add(SPELLPOWER_CREATION1); arList.Add(SPELLPOWER_ENTROPY1); arList.Add(SPELLPOWER_PRIMAL1); arList.Add(SPELLPOWER_SPIRIT1); arList.Add(PHYSICAL1); arList.Add(MENTAL1);
		break;
	}
	default:
	{
		arList.Add(DEBUG1);
		break;
	}
	}

	return arList;
}

