#include "UniqueItems.h"
#include "Equipment.h"
#include "ItemTypes.h"
#include "Companions.h"

UEquipment* GetUniqueItem(int32 nTypeId)
{
	UEquipment* uniqueItem = NewObject<UEquipment>();

	//TODO PlotItems
	/*if (nTypeId > 99000)
	{
		return PlotItems.getItem(nTypeId);
	}*/
	
	switch (nTypeId)
	{
	case UNIQUEITEM_BARE_HANDS:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SWORD_LONG, 1, ITEM_NORMAL);
		uniqueItem->avatarFrame = 99;
		break;
	}
	case UNIQUEITEM_STARTER_GREATSWORD:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_GREATSWORD_NICE, 1, ITEM_NORMAL);
		break;
	}
	case UNIQUEITEM_STARTER_LONGSWORD:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SWORD_LONG, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Worn Longsword";
		break;
	}
	case UNIQUEITEM_STARTER_BUCKLER:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SHIELD_BUCKLER, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Worn Buckler";
		break;
	}
	case UNIQUEITEM_STARTER_DAGGER:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_DAGGER_KNIFE, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Worn Dagger";
		break;
	}
	case UNIQUEITEM_STARTER_BOW:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BOW, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Worn Longbow";
		break;
	}
	case UNIQUEITEM_STARTER_STAFF:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_STAFF_STRAIGHT, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Worn Staff";
		break;
	}
	case UNIQUEITEM_STARTER_ARMOR:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_HEAVY_ARMOR_SPLINT, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Plate Armor";
		break;
	}
	case UNIQUEITEM_STARTER_LEATHER:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_MEDIUM_ARMOR_LEATHER, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Leather Armor";
		break;
	}
	case UNIQUEITEM_STARTER_ROBES:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_ROBE_NORMAL, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Robes";
		break;
	}
	case UNIQUEITEM_DUMMY_PLATE_1:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_HEAVY_ARMOR_SPLINT, 1);
		uniqueItem->avatarFrame = 1;
		break;
	}
	case UNIQUEITEM_DUMMY_PLATE_2:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_HEAVY_ARMOR_SPLINT, 1);
		uniqueItem->avatarFrame = 2;
		break;
	}
	case UNIQUEITEM_DUMMY_PLATE_3:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_HEAVY_ARMOR_SPLINT, 1);
		uniqueItem->avatarFrame = 3;
		break;
	}
	case UNIQUEITEM_DUMMY_PLATE_4:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_HEAVY_ARMOR_SPLINT, 1);
		uniqueItem->avatarFrame = 4;
		break;
	}
	case UNIQUEITEM_DUMMY_PLATE_5:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_HEAVY_ARMOR_SPLINT, 1);
		uniqueItem->avatarFrame = 5;
		break;
	}
	case UNIQUEITEM_DUMMY_LEATHER_1:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_MEDIUM_ARMOR_LEATHER, 1);
		uniqueItem->avatarFrame = 1;
		break;
	}
	case UNIQUEITEM_DUMMY_LEATHER_2:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_MEDIUM_ARMOR_LEATHER, 1);
		uniqueItem->avatarFrame = 2;
		break;
	}
	case UNIQUEITEM_DUMMY_LEATHER_3:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_MEDIUM_ARMOR_LEATHER, 1);
		uniqueItem->avatarFrame = 3;
		break;
	}
	case UNIQUEITEM_DUMMY_LEATHER_4:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_MEDIUM_ARMOR_LEATHER, 1);
		uniqueItem->avatarFrame = 4;
		break;
	}
	case UNIQUEITEM_DUMMY_LEATHER_5:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_MEDIUM_ARMOR_LEATHER, 1);
		uniqueItem->avatarFrame = 5;
		break;
	}
	case UNIQUEITEM_WARDEN_BLADE:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SWORD_GLADIUS, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Warden Blade";
		(uniqueItem->attack + 1);
		uniqueItem->bonusDamageToDarkspawn = 1;
		break;
	}
	case UNIQUEITEM_WARDEN_SHIELD:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SHIELD_BUCKLER, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Warden Shield";
		uniqueItem->defense = uniqueItem->defense + 2;
		break;
	}
	case UNIQUEITEM_TEST_SWORD_MAGIC:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SWORD_LONG, 1);
		uniqueItem->itemName = "Enchanted Longsword";
		uniqueItem->avatarFrame = 1;
		uniqueItem->vfx = VFX_GLOW_WEAK_BLUE;
		uniqueItem->attack = 10;
		break;
	}
	case UNIQUEITEM_MARKS_BIG_SWORD:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_GREATSWORD_EVIL, 1);
		uniqueItem->itemName = "Enormous Greatsword";
		uniqueItem->avatarFrame = 3;
		uniqueItem->vfx = VFX_GLOW_WEAK_GREEN;
		uniqueItem->attack = 12;
		uniqueItem->mod_strength = 2;
		uniqueItem->piercing = 2;
		break;
	}
	case UNIQUEITEM_TEST_SHIELD_BUCKLER:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SHIELD_BUCKLER, 1);
		uniqueItem->itemName = "Wooden Buckler";
		break;
	}
	case UNIQUEITEM_TEST_SHIELD_STEEL:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SHIELD_STEEL, 1);
		uniqueItem->itemName = "Steel Shield";
		break;
	}
	case UNIQUEITEM_TEST_SHIELD_TOWER:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SHIELD_TOWER, 1);
		uniqueItem->itemName = "Kite Shield";
		break;
	}
	case UNIQUEITEM_COMPANION_BLOODMAGE_ROBES:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_ROBE_NORMAL, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_BLOODMAGE) + "\'s Robes";
		uniqueItem->avatarFrame = 2;
		uniqueItem->hue = -40;
		uniqueItem->iconFrame = 30;
		uniqueItem->armor = 1;
		break;
	}
	case UNIQUEITEM_COMPANION_BLOODMAGE_STAFF:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_STAFF_HELIX, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_BLOODMAGE) + "\'s Staff";
		uniqueItem->iconFrame = 3;
		uniqueItem->attack = 2;
		break;
	}
	case UNIQUEITEM_COMPANION_HEALER_ROBES:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_ROBE_NORMAL, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_SPIRIT_HEALER) + "\'s Robes";
		uniqueItem->avatarFrame = 4;
		uniqueItem->iconFrame = 30;
		uniqueItem->hue = 180;
		uniqueItem->armor = 1;
		uniqueItem->defenseMissile = 2;
		break;
	}
	case UNIQUEITEM_COMPANION_HEALER_STAFF:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_STAFF_STRAIGHT, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_SPIRIT_HEALER) + "\'s Staff";
		uniqueItem->iconFrame = 3;
		uniqueItem->spellpower_creation = 2;
		break;
	}
	case UNIQUEITEM_COMPANION_BERSERKER_ARMOR:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_HEAVY_ARMOR_SPLINT, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_BERSERKER) + "\'s Armor";
		uniqueItem->iconFrame = 33;
		uniqueItem->avatarFrame = 2;
		uniqueItem->armor = 5;
		break;
	}
	case UNIQUEITEM_COMPANION_BERSERKER_WEAPON:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_GREATAXE_DWARVEN, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_BERSERKER) + "\'s Axe";
		uniqueItem->iconFrame = 2;
		uniqueItem->avatarFrame = 2;
		break;
	}
	case UNIQUEITEM_COMPANION_WARDEN_WEAPON:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_GREATSWORD_NICE, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_GREY_WARDEN) + "\'s Greatsword";
		uniqueItem->bonusDamageToDarkspawn = 1;
		break;
	}
	case UNIQUEITEM_COMPANION_WARDEN_ARMOR:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_HEAVY_ARMOR_PLATE, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_GREY_WARDEN) + "\'s Armor";
		uniqueItem->iconFrame = 33;
		break;
	}
	case UNIQUEITEM_COMPANION_ASSASSIN_WEAPON:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_DAGGER_KRIS, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_ASSASSIN) + "\'s Dagger";
		uniqueItem->avatarFrame = 8;
		break;
	}
	case UNIQUEITEM_COMPANION_ASSASSIN_ARMOR:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_MEDIUM_ARMOR_LEATHER, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_ASSASSIN) + "\'s Armor";
		uniqueItem->iconFrame = 32;
		uniqueItem->avatarFrame = 4;
		uniqueItem->hue = 40;
		uniqueItem->armor = 3;
		break;
	}
	case UNIQUEITEM_COMPANION_ARCHER_WEAPON:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BOW, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_BARD) + "\'s Bow";
		break;
	}
	case UNIQUEITEM_COMPANION_ARCHER_ARMOR:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_MEDIUM_ARMOR_LEATHER, 1);
		uniqueItem->itemName = GetCompanionName(COMPANION_BARD) + "\'s Armor";
		uniqueItem->hue = 75;
		break;
	}
	case UNIQUEITEM_GREATSWORD1:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_GREATSWORD_NICE, 1);
		uniqueItem->itemName = "Unique Greatsword 1";
		uniqueItem->attack = 8;
		break;
	}
	case UNIQUEITEM_GREATSWORD2:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_GREATSWORD_NICE, 1);
		uniqueItem->itemName = "Unique Greatsword 2";
		uniqueItem->attack = 8;
		break;
	}
	case UNIQUEITEM_SWORD1:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SWORD_GLADIUS, 1);
		uniqueItem->itemName = "Unique Sword 1";
		uniqueItem->attack = 6;
		break;
	}
	case UNIQUEITEM_SWORD2:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_SWORD_RELIC, 1);
		uniqueItem->itemName = "Unique Sword 2";
		uniqueItem->attack = 6;
		break;
	}
	case UNIQUEITEM_DAGGER1:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_DAGGER_KNIFE, 1);
		uniqueItem->itemName = "Unique Dagger 1";
		uniqueItem->attack = 6;
		break;
	}
	case UNIQUEITEM_DAGGER2:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_DAGGER_KRIS, 1);
		uniqueItem->itemName = "Unique Dagger 2";
		uniqueItem->attack = 6;
		break;
	}
	case UNIQUEITEM_MACE1:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_MACE_FLANGED, 1);
		uniqueItem->itemName = "Unique Mace 1";
		uniqueItem->attack = 6;
		break;
	}
	case UNIQUEITEM_MACE2:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_MACE_MORNING, 1);
		uniqueItem->itemName = "Unique Mace 2";
		uniqueItem->attack = 6;
		break;
	}
	case UNIQUEITEM_STAFF_ACOLYTES:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_STAFF_STRAIGHT, 1);
		uniqueItem->itemName = "Acolyte\'s Staff";
		uniqueItem->attack = 1;
		break;
	}
	case UNIQUEITEM_STAFF_ENCHANTERS:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_STAFF_HELIX, 1);
		uniqueItem->itemName = "Enchanter\'s Staff";
		uniqueItem->attack = 0;
		break;
	}
	case UNIQUEITEM_RING_MEMORY_BAND:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_RING, 1);
		uniqueItem->itemName = "Memory Band";
		uniqueItem->bonusExperiencePercent = 1;
		break;
	}
	case UNIQUEITEM_RING_KEEPERS_BAND:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_RING, 1);
		uniqueItem->itemName = "Keeper\'s Band";
		uniqueItem->mod_dexterity = 1;
		break;
	}
	case UNIQUEITEM_RING_OF_EVASION:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_RING, 1);
		uniqueItem->itemName = "Ring of Evasion";
		uniqueItem->defense = 1;
		break;
	}
	case UNIQUEITEM_RING_ENCHANTERS:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_RING, 1);
		uniqueItem->itemName = "Enchanter\'s Ring";
		uniqueItem->mod_magic = 1;
		break;
	}
	case UNIQUEITEM_RING_GUIDANCE:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_RING, 1);
		uniqueItem->itemName = "Guiding Band";
		uniqueItem->attack = 1;
		break;
	}
	case UNIQUEITEM_RING_TRICKSTERS_BAND:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_RING, 1);
		uniqueItem->itemName = "Trickster\'s Loop";
		uniqueItem->mod_cunning = 1;
		uniqueItem->mod_magic = 1;
		break;
	}
	case UNIQUEITEM_RING_OF_ENDURANCE:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_RING, 1);
		uniqueItem->itemName = "Endurance Ring";
		uniqueItem->mod_constitution = 1;
		uniqueItem->stamina = 10;
		uniqueItem->whoEquips = EQUIP_TO_ROGUE_ALL + EQUIP_TO_WARRIOR_ALL;
		break;
	}
	case UNIQUEITEM_AMULET_STRENGTH_OF_STONE:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMULET, 1);
		uniqueItem->itemName = "Strength of the Stone";
		uniqueItem->mod_strength = 1;
		break;
	}
	case UNIQUEITEM_AMULET_CLARITY:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMULET, 1);
		uniqueItem->itemName = "Clarity of Purpose";
		uniqueItem->mod_magic = 1;
		uniqueItem->mod_willpower = 1;
		break;
	}
	case UNIQUEITEM_AMULET_WICKED:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMULET, 1);
		uniqueItem->itemName = "Wicked Amulet";
		uniqueItem->criticalChance = 1;
		break;
	}
	case UNIQUEITEM_AMULET_PEACE_OF_MIND:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMULET, 1);
		uniqueItem->itemName = "Peace of Mind";
		uniqueItem->resistMental = 40;
		break;
	}
	case UNIQUEITEM_AMULET_LYRIUM_PENDANT:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMULET, 1);
		uniqueItem->itemName = "Lyrium Pendant";
		uniqueItem->spellpower = 2;
		uniqueItem->mana = 10;
		uniqueItem->whoEquips = EQUIP_TO_MAGE_ALL;
		break;
	}
	case UNIQUEITEM_AMULET_PUZZLE_CLASP:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMULET, 1);
		uniqueItem->itemName = "Puzzle Clasp";
		uniqueItem->mod_dexterity = 1;
		uniqueItem->mod_cunning = 1;
		break;
	}
	case UNIQUEITEM_AMULET_BLESSED_WARD:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMULET, 1);
		uniqueItem->itemName = "Blessed Ward";
		uniqueItem->defense = 3;
		break;
	}
	case UNIQUEITEM_AMULET_HEARTSTONE:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMULET, 1);
		uniqueItem->itemName = "Heartstone";
		uniqueItem->mod_constitution = 2;
		break;
	}
	case UNIQUEITEM_AMULET_WARRIORS_CHARM:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMULET, 1);
		uniqueItem->itemName = "Warrior\'s Charm";
		uniqueItem->attack = 3;
		break;
	}
	case UNIQUEITEM_BELT_DWARVEN_WARRIOR:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Dwarven Warrior\'s Belt";
		uniqueItem->mod_strength = 1;
		uniqueItem->armor = 1;
		break;
	}
	case UNIQUEITEM_BELT_DWARVEN_SMITH:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Dwarven Smith\'s Belt";
		uniqueItem->armor = 1;
		break;
	}
	case UNIQUEITEM_BELT_DWARVEN_MERCHANT:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Dwarven Merchant\'s Belt";
		uniqueItem->bonusGoldPercent = 10;
		break;
	}
	case UNIQUEITEM_BELT_HARDY:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Hardy\'s Belt";
		uniqueItem->mod_constitution = 1;
		break;
	}
	case UNIQUEITEM_BELT_FENCER:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Fencer\'s Cinch";
		uniqueItem->attack = 4;
		break;
	}
	case UNIQUEITEM_BELT_SWORD:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Sword Belt";
		uniqueItem->piercing = 2;
		break;
	}
	case UNIQUEITEM_BELT_ORNATE_LEATHER:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Ornate Leather Belt";
		uniqueItem->mod_strength = 1;
		uniqueItem->mod_willpower = 1;
		break;
	}
	case UNIQUEITEM_BELT_MAGISTER_LORDS:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Belt of the Magister Lords";
		uniqueItem->spellpower = 3;
		uniqueItem->whoEquips = EQUIP_TO_MAGE_ALL;
		break;
	}
	case UNIQUEITEM_BELT_NIMBLE_SASH:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Nimble Sash";
		uniqueItem->mod_dexterity = 2;
		break;
	}
	case UNIQUEITEM_BELT_REINFORCED:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Reinforced Girdle";
		uniqueItem->stamina = 20;
		uniqueItem->resistPhysical = 10;
		uniqueItem->whoEquips = EQUIP_TO_WARRIOR_ALL;
		break;
	}
	case UNIQUEITEM_BELT_BUCKLE_OF_THE_WINDS:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_BELT, 1);
		uniqueItem->itemName = "Buckle of the Winds";
		uniqueItem->defense = 3;
		break;
	}
	case UNIQUEITEM_AMMO_DEFAULT:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMMO, 1, ITEM_NORMAL);
		uniqueItem->itemName = "Arrows";
		break;
	}
	case UNIQUEITEM_AMMO_BARBED:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMMO, 1, ITEM_ONE_MOD);
		uniqueItem->itemName = "Barbed Arrows";
		uniqueItem->damage = 1;
		break;
	}
	case UNIQUEITEM_AMMO_PIERCING:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMMO, 1, ITEM_ONE_MOD);
		uniqueItem->itemName = "Veridium-Tipped Arrows";
		uniqueItem->piercing = 3;
		break;
	}
	case UNIQUEITEM_AMMO_FIRE:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMMO, 1, ITEM_ONE_MOD);
		uniqueItem->itemName = "Fire Arrows";
		uniqueItem->damage = 2;
		break;
	}
	case UNIQUEITEM_AMMO_ELF_MAKE:
	{
		uniqueItem->InitializeEquipment(ITEMTYPE_AMMO, 1, ITEM_ONE_MOD);
		uniqueItem->itemName = "Elf-Fletched Arrows";
		uniqueItem->attack = 3;
		break;
	}
	default:
	{
		break;
	}
	}

	if (uniqueItem)
	{
		uniqueItem->uniqueId = nTypeId;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UniqueItems: failed to create item with uniqueId %i"), nTypeId);
	}
	
	return uniqueItem;
}

