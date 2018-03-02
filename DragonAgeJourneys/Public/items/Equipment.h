// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Item.h"
#include "Equipment.generated.h"

const int32 NUM_SLOTS = 8;//TODO Move to slot.h
const int32 GEAR_SLOTS = NUM_SLOTS;
const int32 EQUIP_TO_WARRIOR = 1;
const int32 EQUIP_TO_ROGUE = 2;
const int32 EQUIP_TO_MAGE = 4;
const int32 EQUIP_TO_BERSERKER = 8;
const int32 EQUIP_TO_CHAMPION = 16;
const int32 EQUIP_TO_BARD = 32;
const int32 EQUIP_TO_ASSASSIN = 64;
const int32 EQUIP_TO_BLOODMAGE = 128;
const int32 EQUIP_TO_SPIRITHEALER = 256;
const int32 EQUIP_TO_WARRIOR_ALL = EQUIP_TO_WARRIOR + EQUIP_TO_BERSERKER + EQUIP_TO_CHAMPION;
const int32 EQUIP_TO_MAGE_ALL = EQUIP_TO_MAGE + EQUIP_TO_BLOODMAGE + EQUIP_TO_SPIRITHEALER;
const int32 EQUIP_TO_ROGUE_ALL = EQUIP_TO_ROGUE + EQUIP_TO_BARD + EQUIP_TO_ASSASSIN;
const int32 EQUIP_TO_ALL = EQUIP_TO_WARRIOR_ALL + EQUIP_TO_ROGUE_ALL + EQUIP_TO_MAGE_ALL;

const int32 dagger_base_damage = 16;
const int32 sword_base_damage = 18;
const int32 mace_base_damage = 17;
const int32 two_hander_base_damage = 27;
const int32 bow_base_damage = 14;
const int32 staff_base_damage = 13;
const int32 RANGE_BOW_DEFAULT = 8;
const int32 RANGE_STAFF_DEFAULT = 6;
const int32 VFX_GLOW_WEAK_BLUE = 1;
const int32 VFX_GLOW_WEAK_GREEN = 2;

const float FATIGUE_MULTIPLIER_HEAVY_ARMOR = 1.15;
const float FATIGUE_MULTIPLIER_HEAVY_SHIELD = 1.1;
const float FATIGUE_MULTIPLIER_MEDIUM_SHIELD = 1.05;

UCLASS()
class UEquipment : public UItem
{
	GENERATED_UCLASS_BODY()

public:
	void InitializeEquipment(int32 param1, int32 param2 = 1, int32 param3 = 0, int32 param4 = 0, int32 param5 = 0);
	bool IsShield();
	bool IsWeapon();
	bool IsHeavyArmor();
	bool IsMediumArmor();
	bool IsArmor();
	bool IsMelee();
	bool IsRanged();
	bool IsTwoHander();
	bool IsTwoHanded();
	bool IsStaff();
	bool IsOffHand();

	bool isDefault = false;
	bool quickRetreat = false;
	float valueMod = 1;
	float damage = 0;
	float piercing = 0;
	float criticalChance = 0;
	float fatigue_mod = 0;
	int32 modifier1 = 0;
	int32 modifier2 = 0;
	int32 modifier3 = 0;
	int32 modifier4 = 0;
	int32 material = -1;
	int32 enchantmentType = 0;
	int32 enchantmentValue = 0;
	int32 slot = 0;
	int32 defense = 0;
	int32 defenseMissile = 0;
	int32 attack = 0;
	int32 vfx = -1;
	int32 mod_strength = 0;
	int32 mod_dexterity = 0;
	int32 mod_willpower = 0;
	int32 mod_magic = 0;
	int32 mod_cunning = 0;
	int32 mod_constitution = 0;
	int32 mod_quickness = 0;
	int32 spellpower = 0;
	int32 spellpower_creation = 0;
	int32 spellpower_spirit = 0;
	int32 spellpower_primal = 0;
	int32 spellpower_entropy = 0;
	int32 stamina = 0;
	int32 mana = 0;
	int32 resistPhysical = 0;
	int32 resistMental = 0;
	int32 resistPoison = 0;
	int32 resistStun = 0;
	int32 bonusExperiencePercent = 0;
	int32 bonusGoldPercent = 0;
	int32 decisiveStrikePercentage = 0;
	int32 bonusDamageToDarkspawn = 0;
	int32 vampiric_percent = 0;
	int32 causes_stun_percent = 0;
	int32 poisonDamage = 0;
	int32 blind = 0;
	int32 lethality = 0;
	int32 powerRegen = 0;
	int32 healthRegen = 0;
	int32 accuracyBonus = 0;
	int32 damageReflection = 0;
	int32 replenishing = 0;
	int32 mod_healing = 0;
	int32 range = 1;
	int32 avatarFrame = 0;
	int32 hue = 0;
	int32 saturation = 0;
	int32 armor = 0;
	int32 whoEquips = -1;
	TArray<int32> modifiers;

protected:
	void SetStatsForEquipmentType(int32 nTypeId);

private:
	virtual ~UEquipment() override;
};



