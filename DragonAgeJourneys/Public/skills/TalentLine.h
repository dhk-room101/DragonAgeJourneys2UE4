// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TalentLine.generated.h"

const int32 TALENT_TYPE_CLASS = 1;
const int32 TALENT_TYPE_SPECIALIZATION = 2;
const int32 TALENT_TYPE_RACIAL = 3;
const int32 TALENT_TYPE_DUAL_WEAPON = 4;
const int32 TALENT_TYPE_TWO_HANDED = 5;
const int32 TALENT_TYPE_ARCHERY = 6;
const int32 TALENT_TYPE_SHIELD = 7;
const int32 TALENT_TYPE_PRIMAL = 8;
const int32 TALENT_TYPE_ENTROPY = 9;
const int32 TALENT_TYPE_CREATION = 10;
const int32 TALENT_TYPE_SPIRIT = 11;
const int32 TALENT_TYPE_ORIGIN = 15;
const int32 TALENT_TYPE_OTHER = 99;
const int32 TALENT_MAGE_POWER = 509;
const int32 TALENT_MAGE_FIRE = 510;
const int32 TALENT_MAGE_COLD = 511;
const int32 TALENT_MAGE_LIGHTNING = 512;
const int32 TALENT_MAGE_STONE = 513;
const int32 TALENT_MAGE_HEAL = 515;
const int32 TALENT_MAGE_DEATH = 520;
const int32 TALENT_MAGE_PSIONICS = 521;
const int32 TALENT_WARRIOR1 = 550;
const int32 TALENT_WARRIOR2 = 551;
const int32 TALENT_ROGUE1 = 600;
const int32 TALENT_ROGUE2 = 601;
const int32 TALENT_ROGUE_STEALTH = 602;
const int32 TALENT_DUAL_WEAPON1 = 650;
const int32 TALENT_DUAL_WEAPON2 = 651;
const int32 TALENT_ARCHERY1 = 652;
const int32 TALENT_ARCHERY2 = 653;
const int32 TALENT_TWO_HANDED1 = 654;
const int32 TALENT_TWO_HANDED2 = 655;
const int32 TALENT_TWO_HANDED3 = 656;
const int32 TALENT_SHIELD1 = 660;
const int32 TALENT_SHIELD2 = 661;
const int32 TALENT_BLOODMAGE = 710;
const int32 TALENT_SPIRIT_HEALER = 720;
const int32 TALENT_ASSASSIN = 740;
const int32 TALENT_BERSERKER = 750;
const int32 TALENT_CHAMPION = 760;
const int32 TALENT_ELF_RACIAL = 940;
const int32 TALENT_DWARF_RACIAL = 950;
const int32 TALENT_HUMAN_RACIAL = 960;
const int32 TALENT_BARD = 730;
const int32 TALENT_ORIGIN_DWARF_COMMONER = 970;
const int32 TALENT_ORIGIN_DWARF_NOBLE = 971;
const int32 TALENT_ORIGIN_ELF_CITY = 972;
const int32 TALENT_ORIGIN_ELF_DALISH = 973;
const int32 TALENT_ORIGIN_HUMAN_NOBLE = 974;
const int32 TALENT_ORIGIN_MAGE = 975;
const int32 TALENT_GREY_WARDEN = 965;

UCLASS()
class UTalentLine : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void InitializeTalentLine(int32 nTreeId, int32 nUnlocked = -1);
	FString GetLineTitle(int32 nTreeId, UGameCharacter* gameCharacter = nullptr /*//DHK*/);

	int32 treeId = 0;
	int32 unlocked = 0;
	TArray<int32> skillIds;
	int32 type = 1;

private:
	virtual ~UTalentLine();
	void SetTalentLineProperties();
};



