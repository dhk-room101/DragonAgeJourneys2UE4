#include "Equipment.h"
#include "ItemTypes.h"
#include "Item.h"
#include "Modifier.h"
#include "Slot.h"
#include "Weapon.h"
#include "Materials.h"

UEquipment::UEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UEquipment::InitializeEquipment(int32 nItemTypeId, int32 nLevel /*= 1*/, int32 nRarity /*= 0*/, int32 nMaterial /*= 0*/, int32 nModifier /*= 0*/)
{
	float fBase = 0.f;
	int32 nMod = 0;

	modifiers.Empty();
	itemName = "Equipment";
	slot = GEAR_SLOTS;
	iconFrame = 1;
	whoEquips = EQUIP_TO_ALL;
	avatarFrame = 1;
	hue = 0;
	saturation = 0;

	if (nItemTypeId == -1)
	{
		return;
	}

	itemLevel = nLevel;
	itemRarity = nRarity;
	typeId = nItemTypeId;

	if (nMaterial != -1)
	{
		material = nMaterial;
	}
	else
	{
		material = GetRandomMaterial(typeId, itemLevel);
	}

	//TODO Modifier array for nModifier, currently only one hard coded
	modifier1 = nModifier;
	modifiers.Add(nModifier);

	if (itemRarity == -1)
	{
		if (nModifier == 0)
		{
			if (FMath::RandRange(1, 100) < 20)
			{
				itemRarity = ITEM_NORMAL;
			}
			else if (FMath::RandRange(1, 100) < 70)
			{
				itemRarity = ITEM_ONE_MOD;
			}
			else if (FMath::RandRange(1, 100) < 90)
			{
				itemRarity = ITEM_TWO_MODS;
			}
			else if (FMath::RandRange(1, 100) < 97)
			{
				itemRarity = ITEM_THREE_MODS;
			}
			else
			{
				itemRarity = ITEM_FOUR_MODS;
			}
		}
		else if (modifiers.Num() == 0)
		{
			itemRarity = ITEM_NORMAL;
		}
		else
		{
			itemRarity = modifiers.Num();
		}
	}

	SetStatsForEquipmentType(typeId);

	if (itemRarity > ITEM_NORMAL && modifiers.Num() > 0)
	{
		int32 nCounter = 0;
		while (nCounter < itemRarity)
		{
			//TODO Doublecheck modifiers applying

			nMod = GetModifierForItem(typeId, itemLevel);
			modifiers.Add(nMod);

			if (modifier1 == 0)
			{
				modifier1 = nMod;
			}
			else if (modifier2 == 0)
			{
				modifier2 = nMod;
			}
			else if (modifier3 == 0)
			{
				modifier3 = nMod;
			}
			else if (modifier4 == 0)
			{
				modifier4 = nMod;
			}
			++nCounter;
		}
	}

	fBase = 0.3;
	float fMaterial = 1 + fBase * material;
	if (armor > 0)
	{
		armor = FMath::CeilToFloat(armor * fMaterial);
	}
	if (defense > 0)
	{
		defense = FMath::CeilToFloat(defense * fMaterial);
	}
	if (damage > 0)
	{
		damage = FMath::CeilToFloat(damage * fMaterial);
	}
	if (attack > 0)
	{
		attack = FMath::CeilToFloat(attack * fMaterial);
	}
	if (piercing > 0)
	{
		piercing = FMath::CeilToFloat(piercing * fMaterial);
	}

	for (int32 i = 0; i < modifiers.Num(); i++)
	{
		if (modifiers[i] != 0)
			ModifierApplyTo(modifiers[i], this);
	}

	if (modifiers.Num() > 0)
	{
		itemName = ModifierGetName(modifiers[FMath::RandRange(0, modifiers.Num() - 1)], typeId) + " " + itemName;
		if (IsWeapon())
		{
			if (modifiers.Num() == 3)
				vfx = VFX_GLOW_WEAK_BLUE;

			if (modifiers.Num() == 4)
				vfx = VFX_GLOW_WEAK_GREEN;
		}
		else if (IsArmor())
		{
			if (modifiers.Num() == 3)
			{
				switch (typeId)
				{
				case ITEMTYPE_MEDIUM_ARMOR_REINFORCED:
				{
					hue = 40;
					break;
				}
				case ITEMTYPE_MEDIUM_ARMOR_CHAIN:
				{
					hue = 40;
					break;
				}
				case ITEMTYPE_MEDIUM_ARMOR_LEATHER:
				{
					hue = -40;
					break;
				}
				case ITEMTYPE_HEAVY_ARMOR_SPLINT:
				{
					hue = 40;
					break;
				}
				case ITEMTYPE_HEAVY_ARMOR_SCALE:
				{
					hue = -40;
					break;
				}
				case ITEMTYPE_HEAVY_ARMOR_PLATE:
				{
					hue = -40;
					break;
				}
				case ITEMTYPE_ROBE_NORMAL:
				{
					hue = -170;
					break;
				}
				case ITEMTYPE_ROBE_BODICE:
				{
					hue = 40;
					break;
				}
				case ITEMTYPE_ROBE_DRAPE:
				{
					hue = 100;
					break;
				}
				default:
				{
					hue = -40;
					break;
				}
				}
			}
			else if (modifiers.Num() == 4)
			{
				switch (typeId)
				{
				case ITEMTYPE_MEDIUM_ARMOR_LEATHER:
				{
					hue = 180;
					break;
				}
				case ITEMTYPE_MEDIUM_ARMOR_REINFORCED:
				{
					hue = -40;
					break;
				}
				case ITEMTYPE_MEDIUM_ARMOR_CHAIN:
				{
					hue = -40;
					break;
				}
				case ITEMTYPE_HEAVY_ARMOR_SPLINT:
				{
					hue = -40;
					break;
				}
				case ITEMTYPE_HEAVY_ARMOR_SCALE:
				{
					hue = 60;
					break;
				}
				case ITEMTYPE_HEAVY_ARMOR_PLATE:
				{
					hue = 180;
					break;
				}
				case ITEMTYPE_ROBE_NORMAL:
				{
					hue = 150;
					break;
				}
				case ITEMTYPE_ROBE_BODICE:
				{
					hue = -40;
					break;
				}
				case ITEMTYPE_ROBE_DRAPE:
				{
					hue = -130;
					break;
				}
				default:
				{
					hue = 40;
					break;
				}
				}
			}
		}
	}

	itemName = itemName + (" (" + MaterialGetName(material, typeId) + ")");

	/*//DHK
	const TCHAR* arChar = *itemName;
	this->Rename(arChar, this->GetOuter(), REN_None);*/
}

