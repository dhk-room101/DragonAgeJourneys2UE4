// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "STypes.h"
#include "Intelligence.generated.h"

const int32 STANCE_NEUTRAL = 0;
const int32 STANCE_AGGRESSIVE = 1;
const int32 STANCE_DEFENSIVE = 2;
const int32 STANCE_RANGED = 3;

class UGameCharacter;
class UAiAction;
class USkill;

UCLASS()
class UIntelligence : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	inline static bool ActionValueSorter(const UAiAction& action1, const UAiAction& action2)
	{
		return action1.GetActionValue() < action2.GetActionValue();
	}

	void InitializeIntelligence(UGameCharacter* gameCharacter, int32 nStance = 0);
	void TookDamageFrom(int32 nAmount, UGameCharacter* charAttacker);
	void SkillActionsUntargeted(USkill* skill);
	void SkillActionsForTarget(UGameCharacter* gameCharacter, USkill* skill);
	void MoveActionsTowards(AHex* hex);
	int32 GetHateFor(UGameCharacter* gameCharacter);
	bool ValidHexToUseSkillOn(AHex* hexStart, USkill* skill, AHex* hexEnd, int32 arg4 = -1);
	UAiAction* SelectAction();

	UPROPERTY()
		UGameCharacter* charRef = nullptr;
	UPROPERTY()
		UGameCharacter* latestAttacker = nullptr;
	UPROPERTY()
		UGameCharacter* target = nullptr;
	UPROPERTY()
		TArray<FDamageSource> damageSources;
	UPROPERTY()
		TArray<UAiAction*> possibleActions;
	int32 stance;

private:
	virtual ~UIntelligence();

	int32 GetDamageFrom(UGameCharacter* gameCharacter);
	int32 MaxRangeOfSkill(USkill* skill);

};



