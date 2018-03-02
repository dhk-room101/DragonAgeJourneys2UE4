#pragma once

#include <UnrealString.h>

class UGameCharacter;

TArray<FString> Names = { "Military_Leader", "lowly_soldier", "bandit", "NPC_Warden", "NPC_Healer", "NPC_Bard", "noble", "crime_lord_commons", "armorer", "merchant_commons", "innkeeper_commons", "captain", "armorer_dust", "merchant_dust", "innkeeper_dust", "punk_dust", "crime_lord_dust", "Emissary", "potion_vendor_commons", "Blank", "origin_human_noble_leader", "origin_human_noble_advisor", "origin_human_noble_advisor2", "origin_elf_dalish_brother", "origin_elf_dalish_bodyguard2", "origin_elf_city_sister", "origin_elf_city_merchant", "origin_elf_city_bodyguard", "origin_dwarf_noble_brother", "origin_dwarf_noble_brother2", "origin_mage_mentor", "origin_mage_warrior", "origin_dwarf_commoner_fwarrior" };
const FString MILITARY_COMMANDER_NAME = "Andrin";
const FString LOWLY_SOLDIER_NAME = "Pilgar";
const FString BANDIT_NAME = "Feran";
const FString NOBLE_NAME = "Belmar";
const FString ARMORER_COMMONS_NAME = "Dwif";
const FString MERCHANT_COMMONS_NAME = "Dwaf";
const FString INNKEEPER_COMMONS_NAME = "Durant";
const FString ARMORER_DUST_NAME = "Leewick";
const FString MERCHANT_DUST_NAME = "Danan";
const FString INNKEEPER_DUST_NAME = "Emlyn";
const FString PUNK_DUST_NAME = "Torin";
const FString POTION_VENDOR_COMMONS_NAME = "Arteme";
const FString CAPTAIN_NAME = "Captain Gareth";
const FString CRIME_LORD_NAME = "Saria";

bool IsValidNPCId(const FString sID);
UGameCharacter* GetTutorialAlly(int32 nPartyIndex);