// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Skill.generated.h"

class UGameCharacter;
class UTalentLine;
class UConsumable;
class UAnimationAsset;

const int32 SKILL_TYPE_TARGETED = 1;
const int32 SKILL_TYPE_TOGGLED = 2;
const int32 SKILL_TYPE_PASSIVE = 3;
const int32 SKILL_TYPE_INSTANT = 4;
const int32 TEMPLATE_HORIZONTAL4 = 1;
const int32 TEMPLATE_CONE = 2;
const float SPELLPOWER_BONUS_PC_DAMAGE = 0.03f;
const float SPELLPOWER_BONUS_PC_HEALING = 0.02f;

UCLASS()
class USkill : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void InitializeSkill(int32 nSkillId, UGameCharacter* uGameChar);
	UAnimationAsset* GetTargetAnimation();
	bool IsUnlocked();
	bool IsUsable();
	bool MeetsEquipmentRequirements();
	bool ShowDirection();
	int32 GetTalentIndex();
	int32 GetMinimumEnemyDistance();
	void SetRange(int32 nAmount);
	int32 GetRange();
	void SetMinimumRange(int32 nAmount);
	int32 GetMinimumRange();
	void SetPowerCost(int32 nAmount);
	int32 GetPowerCost();
	void SetBaseHealing(int32 nAmount);
	int32 GetBaseHealing();
	void SetPowerDelta(int32 nAmount);
	int32 GetPowerDelta();
	int32 GetHealing();
	int32 GetDamage();
	bool IsSpell();
	bool IsWeaponAttack();
	bool GetAlwaysHits();
	FString GetSkillDescription();

	int32 skillId = 0;
	int32 iconFrame = 1;
	int32 shortcutSlot = -1;
	int32 type = 1;
	int32 addsCooldown = 0;
	int32 currentCooldown = 0;
	int32 overrideCursor = -1;
	int32 auraEffectId = 0;
	int32 auraRadius = 0;
	int32 areaEffectRadius = 0;
	int32 arcRadius = 0;
	int32 knockback = 0;
	int32 characterAnimation = -1;
	int32 aiCastLimit = -1;
	int32 aiCastCount = 0;
	int32 aiPriority = 5;
	int32 targetsTemplate = 0;
	int32 passiveMovementIncrease = 0;
	int32 passiveCriticalChanceIncrease = 0;
	int32 passivePhysicalResistance = 0;
	int32 passiveMentalResistance = 0;
	int32 passiveAttackValueBonus = 0;
	int32 passiveDefenseValueBonus = 0;
	int32 passiveArmorValueBonus = 0;
	int32 passiveSpellpowerBonus = 0;
	int32 damageType = 7;
	int32 powerCost = 0;
	int32 healthCost = 0;
	int32 damage = 0;
	int32 _healing = 0;
	int32 _powerDelta = 0;
	int32 armorPenetrationBonus = 0;
	int32 lethality = 0;
	int32 vampiric_percent = 0;
	int32 damageBonusVsCreatureType = 0;
	int32 movementBonus = 0;
	int32 attacksMultipleTimes = 1;
	int32 extraSpeedAttacks = 0;
	int32 targetEffectId = 0;
	int32 selfEffectId = 0;
	int32 critsAgainstTargetsWithEffectId = 0;
	int32 removesSourceType = 0;
	int32 onTargetKillSelfEffectId = 0;
	int32 onTargetHitSelfEffectId = 0;
	float monsterDamageScale = 0.f;
	float splashDamageMultiplier = 0.f;
	float passiveHealthMultiplier = 1.f;
	float passiveDamageMultiplier = 1.f;
	float undeadMultiplier = 0.f;
	float damageBonusVsCreatureModifier = 1.f;
	float userConvertsDamageToHealthRatio = 0.f;
	float scaleWeaponDamage = 1.f;
	float magicStrength = 0.f;
	float addsPowerToDamageRatio = 0.f;
	float attacksWithAccuracyMod = 1.f;
	FString skillname = "Skill";
	FString sound_actionFrame = "";
	FString sound_begin = "";
	FString sound_impact = "";
	FString sound_miss = "";
	bool nonCombat = false;
	bool toggledOn = false;
	bool friendlyFire = true;
	bool toggleEndsTurn = false;
	bool moveOnly = false;
	bool automaticCritical = false;
	bool untogglesSongs = false;
	bool requiresHorizontalLine = false;
	bool requiresBehindTarget = false;
	bool playCharacterAttackGrunt = false;
	bool actionSoundEvenOnMiss = false;
	bool accelerateAnimation = false;
	bool spawnsProjectile = false;
	bool spawnsBeam = false;
	bool spawnsTrap = false;
	bool drawsBlood = true;
	bool aiNotUnlessHurt = false;
	bool aiNotUnlessLowPower = false;
	bool aiNotAlone = false;
	bool aiAllowSuicide = false;
	bool aiTargetOnlyLowHealth = false;
	bool aiAlwaysUse = false;
	bool aiOnlyWhenHordeDead = false;
	bool aiDesireAllyProximity = false;
	bool aiNotWithMove = false;
	bool aiOnlyWhenNearlyDead = false;
	bool requiresWeapon = false;
	bool requiresMelee = false;
	bool requiresRanged = false;
	bool requiresShield = false;
	bool requiresDualWield = false;
	bool requiresTwoHander = false;
	bool requiresMediumArmor = false;
	bool unarmedAttack = false;
	bool requiresLowHealth = false;
	bool rangedAttack = false;
	bool directAttack = false;
	bool usesWeaponRange = false;
	bool targetsFriendlies = false;
	bool targetsDead = false;
	bool targetsAllCharacters = false;
	bool targetsSelf = false;
	bool targetsEmpty = false;
	bool hitsAllEmpty = false;
	bool hitsAllFriendlies = false;
	bool hitsAllEnemies = false;
	bool hitsAllMeleeRange = false;
	bool piercesColumn = false;
	bool splashHitsAllies = false;
	bool doesNotEffectSelf = false;
	bool doesNotEffectMages = false;
	bool requiresVerticalGap = false;
	bool targetsFriendlyFrontRow = false;
	bool targetsStunned = false;
	bool targetsSummoned = false;
	bool pointBlankAE = false;
	bool resurrects = false;
	bool powerDrain = false;
	bool nonLethal = false;
	bool onlyDamagesUndead = false;
	bool forceRetreat = false;
	bool ignoresArmor = false;
	bool specialMovementSkill = false;
	bool enablesOwnCounterAttack = false;
	bool disablesOwnCounterAttack = false;
	bool cannotBeCountered = false;
	bool addWeaponDamage = false;
	bool clonesPlayers = false;
	bool summonsWhirlwind = false;
	bool shufflesEnemies = false;
	bool swapPositions = false;
	bool costsRemainingPower = false;
	bool makeDamageSponge = false;
	bool removesPoison = false;
	bool intimidating = false;
	bool allyBonusAttack = false;
	bool attacksTwoTargets = false;
	bool killsUser = false;
	bool hasCasterAnimation = false;
	bool hasTargetAnimation = false;
	bool targetPointAnimation = false;
	bool showWeaponAnimation = false;
	bool suppressStunAnim = false;
	bool suppressTargetFacing = false;
	bool adjustDepthToBehind = false;
	bool ignoreActionFrame = false;
	UPROPERTY()
		UGameCharacter* char_ref = nullptr;
	UPROPERTY()
		UConsumable* consumable = nullptr;
	UPROPERTY()
		UTalentLine* talentLine = nullptr;

private:
	virtual ~USkill();
	void SetSkillProperties();
	void SetArrowSounds();
	bool AiFlagsOkay();

	int32 _range = 1;
	int32 _minimumRange = 0;
	int32 _minimumEnemyDistance = -1;
	int32 requiresStrength = 0;
	int32 requiresDexterity = 0;
	int32 requiresWillpower = 0;
	int32 requiresMagic = 0;
	int32 requiresIntelligence = 0;
	int32 requiresCunning = 0;
	int32 requiresConstitution = 0;
	bool _alwaysHits = false;
};



