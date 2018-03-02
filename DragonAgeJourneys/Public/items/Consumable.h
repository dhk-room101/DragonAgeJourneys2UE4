// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Item.h"
#include "Consumable.generated.h"

const int32 INJURY_KIT_HEAL_PERCENT = 25;
const float PER_MAGIC_POTION_MULTIPLIER = 0.05f;

class UGameCharacter;

UCLASS()
class UConsumable : public UItem
{
	GENERATED_UCLASS_BODY()

public:
	void InitializeConsumable(int32 nTypeId = 5001 /*POTION_HEAL_SMALL*/, int32 nStack = 1);
	int32 GetHealing(UGameCharacter* gameCharacter);
	int32 GetPower(UGameCharacter* gameCharacter);
	bool UsableBy(UGameCharacter* gameCharacter, bool bSkill = false);
	virtual FString ItemStatsString(FString sStats) override;

	FString consumeSound;
	int32 restore_health_pc = 0;
	int32 restore_power_pc = 0;
	int32 restore_health = 0;
	int32 restore_power = 0;
	int32 restore_stamina = 0;
	int32 restore_mana = 0;
	int32 increase_strength = 0;
	int32 increase_dexterity = 0;
	int32 increase_constitution = 0;
	int32 increase_intellect = 0;
	int32 damage_resist = 0;
	int32 damage_bonus = 0;
	int32 poison_resist = 0;
	int32 stun_resist = 0;
	int32 quickness_bonus = 0;
	int32 stack = 1;
	bool resurrect = false;
	bool autoRevival = false;
	bool escapeItem = false;
	bool usableInBattle = true;
	bool sneaky = false;
	bool reset_stats = false;
	bool town_portal = false;
	bool campaign_combiner = false;
	bool campaign_portal = false;
	
protected:
	
private:
	virtual ~UConsumable() override;
	void SetStatsForConsumableType(int32 nTypeId);

};