bool UEquipment::IsShield()
{
	if (typeId == ITEMTYPE_SHIELD_TOWER)
	{
		return true;
	}

	if (typeId == ITEMTYPE_SHIELD_BUCKLER)
	{
		return true;
	}

	if (typeId == ITEMTYPE_SHIELD_STEEL)
	{
		return true;
	}

	return false;
}

bool UEquipment::IsWeapon()
{
	if (typeId > 0 && typeId < 200)
	{
		return true;
	}

	return false;
}

bool UEquipment::IsHeavyArmor()
{
	if (typeId >= 500 && typeId < 600)
	{
		return true;
	}

	return false;
}

bool UEquipment::IsMediumArmor()
{
	if (typeId >= 400 && typeId < 500)
	{
		return true;
	}

	return false;
}

bool UEquipment::IsArmor()
{
	if (slot == SLOT_ARMOR)
		return true;

	return false;
}

bool UEquipment::IsMelee()
{
	if (typeId > 0 && typeId < 100)
	{
		return true;
	}

	return false;
}

bool UEquipment::IsRanged()
{
	if (typeId == ITEMTYPE_BOW || typeId == ITEMTYPE_CROSSBOW)
	{
		return true;
	}

	return false;
}

bool UEquipment::IsTwoHander()
{
	if (typeId >= 60 && typeId <= 80)
	{
		return true;
	}

	return false;
}

