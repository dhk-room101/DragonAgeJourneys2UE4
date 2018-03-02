#pragma once

class UEquipment;

const int32 ATTACK1 = 1;
const int32 DAMAGE1 = 2;
const int32 ARMOR1 = 3;
const int32 DEFENSE1 = 4;
const int32 RANGE1 = 5;
const int32 SPELLPOWER1 = 6;
const int32 CRITICAL1 = 7;
const int32 PENETRATION1 = 8;
const int32 INITIATIVE1 = 9;
const int32 STRENGTH1 = 10;
const int32 DEXTERITY1 = 11;
const int32 CUNNING1 = 12;
const int32 WILLPOWER1 = 13;
const int32 MAGIC1 = 14;
const int32 CONSTITUTION1 = 15;
const int32 DECISIVE1 = 16;
const int32 DARKSPAWN1 = 17;
const int32 REGEN_HEALTH1 = 18;
const int32 REGEN_POWER1 = 19;
const int32 SPELLPOWER_ENTROPY1 = 20;
const int32 SPELLPOWER_CREATION1 = 21;
const int32 SPELLPOWER_SPIRIT1 = 22;
const int32 SPELLPOWER_PRIMAL1 = 23;
const int32 DEFENSE_MISSILES1 = 24;
const int32 MENTAL1 = 25;
const int32 PHYSICAL1 = 26;
const int32 FATIGUE1 = 27;
const int32 SPELLPOWER2 = 40;
const int32 RANGE2 = 41;
const int32 DEFENSE2 = 42;
const int32 ARMOR2 = 43;
const int32 DAMAGE2 = 44;
const int32 ATTACK2 = 45;
const int32 CRITICAL2 = 46;
const int32 STRENGTH2 = 47;
const int32 DEXTERITY2 = 48;
const int32 DEBUG1 = 99;

void ModifierApplyTo(int32 nModifier, UEquipment* equipment); 
FString ModifierGetName(int32 nModifierType, int32 nItemType);
int32 GetModifierForItem(int32 nTypeId, int32 nLevel = 1);
TArray<int32> GetListForItem(int32 nTypeId);