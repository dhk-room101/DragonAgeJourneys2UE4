// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Action.generated.h"

class UGameCharacter;
class USkill;

const int32 ACTION_MOVE = 1;
const int32 ACTION_SKILL_ANIMATION = 2;
const int32 ACTION_SKILL_RESULT = 3;
const int32 ACTION_SKILL_WAIT_FOR_ANIMATION = 4;
const int32 ACTION_PASS = 5;
const int32 ACTION_RETREAT = 7;
const int32 ACTION_BARK_WAIT = 8;
const int32 ACTION_ENTER = 9;
const int32 ACTION_END_BATTLE = 10;
const int32 ACTION_WAIT = 11;

UCLASS()
class UAction : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void InitializeAction(int32 nActionType, UGameCharacter* gameChar);

	UPROPERTY()
		UGameCharacter* gameCharacter = nullptr;
	UPROPERTY()
		USkill* useSkill = nullptr;
	int32 actionType = 1;
	int32 moveToHexIndex = -1;
	int32 targetHexIndex = -1;
	int32 intArg = 0;
    bool skillMiss = false;
	bool complete = false;
	bool flank = false;
	bool backstab = false;
	bool counterattack = false;
	FVector moveToPoint;
	FString stringArg;

private:
	virtual ~UAction();

};



