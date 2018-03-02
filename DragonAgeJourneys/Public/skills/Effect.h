// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Effect.generated.h"

class UGameCharacter;

const int32 VFX_FLAMING_WEAPON = 1;
const int32 VFX_STEALTH = 2;
const int32 VFX_WEB = 3;
const int32 VFX_FORCE_FIELD = 4;
const int32 VFX_BERSERK = 5;
const int32 EFFECT_FIRE = 1;
const int32 EFFECT_COLD = 2;
const int32 EFFECT_HOLY = 3;
const int32 EFFECT_CURSE = 4;
const int32 EFFECT_MAGIC = 5;
const int32 EFFECT_NATURE = 6;
const int32 EFFECT_PHYSICAL = 7;
const int32 EFFECT_POISON = 8;
const int32 EFFECT_LIGHTNING = 9;
const int32 EFFECT_MENTAL = 10;
const int32 EFFECT_BLOOD = 11;
const int32 EFFECT_TYPE_INSTANT = 1;
const int32 EFFECT_TYPE_PASSIVE = 2;
const int32 EFFECT_TYPE_PERIODIC = 3;
const int32 EFFECT_DURATION_NEXT_START = 1000;
const int32 EFFECT_DURATION_NEXT_END = 1001;
const int32 EFFECT_DURATION_BATTLE = 1002;
const int32 EFFECT_DURATION_SPECIAL = 1003;
const int32 EFFECT_COUNTER_ATTACK = 1;
const int32 EFFECT_FLAMING_WEAPON = 50;
const int32 EFFECT_CLERIC_HOLY_SHIELD = 100;
const int32 EFFECT_CLERIC_HEAL = 101;
const int32 EFFECT_CLERIC_REVIVE = 102;
const int32 EFFECT_CLERIC_ANOINT = 103;
const int32 EFFECT_CLERIC_FLASH = 104;
const int32 EFFECT_CLERIC_PURIFY = 105;
const int32 EFFECT_MAGE_ARMOR = 200;
const int32 EFFECT_MAGE_PRISON = 201;
const int32 EFFECT_MAGE_INVISIBILITY = 202;
const int32 EFFECT_OVERRUN = 250;
const int32 EFFECT_TAUNT = 251;
const int32 EFFECT_THREATEN = 252;
const int32 EFFECT_DEATH_BLOW = 253;
const int32 EFFECT_DISENGAGE = 254;
const int32 EFFECT_PRECISE_STRIKING = 255;
const int32 EFFECT_WAR_CRY = 256;
const int32 EFFECT_RALLY = 257;
const int32 EFFECT_INDOMITABLE = 258;
const int32 EFFECT_POWERFUL_SWINGS = 259;
const int32 EFFECT_MIGHTY_BLOW = 260;
const int32 EFFECT_SHIELD_DEFENSE = 270;
const int32 EFFECT_SHIELD_COVER = 271;
const int32 EFFECT_FEIGN_DEATH = 300;
const int32 EFFECT_STEALTH = 301;
const int32 EFFECT_DIRTY_FIGHTING = 302;
const int32 EFFECT_BELOW_THE_BELT = 303;
const int32 EFFECT_DUAL_DOUBLE_STRIKE = 401;
const int32 EFFECT_DUAL_CRIPPLE = 402;
const int32 EFFECT_MOMENTUM = 403;
const int32 EFFECT_AIM = 503;
const int32 EFFECT_DEFENSIVE_FIRE = 504;
const int32 EFFECT_PINNING_SHOT = 505;
const int32 EFFECT_CRIPPLING_SHOT = 506;
const int32 EFFECT_SUPPRESSING_FIRE = 507;
const int32 EFFECT_DISTRACTION = 600;
const int32 EFFECT_SONG_VALOR = 601;
const int32 EFFECT_SONG_COURAGE = 602;
const int32 EFFECT_SONG_CAPTIVATE = 603;
const int32 EFFECT_BERSERK = 604;
const int32 EFFECT_BLOOD_MAGIC = 608;
const int32 EFFECT_BLOOD_WOUND = 609;
const int32 EFFECT_BLOOD_CONTROL = 610;
const int32 EFFECT_MARK_OF_DEATH = 615;
const int32 EFFECT_MIND_BLAST = 700;
const int32 EFFECT_FORCE_FIELD = 701;
const int32 EFFECT_DEATH_MAGIC = 702;
const int32 EFFECT_POISON_DRAKE = 1050;
const int32 EFFECT_POISON_DRAKE_AE = 1051;
const int32 EFFECT_CORROSIVE_SPIT = 1055;
const int32 EFFECT_GENLOCK_SHIELD_SELF = 1070;
const int32 EFFECT_GENLOCK_SHIELD_ALLY = 1071;
const int32 EFFECT_GENLOCK_ACID_VIAL = 1072;
const int32 EFFECT_HURLOCK_DEEP_WOUNDS = 1073;
const int32 EFFECT_HURLOCK_BERSERK = 1075;
const int32 EFFECT_HEADSHOT = 1074;
const int32 EFFECT_SHRIEK_SCREAM = 1084;
const int32 EFFECT_SPIDER_POISON = 1092;
const int32 EFFECT_SPIDER_WEB = 1093;
const int32 EFFECT_SHOCKWAVE = 1110;
const int32 EFFECT_WEAKNESS = 1111;
const int32 EFFECT_NO_POWER_REGEN = 5000;
const int32 EFFECT_REMOVE_NEGATIVE_EFFECTS = 5001;
const int32 EFFECT_DIZZY = 5005;
const int32 EFFECT_TIRED_3 = 4000;