bool UEquipment::IsTwoHanded()
{
	if (IsTwoHander())
	{
		return true;
	}

	if (IsStaff())
	{
		return true;
	}

	if (IsRanged())
	{
		return true;
	}

	return false;
}

bool UEquipment::IsStaff()
{
	if (typeId >= 150 && typeId <= 170)
	{
		return true;
	}

	return false;
}

bool UEquipment::IsOffHand()
{
	if (IsShield())
	{
		return true;
	}

	if (typeId == ITEMTYPE_TOME)
	{
		return true;
	}

	return false;
}

void UEquipment::SetStatsForEquipmentType(int32 nTypeId)
{
	switch (nTypeId)
	{
	case ITEMTYPE_SWORD_LONG:
	{
		itemName = "Longsword";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 4;
		damage = sword_base_damage;
		avatarFrame = WEAPON_SWORD_STANDARD;
		valueMod = 1;
		break;
	}
	case ITEMTYPE_SWORD_GLADIUS:
	{
		itemName = "Gladius";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 4;
		damage = sword_base_damage;
		avatarFrame = WEAPON_SWORD_HILTLESS;
		valueMod = 1.01;
		break;
	}
	case ITEMTYPE_SWORD_RELIC:
	{
		itemName = "Relic Blade";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 4;
		damage = sword_base_damage - 1;
		avatarFrame = WEAPON_SWORD_ANCIENT;
		valueMod = 1.15;
		break;
	}
	case ITEMTYPE_GREATSWORD_NICE:
	{
		itemName = "Greatsword";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 5;
		damage = two_hander_base_damage + 1;
		avatarFrame = WEAPON_GREATSWORD_NICE;
		valueMod = 1.42;
		break;
	}
	case ITEMTYPE_GREATSWORD_EVIL:
	{
		itemName = "Berserker\'s Blade";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 5;
		damage = two_hander_base_damage + 2;
		avatarFrame = WEAPON_GREATSWORD_NASTY;
		valueMod = 1.52;
		break;
	}
	case ITEMTYPE_GREATSWORD_DARKSPAWN:
	{
		itemName = "Darkspawn Sword";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 5;
		damage = two_hander_base_damage + 2;
		attack = -2;
		avatarFrame = WEAPON_SWORD_DARKSPAWN;
		valueMod = 0.48;
		break;
	}
	case ITEMTYPE_MACE_FLANGED:
	{
		itemName = "Mace";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 6;
		damage = mace_base_damage;
		avatarFrame = 3;
		valueMod = 1.03;
		break;
	}
	case ITEMTYPE_MACE_MORNING:
	{
		itemName = "Morningstar";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 6;
		damage = mace_base_damage;
		avatarFrame = 4;
		valueMod = 1.02;
		break;
	}
	case ITEMTYPE_AXE_CHOPPER:
	{
		itemName = "Axe";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 7;
		damage = sword_base_damage;
		avatarFrame = 5;
		valueMod = 1.01;
		break;
	}
	case ITEMTYPE_AXE_BATTLE:
	{
		itemName = "Battle-Axe";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 7;
		damage = sword_base_damage;
		avatarFrame = WEAPON_AXE_DWARF;
		valueMod = 1.05;
		break;
	}
	case ITEMTYPE_GREATAXE_DWARVEN:
	{
		itemName = "Dwarven Greataxe";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 2;
		damage = two_hander_base_damage;
		piercing = 2;
		avatarFrame = WEAPON_GREATAXE_DWARF;
		valueMod = 1.45;
		break;
	}
	case ITEMTYPE_GREATAXE_ORNATE:
	{
		itemName = "Ornate Greataxe";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 2;
		piercing = 2;
		attack = 1;
		damage = two_hander_base_damage;
		avatarFrame = WEAPON_GREATAXE_ORNATE;
		valueMod = 1.65;
		break;
	}
	case ITEMTYPE_GREATAXE_DARKSPAWN:
	{
		itemName = "Darkspawn Axe";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 2;
		damage = two_hander_base_damage + 1;
		piercing = 1;
		attack = -1;
		avatarFrame = WEAPON_AXE_DARKSPAWN;
		valueMod = 0.5;
		break;
	}
	case ITEMTYPE_DAGGER_KNIFE:
	{
		itemName = "Dagger";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 8;
		damage = dagger_base_damage;
		avatarFrame = 7;
		valueMod = 0.95;
		break;
	}
	case ITEMTYPE_DAGGER_KRIS:
	{
		itemName = "Kris";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 8;
		damage = dagger_base_damage;
		avatarFrame = WEAPON_DAGGER_CURVY;
		valueMod = 0.95;
		break;
	}
	case ITEMTYPE_BOW:
	{
		itemName = "Longbow";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 9;
		damage = bow_base_damage;
		range = RANGE_BOW_DEFAULT;
		avatarFrame = WEAPON_BOW_LONG;
		valueMod = 1.32;
		break;
	}
	case ITEMTYPE_STAFF_STRAIGHT:
	{
		itemName = "Circle Staff";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_MAGE_ALL;
		iconFrame = 3;
		damage = staff_base_damage;
		range = RANGE_STAFF_DEFAULT;
		avatarFrame = WEAPON_STAFF_BULBOUS;
		valueMod = 1.55;
		break;
	}
	case ITEMTYPE_STAFF_HELIX:
	{
		itemName = "Enchanter\'s Staff";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_MAGE_ALL;
		iconFrame = 3;
		damage = staff_base_damage;
		range = RANGE_STAFF_DEFAULT;
		avatarFrame = WEAPON_STAFF_TWISTY;
		mod_magic = 1;
		valueMod = 1.6;
		break;
	}
	case ITEMTYPE_STAFF_TRIDENT:
	{
		itemName = "Tined Staff";
		slot = SLOT_WEAPON;
		whoEquips = EQUIP_TO_MAGE_ALL;
		iconFrame = 3;
		damage = staff_base_damage + 1;
		range = RANGE_STAFF_DEFAULT;
		avatarFrame = WEAPON_STAFF_TRIDENT;
		spellpower = 1;
		valueMod = 1.7;
		break;
	}
	case ITEMTYPE_BELT:
	{
		itemName = "Belt";
		iconFrame = 35;
		slot = SLOT_BELT;
		valueMod = 0.8;
		break;
	}
	case ITEMTYPE_LIGHT_HEAD:
	{
		itemName = "Hood";
		iconFrame = 33;
		slot = SLOT_HELMET;
		break;
	}
	case ITEMTYPE_ROBE_BODICE:
	case ITEMTYPE_ROBE_DRAPE:
	case ITEMTYPE_ROBE_NORMAL:
	{
		itemName = "Robes";
		iconFrame = 30;
		whoEquips = EQUIP_TO_MAGE_ALL;
		armor = 0;
		slot = SLOT_ARMOR;
		//TODO check avatar frame based on item type
		if (nTypeId == ITEMTYPE_ROBE_NORMAL)
		{
			avatarFrame = 1;
		}
		else if (nTypeId == ITEMTYPE_ROBE_BODICE)
		{
			avatarFrame = 2;
		}
		else if (nTypeId == ITEMTYPE_ROBE_DRAPE)
		{
			avatarFrame = 3;
		}
		valueMod = 1.15;
		break;
	}
	case ITEMTYPE_MEDIUM_ARMOR_LEATHER:
	{
		itemName = "Leather Armor";
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 32;
		slot = SLOT_ARMOR;
		valueMod = 1.25;
		armor = 1;
		avatarFrame = 1;
		break;
	}
	case ITEMTYPE_MEDIUM_ARMOR_REINFORCED:
	{
		itemName = "Reinforced Leather";
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 32;
		slot = SLOT_ARMOR;
		valueMod = 1.3;
		armor = 1;
		defense = 2;
		avatarFrame = 2;
		break;
	}
	case ITEMTYPE_MEDIUM_ARMOR_CHAIN:
	{
		itemName = "Leather Armor";
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 32;
		slot = SLOT_ARMOR;
		valueMod = 1.45;
		armor = 2;
		defense = 2;
		avatarFrame = 3;
		break;
	}
	case ITEMTYPE_HEAVY_ARMOR_SPLINT:
	{
		itemName = "Splint Mail";
		iconFrame = 33;
		slot = SLOT_ARMOR;
		avatarFrame = 1;
		whoEquips = EQUIP_TO_WARRIOR_ALL + EQUIP_TO_ROGUE_ALL;
		armor = 4;
		valueMod = 1.25;
		break;
	}
	case ITEMTYPE_HEAVY_ARMOR_SCALE:
	{
		itemName = "Scale Mail";
		iconFrame = 33;
		slot = SLOT_ARMOR;
		avatarFrame = 3;
		whoEquips = EQUIP_TO_WARRIOR_ALL + EQUIP_TO_ROGUE_ALL;
		armor = 4;
		defense = 2;
		valueMod = 1.35;
		break;
	}
	case ITEMTYPE_HEAVY_ARMOR_PLATE:
	{
		itemName = "Plate Armor";
		iconFrame = 33;
		slot = SLOT_ARMOR;
		avatarFrame = 4;
		whoEquips = EQUIP_TO_WARRIOR_ALL + EQUIP_TO_ROGUE_ALL;
		armor = 5;
		defense = 1;
		defenseMissile = 1;
		valueMod = 1.5;
		break;
	}
	case ITEMTYPE_MEDIUM_HEAD:
	{
		itemName = "Helmet";
		iconFrame = 48;
		slot = SLOT_HELMET;
		break;
	}
	case ITEMTYPE_HEAVY_HEAD:
	{
		itemName = "Plate Helm";
		slot = SLOT_HELMET;
		iconFrame = 2;
		break;
	}
	case ITEMTYPE_RING:
	{
		itemName = "Ring";
		slot = SLOT_RING1;
		whoEquips = EQUIP_TO_ALL;
		iconFrame = 36;
		valueMod = 0.7;
		break;
	}
	case ITEMTYPE_AMULET:
	{
		itemName = "Amulet";
		slot = SLOT_AMULET;
		whoEquips = EQUIP_TO_ALL;
		iconFrame = 37;
		valueMod = 0.75;
		break;
	}
	case ITEMTYPE_AMMO:
	{
		itemName = "Arrows";
		slot = SLOT_AMMO;
		whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		iconFrame = 38;
		valueMod = 0.62;
		break;
	}
	case ITEMTYPE_SHIELD_TOWER:
	{
		itemName = "Kite Shield";
		slot = SLOT_OFFHAND;
		whoEquips = EQUIP_TO_WARRIOR_ALL;
		defense = 4;
		armor = 3;
		avatarFrame = 3;
		iconFrame = 42;
		valueMod = 1.4;
		break;
	}
	case ITEMTYPE_SHIELD_STEEL:
	{
		itemName = "Steel Shield";
		slot = SLOT_OFFHAND;
		whoEquips = EQUIP_TO_WARRIOR_ALL;
		armor = 2;
		defense = 3;
		avatarFrame = 4;
		iconFrame = 41;
		valueMod = 1.2;
		break;
	}
	case ITEMTYPE_SHIELD_BUCKLER:
	{
		itemName = "Round Shield";
		slot = SLOT_OFFHAND;
		whoEquips = EQUIP_TO_WARRIOR_ALL;
		armor = 1;
		defense = 1;
		avatarFrame = 2;
		iconFrame = 40;
		valueMod = 1.1;
		break;
	}
	case 0:
	default:
	{
		break;
	}
	}
}

UEquipment::~UEquipment()
{
}
