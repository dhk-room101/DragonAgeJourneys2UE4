// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Quest.generated.h"

const int32 FLIGHT_TO_ORZAMMAR = 1;
const int32 MISSING_WARDEN1 = 2;
const int32 MISSING_WARDEN2 = 3;
const int32 MEET_THE_HEALER = 4;
const int32 RUNAROUND_ARMOR_RECOVER = 5;
const int32 RUNAROUND_ARMOR_RETURN = 6;
const int32 RUNAROUND_NOBLE_MEET = 7;
const int32 RUNAROUND_GENLOCK_GATHER = 8;
const int32 RUNAROUND_GENLOCK_RETURN = 9;
const int32 RUNAROUND_CAPTAIN_MEET = 10;
const int32 RUNAROUND_CRIMELORD_MEET = 11;
const int32 RUNAROUND_DIAMONDS_FIND = 12;
const int32 RUNAROUND_CRIMELORD_RETURN = 13;
const int32 EXCAVATION1 = 20;
const int32 EXCAVATION2 = 21;
const int32 EXCAVATION3 = 22;
const int32 EXCAVATION_DESTROYED1 = 24;
const int32 EXCAVATION_DESTROYED2 = 25;
const int32 RUNAROUND_BEGIN = 27;
const int32 COMMONS_POTIONS_QUEST_SEARCH = 100;
const int32 COMMONS_POTIONS_QUEST_RETURN = 101;
const int32 CLEAR = 1;
const int32 EXPLORE = 2;
const int32 BOSS = 3;
const int32 ITEM = 4;
const int32 RESCUE = 5;
const int32 CONVERSATION = 6;
const int32 SCRIPTED = 7;
const int32 LOOT = 8;
const int32 GROUP_NONE = 0;
const int32 GROUP_ORZAMMAR_COMMONS = 1;
const int32 GROUP_ORZAMMAR_DUSTTOWN = 2;
const int32 GROUP_DEEPROADS = 3;
const int32 GROUP_COMPANIONS = 5;

UCLASS()
class UQuest : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual ~UQuest();

public:
	int32 questId = -1;
	bool completed = false;
};
