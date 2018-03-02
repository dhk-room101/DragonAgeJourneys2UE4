// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Party.generated.h"

class UGameCharacter;
class UQuest;
class UGroup;
class UItem;

const int32 AUTO_INJURY_KIT_DEATH = 1;
const int32 AUTO_INJURY_KIT_OBTAINED = 2;
const int32 ORZ_EXIT_LOC_COMMONS = 1;
const int32 ORZ_EXIT_LOC_DUST = 2;
const int32 FACING_RIGHT = 0;
const int32 FACING_LEFT = 1;
const int32 max_size = 4;
const int32 MAX_OVERFLOW = 0;
const int32 COPPER_PER_SILVER = 100;
const int32 SILVER_PER_GOLD = 100;
const int32 COPPER_PER_GOLD = 10000;

UCLASS()
class UParty : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
		TArray<UGameCharacter*> members;
	UPROPERTY()
		TArray<UQuest*> quests;
	UPROPERTY()
		TMap<FString, bool> enemiesDefeated;
	UPROPERTY()
		TMap<FString, bool> hexesLooted;
	UPROPERTY()
		TMap<FString, bool> recoveryUsed;
	UPROPERTY()
		TMap<FString, bool> conversations;
	UPROPERTY()
		TMap<int32, UGameCharacter*> companions;
	FString locationId;
	FString areaString;
	float version;
	int32 orzExitLocation = 1;

	int32 GetLevel();
	int32 GetSneaking();
	int32 GetDifficulty();
	bool HasQuest(int32 nQuestId, bool bRepeat = false);
	bool HasFinishedQuest(int32 nQuestId);
	bool AddToInventory(UItem* item);
	bool RemoveFromInventory(UItem* item);
	UGameCharacter* GetCompanionByClass(int32 nClassId, bool bMonster = true);
	UGameCharacter* GetHeroCharacter();
	UGroup* GetGroup();


private:
	virtual ~UParty();
	int32 _difficulty = 1;
	int32 _sneaking = 0;

};



