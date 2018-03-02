// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Item.generated.h"

class UGameCharacter;

const int32 ITEM_NORMAL = 0;
const int32 ITEM_ONE_MOD = 1;
const int32 ITEM_TWO_MODS = 2;
const int32 ITEM_THREE_MODS = 3;
const int32 ITEM_FOUR_MODS = 4;
const int32 ITEM_ORANGE = 5;
const int32 ITEM_QUEST = 6;
const int32 ITEM_VALUE_INCREASE_PER_LEVEL = 20;

UCLASS()
class UItem : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	virtual ~UItem();
	virtual FString ItemStatsString(FString sStats);

	int32 itemLevel = 1;
	int32 itemRarity = 0;
	int32 typeId = 0;
	int32 uniqueId = 0;
	int32 questId = -1;
	int32 iconFrame = 1;
	int32 bag = 1;
	int32 temporaryValue = 0;
	int32 activatedEffectId = 0;
	int32 passiveEffectId = 0;
	int32 attackEffectId = 0;
	int32 defenseEffectId = 0;
	int32 priceModifier = 0;
	float attackEffectPercent = 0;
	float defenseEffectPercent = 0;
	bool shopItem = false;
	bool newish = false;
	FString itemName = "Item";
	FString creator = "";
	FString lore = "";
	UPROPERTY()
		UGameCharacter* playerOwner = nullptr;

private:
};



