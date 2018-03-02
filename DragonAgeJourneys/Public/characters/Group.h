// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "STypes.h"
#include "Group.generated.h"

class UGameCharacter;
class UPartyPosition;
class AMapHex;

const int32 vs_emissary_mine = 1000;
const int32 vs_carta_thieves = 1001;
const int32 vs_thaig_golem1 = 1002;
const int32 vs_lake_workgroup1 = 1003;
const int32 vs_lake_workgroup2 = 1004;
const int32 vs_lake_workgroup3 = 1005;
const int32 vs_thaig_golem2 = 1006;
const int32 vs_tut_emissary = 1007;
const int32 vs_thaig_spiders1 = 1008;
const int32 vs_thaig_spiders2 = 1009;
const int32 vs_thaig_spiders3 = 1010;
const int32 vs_deepstalkers_1 = 1011;
const int32 vs_warden_guards = 1012;
const int32 vs_warden_patrol = 1013;
const int32 vs_warden_ambush = 1014;
const int32 vs_genlock_test = 1015;
const int32 vs_test_ogre = 1016;
const int32 vs_spiders = 1017;
const int32 vs_pair = 1018;
const int32 vs_final = 1019;
const int32 vs_test = 1020;
const int32 vs_hurlocks = 1021;
const int32 vs_patrol = 1022;
const int32 vs_spiderama = 1023;
const int32 quest_test_spiders = 1024;
const int32 vs_tut_genlock = 1025;


UCLASS()
class UGroup : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void InitializeGroup(TArray<int32> tChars);
	void Add(UGameCharacter* uGameChar);
	bool IsDefeated();
	bool GetDefeated();
	UGroup* GenerateGroup(FString sGroup);
	TArray<int32> GetBattlePositions();

	FMapEncounterStruct encounter_data;
	UPROPERTY()
		UPartyPosition* groupPosition = nullptr;
	UPROPERTY()
		TArray<UGameCharacter*> chars;
	UPROPERTY()
		TArray<AMapHex*> pursuitRoute;
	UPROPERTY()
		AMapHex* mapHex = nullptr;
	UPROPERTY()
		AMapHex* pursuitHex = nullptr;
	int32 level = 1;
	int32 aggroRadius = 2;
	bool displayed = false; //TODO Needed?
	bool pursuing = false; //TODO Needed?
	bool player = false;
	bool overrideDefeated = false;
	FString preventFightWithBarkId;
	FString preventFightWithDialogueId;
	FString id;

private:
	virtual ~UGroup();

	int32 GetGroupIntFromString(FString sGroup);

};



