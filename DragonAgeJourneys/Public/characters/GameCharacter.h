// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameCharacter.generated.h"

class UGroup;
class UParty;
class USkill;
class UEquipment;
class UEffect;
class UTalentLine;
class UIntelligence;
class ADragonAgeJourneysCharacter;
class AHex;

const int32 MONSTER_BASE_DAMAGE = 16;
const int32 MONSTER_BASE_HEALTH = 60;

const FString qkbSkill = "qkb_skill";
const FString txtSkill = "txt_skill";

UCLASS()
class UGameCharacter : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void InitializeGameCharacter(int32 nClassId, bool bIsAI);
	void DefaultEquipment();
	void DefaultEquipmentWeapon();
	void SetDefaultEquipmentTemplate();
	void ForceEquip(UEquipment* item, int32 nSlot);
	void EquipDefaultArmor(UEquipment* itemArmor);
	void EquipDefaultWeapon(UEquipment* itemWeapon, bool bActive = false);
	void TakeDamage(int32 nAmount, UGameCharacter* charAttacker = nullptr/*, AHex* hex = nullptr*/);
	void SetMonsterLevel(int32 nLevel);
	void ClearStatusEffects();
	void ClearNegativeEffects();
	void UntoggleSkills();
	void RefreshSkills();
	void BoundHealth();
	void BoundPower();
	void MouseOver(FVector vLocation, bool bTooltip = true);
	int32 GetBonusDamageToDarkspawn();
	int32 GetCriticalRate();
	int32 GetCriticalDamageMultiplier();
	int32 GetIncomingDamageMultiplier();
	int32 GetIncomingHealingMultiplier();
	int32 GetInitiative() const;
	int32 GetWillpower();
	int32 GetMagic();
	int32 GetCunning();
	int32 GetStrength();
	int32 GetConstitution();
	int32 GetDexterity();
	int32 GetBaseWillpower();
	int32 GetBaseMagic();
	int32 GetBaseCunning();
	int32 GetBaseStrength();
	int32 GetBaseConstitution();
	int32 GetBaseDexterity();
	void SetBaseWillpower(int32 nValue);
	void SetBaseMagic(int32 nValue);
	void SetBaseCunning(int32 nValue);
	void SetBaseStrength(int32 nValue);
	void SetBaseConstitution(int32 nValue);
	void SetBaseDexterity(int32 nValue);
	int32 GetSpeed();
	int32 GetAttack();
	int32 GetDefense();
	int32 GetDefenseMissile();
	int32 GetArmor();
	int32 GetPenetration();
	int32 GetDecisiveStrikePercentage();
	int32 GetHealthMax();
	int32 GetPowerMax();
	int32 GetSpellpower();
	int32 GetStealthStrength();
	int32 GetPerceptionRoll();
	int32 GetPhysicalResistance();
	int32 GetMentalResistance();
	int32 GetStunResistance();
	int32 GetSlowResistance();
	int32 GetHealthRegeneration();
	int32 GetPowerRegeneration();
	int32 GetAttackCostModifier();
	int32 GetSpellpowerForSkill(USkill* skill);
	int32 GetRace();
	void SetRace(int32 nValue);
	void InitSkills();
	bool IsMage();
	bool IsWarrior();
	bool IsRogue();
	bool IsMonster();
	bool IsHidden();
	bool IsInactive();
	bool IsLargeCreature();
	bool IsBerserk();
	bool IsStunned();
	bool IsDisoriented();
	bool IsUsingHealthForPower();
	bool IsImmuneToFlanking();
	bool HasShield();
	bool CanCounterattack();
	void SetCounterAttackReady(bool bReady);
	bool GetCounterAttackReady();
	bool AddEffect(UEffect* effect);
	bool RemoveEffectWithId(int32 nEffectId);
	bool RemoveEffectWithIndex(int32 nIndex);
	float GetBattleInitiative() const;
	int32 IncreaseHealth(int32 nAmount, bool bResurrects = false);
	int32 ReduceHealth(int32 nAmount);
	int32 IncreasePower(int32 nAmount);
	int32 ReducePower(int32 nAmount);
	int32 GetHealthPercent();
	int32 GetPowerPercent();
	int32 GetWeaponDamage();
	int32 GetOffhandDamage();
	int32 GetMonsterBaseDamage();
	int32 GetDamageMultiplier();
	int32 GetMovesLeft();
	int32 GetHealingBonus(bool bBool = true);
	bool HasSkill(int32 nSkillId, bool bPassive = false);
	bool HasEffectWithId(int32 nEffectId);
	bool Unequip(UEquipment* item);
	USkill* GetDefaultAttack();
	USkill* GetSkill(int32 nSkillId, bool bPassive = false);
	USkill* FindSkillWithId(int32 nSkillId);
	TArray<USkill*> GetSkills();

	bool isRunt = false;
	bool immortal = false;
	bool usedSkill = false;
	bool levelUpEffectPending = false;
	bool diedLastBattle = false;
	bool autoStats = true;
	bool retreated = false;
	bool scoredCritical = false;
	bool equipmentChanged = false;
	float animScale = 1.f;
	float monsterDamageMultiplier = 1.f;
	float monsterHealthMultiplier = 1.f;
	float initiativeRoll = 0.f;
	FString charName = "Character";
	FString description = "[Blank Description]";
	FString convDescription = "";
	int32 level = 1;
	int32 gender = 1;
	int32 experienceForNext = -1;
	int32 experienceForPrev = -1;
	int32 hairType = 1;
	int32 companionId = -1;
	int32 old_power = 0;
	int32 statPoints = 0;
	int32 skillPoints = 0;
	int32 base_initiative = 20;
	int32 accuracy = 92;
	int32 latestHate = 0;
	int32 hateRanking = 0;
	int32 creatureWidth = 1;//TODO not needed in 3D
	int32 creatureHeight = 1;//TODO unused?
	int32 moved = 0;
	int32 defaultWeapon = -1;
	int32 defaultArmor = -1;
	int32 defaultArmorHue = 0;
	int32 classId = 0;
	int32 preBattleHealth = 0;
	int32 health = 0;
	int32 power = 0;
	int32 skinType = 1;
	int32 skinColor = 0;
	int32 hairColor = 0;
	int32 currentWeaponSet = 1;
	UPROPERTY()
		UGroup* group = nullptr;
	UPROPERTY()
		UParty* party = nullptr;
	UPROPERTY()
		ADragonAgeJourneysCharacter* avatar = nullptr;
	UPROPERTY()
		TArray<USkill*> passiveSkills;
	UPROPERTY()
		TArray<UEquipment*> gear;
	UPROPERTY()
		TArray<UEffect*> effects;
	UPROPERTY()
		TArray<UTalentLine*> talents;
	UPROPERTY()
		UIntelligence* ai = nullptr;
	//Portrait

protected:
	int32 experience = 0;

private:
	virtual ~UGameCharacter();
	int32 GetSpellpowerBonusForSchool(int32 nType);
	TArray<USkill*> GetUnlockedTalents();

	int32 _race = 1;
	int32 _origin = -1;
	int32 _willpower = 0;
	int32 _magic = 0;
	int32 _cunning = 0;
	int32 _strength = 0;
	int32 _constitution = 0;
	int32 _dexterity = 0;
	int32 _quickness = 50;
	int32 _speed = 3;
	UPROPERTY()
		TArray<USkill*> _skills;//quickbar and battle related
	UPROPERTY()
		UEquipment* _altOffhand = nullptr;
	UPROPERTY()
		UEquipment* _altWeapon = nullptr;
	UPROPERTY()
		UEquipment* _altAmmo = nullptr;
	UPROPERTY()
		USkill* _defaultAttack = nullptr;
	UPROPERTY()
		TArray<USkill*> _allSkills;
	UPROPERTY()
		TArray<USkill*> _tempSkills;

	UPROPERTY()
		FTimerHandle tTimerDefaultWeapon;
};