UCLASS()
class UEffect : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void InitializeEffect(int32 nEffectId, UGameCharacter* charCaster, UGameCharacter* charTarget, int32 nLevel = -1);
	void ApplyTo(UGameCharacter* gameCharacter);

	bool beneficial = false;
	bool active = true;
	bool createdThisTurn = true;
	bool suppressFloatingNotification = false;
	bool removesNegativeEffects = false;
	bool invulnerable = false;
	bool powerDrain = false;
	bool hidden = false;
	bool nonLethal = false;
	bool onlyDamagesUndead = false;
	bool cancelPowerRegen = false;
	bool cancelHealthRegen = false;
	bool forceRetreat = false;
	bool resurrects = false;
	bool ignoresArmor = false;
	bool cannotDie = false;
	bool mustTargetCaster = false;
	bool randomFacing = false;//TODO random angle
	bool cancelCriticals = false;
	bool cancelBackstabs = false;
	bool stun = false;
	bool disorient = false;
	bool skillsCostHealthInsteadOfPower = false;
	bool giveOffhandFullDamage = false;
	bool preventsRanged = false;
	int32 duration = 0;
	int32 currentStep = 0;
	int32 resistPoison = 0;
	int32 resistStun = 0;
	int32 resistSlow = 0;
	int32 damageResist = 0;
	int32 damageReflect = 0;
	int32 healthRegen = 0;
	int32 powerRegen = 0;
	int32 accuracy = 0;
	int32 initiativeModifier = 0;
	int32 enemyHateModifier = 0;
	int32 healthDelta = 0;
	int32 powerDelta = 0;
	int32 woundsRestorePower = 0;
	int32 lethality = 0;
	int32 vampiric_percent = 0;
	int32 stun_percent = 0;
	int32 damageBonusVsCreatureType = 0;
	int32 weaponDamageModifier = 0;
	int32 armorPenetrationModifier = 0;
	int32 criticalChanceModifier = 0;
	int32 movementModifier = 0;
	int32 attackModifier = 0;
	int32 defenseModifier = 0;
	int32 defenseMissileModifier = 0;
	int32 armorModifier = 0;
	int32 attackPowerCostModifier = 0;
	int32 vfx = -1;
	int32 effectId = 0;
	int32 level = 1;
	int32 source = 1;
	int32 iconFrame = 1;
	int32 type = 1;
	int32 maxStep = 0;
	int32 removesSourceType = 0;
	float damageMultiplier = 1.f;
	float undeadMultiplier = 0.f;
	float damageBonusVsCreatureModifier = 1.f;
	float incomingHealingMultiplier = 1.f;
	float incomingDamageMultiplier = 1.f;
	float criticalDamageMultiplier = 1.f;
	float attackPowerCostMultiplier = 1.f;
	UPROPERTY()
		UGameCharacter* caster = nullptr;
	UPROPERTY()
		UGameCharacter* target = nullptr;

private:
	virtual ~UEffect();
	void SetEffectProperties();
};



