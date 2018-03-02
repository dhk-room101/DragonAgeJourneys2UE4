#include "Locations.h"
#include "NPC.h"
#include "Scripting.h"
#include "Party.h"
#include "Map.h"
#include "Quest.h"
#include "Companions.h"
#include "DialogueNode.h"
#include "Classes.h"

bool IsConversationLocation(FString sLocationName)
{
	if (IsValidNPCId(sLocationName))
	{
		return true;
	}
	switch (GetLocationIntFromString(sLocationName))
	{
	case orz_main_entrance:
	{
		//TODO if (GetParty()->HasFinishedQuest(RUNAROUND_CRIMELORD_RETURN)) return false;
		return true;
	}
	case q2m1_entrance:
	case q3m1_entrance:
	case q4m1_entrance:
	case military_placeholder:
	case petty_officer:
	case carta_thieves:
	case lowly_soldier:
	case captain:
	case crime_lord_commons:
	case q3m2_camp:
	case merchant_commons:
	case armorer:
	case q4m1_dead_smugglers:
	case armorer_dust:
	case innkeeper_commons:
	case innkeeper_dust:
	case crime_lord_dust:
	case merchant_dust:
	case punk_dust:
	case dust_to_quests:
	case orz_bard:
	case orz_healer:
	case orz_warden:
	case potion_vendor_commons:
	{
		return true;
	}
	case -1:
	default:
		break;
	}

	return false;
}

bool IsCityLocation(FString areaString)
{
	if (areaString == "orz_commons" ||
		areaString == "orz_dusttown") return true;

	return false;
}

TArray<FString> GetDestination(FString sLocationName)
{
	switch (GetDestinationIntFromString(sLocationName))
	{
	case tut_exit:
	{
		TArray<FString> Destinations = { "q1m1", "q1m1_entrance" }; return Destinations;
	}
	case q2m1_exit:
	{
		TArray<FString> Destinations = { "q2m2", "q2m2_entrance" }; return Destinations;
	}
	case q2m2_entrance:
	{
		TArray<FString> Destinations = { "q2m1", "q2m1_exit" }; return Destinations;
	}
	case q2m2_exit:
	{
		TArray<FString> Destinations = { "q2m3", "q2m3_entrance" }; return Destinations;
	}
	case q2m3_entrance:
	{
		TArray<FString> Destinations = { "q2m2", "q2m2_exit" }; return Destinations;
	}
	case dust_to_common:
	{
		TArray<FString> Destinations = { "orz_commons", "common_to_dust" }; return Destinations;
	}
	case common_to_dust:
	{
		TArray<FString> Destinations = { "orz_dusttown", "dust_to_common" }; return Destinations;
	}
	case q4m1_to_q4m2:
	{
		TArray<FString> Destinations = { "q4m2", "q4m2_entrance" }; return Destinations;
	}
	case q4m1_to_q4m3:
	{
		TArray<FString> Destinations = { "q4m3", "q4m3_entrance" }; return Destinations;
	}
	case q4m1_to_q4m4:
	{
		TArray<FString> Destinations = { "q4m4", "q4m4_entrance" }; return Destinations;
	}
	case q4m2_entrance:
	{
		TArray<FString> Destinations = { "q4m1", "q4m1_to_q4m2" }; return Destinations;
	}
	case q4m3_entrance:
	{
		TArray<FString> Destinations = { "q4m1", "q4m1_to_q4m3" }; return Destinations;
	}
	case q4m4_entrance:
	{
		TArray<FString> Destinations = { "q4m1", "q4m1_to_q4m4" }; return Destinations;
	}
	case q1m1_exit:
	{
		TArray<FString> Destinations = { "", "" }; return Destinations;
	}
	case tunnel_entrance:
	case q3m2_entrance:
	case transitionToOrz:
	{
		if (GetParty()->orzExitLocation == ORZ_EXIT_LOC_COMMONS)
		{
			TArray<FString> Destinations = { "orz_commons", "orz_main_entrance" }; return Destinations;
		}
		if (GetParty()->orzExitLocation == ORZ_EXIT_LOC_DUST)
		{
			TArray<FString> Destinations = { "orz_dusttown", "dust_to_quests" }; return Destinations;
		}
		TArray<FString> Destinations = { "orz_commons", "orz_main_entrance" }; return Destinations;
	}
	case transitionToQ1M1:
	{
		TArray<FString> Destinations = { "q1m1", "q1m1_entrance" }; return Destinations;
	}
	case transitionToQ2M1:
	{
		TArray<FString> Destinations = { "q2m1", "q2m1_entrance" }; return Destinations;
	}
	case transitionToQ3M1:
	{
		TArray<FString> Destinations = { "q3m1_genlock", "tunnel_entrance" }; return Destinations;
	}
	case transitionToQ3M2:
	{
		TArray<FString> Destinations = { "q3m2_bandits", "q3m2_entrance" }; return Destinations;
	}
	case transitionToDustTown:
	{
		TArray<FString> Destinations = { "orz_dusttown", "dust_to_common" }; return Destinations;
	}
	case transitionToQ4M1:
	{
		TArray<FString> Destinations = { "q4m1", "q4m1_entrance" }; return Destinations;
	}
	case transitionToTut2:
	{
		TArray<FString> Destinations = { "tut_area2", "tut2_start" }; return Destinations;
	}
	default:
	{
		break;
	}
	}
	return TArray<FString>();
}

FString GetConversationFromLocation(FString sLocation, UParty* party)
{
	FString sConversation;
	bool bConvFlagIsSet = party->conversations.Contains(sLocation)
		&& party->conversations[sLocation] != false;
	switch (GetConversationIntFromString(sLocation))
	{
	case orz_warden:
	{
		if (GetMain()->LevelMap->IsNPCVisible("orz_warden"))
		{
			sConversation = "conv_warden_rejoin";
		}
		break;
	}
	case orz_healer:
	{
		if (GetMain()->LevelMap->IsNPCVisible("orz_healer"))
		{
			if (GetParty()->members.Num() == 1)
			{
				sConversation = "conv_healer";
			}
			else
			{
				sConversation = "conv_healer_rejoin";
			}
		}
		break;
	}
	case orz_bard:
	{
		if (!GetParty()->HasQuest(RUNAROUND_GENLOCK_GATHER))
		{
			GetParty()->HasQuest(RUNAROUND_GENLOCK_GATHER);
		}
		if (GetParty()->HasFinishedQuest(RUNAROUND_GENLOCK_GATHER))
		{
			if (GetParty()->companions[COMPANION_BARD] == nullptr)
			{
				if (bConvFlagIsSet)
				{
					sConversation = "conv_bard2";
				}
				else
				{
					sConversation = "conv_bard";
				}
			}
			else
			{
				if (GetParty()->GetCompanionByClass(COMPANION_BARD, false))
				{
					return "";
				}
				sConversation = "conv_bard_rejoin";
			}
		}
		else
		{
			sConversation = "conv_bard3";
		}
		break;
	}
	case potion_vendor_commons:
	{
		sConversation = "shop_commons_potions";
		break;
	}
	case q3m2_camp:
	{
		if (GetParty()->HasQuest(RUNAROUND_DIAMONDS_FIND))
		{
			sConversation = "conv_search";
		}
		break;
	}
	case tutorial_character:
	{
		sConversation = "conv_origin2";
		break;
	}
	case petty_officer:
	{
		if (GetMain()->LevelMap->IsNPCVisible("petty_officer"))
		{
			if (GetParty()->HasQuest(RUNAROUND_GENLOCK_GATHER))
			{
				sConversation = "conv_noble_during_genlock_quest";
			}
			else if (GetParty()->HasQuest(RUNAROUND_GENLOCK_RETURN))
			{
				sConversation = "conv_noble2";
			}
			else if (GetParty()->HasQuest(RUNAROUND_NOBLE_MEET))
			{
				sConversation = "conv_noble";
			}
			else
			{
				sConversation = "conv_noble3";
			}
		}
		break;
	}
	case armorer:
	{
		sConversation = "shop_commons_blacksmith";
		break;
	}
	case merchant_commons:
	{
		sConversation = "shop_commons_stall";
		break;
	}
	case innkeeper_commons:
	{
		sConversation = "innkeeper_commons";
		break;
	}
	case armorer_dust:
	{
		sConversation = "shop_dust_blacksmith";
		break;
	}
	case merchant_dust:
	{
		sConversation = "shop_dust_stall";
		break;
	}
	case innkeeper_dust:
	{
		sConversation = "innkeeper_dust";
		break;
	}
	case carta_thieves:
	{
		if (GetMain()->LevelMap->IsNPCVisible("carta_thieves"))
		{
			sConversation = "conv_bandit";
		}
		break;
	}
	case conv_q2m3_warden:
	{
		if (!bConvFlagIsSet)
		{
			sConversation = PLOT_WARDEN_MEET;
		}
		break;
	}
	case lowly_soldier:
	{
		if (GetMain()->LevelMap->IsNPCVisible("lowly_soldier"))
		{
			if (GetParty()->HasQuest(RUNAROUND_BEGIN))
			{
				sConversation = "conv_lowlysoldier";
			}
			else if (GetParty()->HasQuest(RUNAROUND_ARMOR_RETURN))
			{
				sConversation = "conv_lowlysoldier2";
			}
			else if (GetParty()->HasQuest(RUNAROUND_ARMOR_RECOVER))
			{
				sConversation = "conv_lowlysoldier3";
			}
			else
			{
				sConversation = "conv_lowlysoldier4";
			}
		}
		break;
	}
	case orz_main_entrance:
	{
		if (GetParty()->members.Num() > 1)
		{
			sConversation = LEAVING_ORZAMMAR_COMMONS;
		}
		break;
	}
	case common_to_dust:
	{
		sConversation = "conv_commons_to_dusttown";
		break;
	}
	case dust_to_quests:
	{
		sConversation = LEAVING_DUST_FOR_QUEST;
		break;
	}
	case q2m1_entrance:
	{
		sConversation = LEAVING_QUEST_AREA;
		break;
	}
	case q3m1_entrance:
	{
		sConversation = LEAVING_QUEST_AREA;
		break;
	}
	case q3m2_entrance:
	{
		sConversation = LEAVING_QUEST_AREA;
		break;
	}
	case q4m1_entrance:
	{
		sConversation = LEAVING_QUEST_AREA;
		break;
	}
	case captain:
	{
		if (GetMain()->LevelMap->IsNPCVisible("captain"))
		{
			if (GetParty()->HasQuest(RUNAROUND_CAPTAIN_MEET))
			{
				sConversation = "conv_captain";
			}
			else
			{
				sConversation = "conv_captain2";
			}
		}
		break;
	}
	case q4m1_dead_smugglers:
	{
		if (GetParty()->HasQuest(EXCAVATION1))
		{
			sConversation = "conv_dead_smugglers";
		}
		break;
	}
	case crime_lord_commons:
	case crime_lord_dust:
	{
		if (((sLocation == "crime_lord_commons") && GetMain()->LevelMap->IsNPCVisible("crime_lord_commons"))
			|| ((sLocation == "crime_lord_dust") && GetMain()->LevelMap->IsNPCVisible("crime_lord_dust"))) //dhk
		{
			if (!GetParty()->HasFinishedQuest(RUNAROUND_CRIMELORD_MEET))
			{
				sConversation = "conv_crimelord_commons";
			}
			else if (GetParty()->HasQuest(RUNAROUND_DIAMONDS_FIND))
			{
				sConversation = "conv_crimelord2";
			}
			else if (!GetParty()->HasQuest(EXCAVATION1, true))
			{
				sConversation = "conv_crimelord3";
			}
			else if (!GetParty()->HasFinishedQuest(EXCAVATION_DESTROYED2))
			{
				sConversation = "conv_crimelord4";
			}
			else
			{
				sConversation = "conv_crimelord5";
			}
		}
		break;
	}
	case punk_dust:
	{
		sConversation = "conv_punk_dust";
		break;
	}
	case military_placeholder:
	{
		if (!GetParty()->HasQuest(MEET_THE_HEALER, true))
		{
			sConversation = "conv_militaryleader";
		}
		else if (!GetParty()->HasFinishedQuest(MISSING_WARDEN1))
		{
			sConversation = "conv_militaryleader_waiting_for_warden";
		}
		else
		{
			if (GetParty()->HasFinishedQuest(MISSING_WARDEN1))
			{
				GetParty()->HasFinishedQuest(MISSING_WARDEN1);
			}
			if (!GetParty()->HasFinishedQuest(MISSING_WARDEN2))
			{
				sConversation = "conv_militaryleader2";
			}
			else
			{
				sConversation = "conv_militaryleader3";
			}
		}
	}
	default:
	{
		break;
	}
	}
	if (sConversation != "")
	{
		return sConversation;
	}

#ifdef DEBUG
	UE_LOG(LogTemp, Error, TEXT("GetConversationFromLocation didn't find %s"), *sLocation);
#endif // DEBUG

	return "";
}

int32 GetLocationIntFromString(FString sLocation)
{
	if (sLocation == "orz_main_entrance") return orz_main_entrance;
	else if (sLocation == "q2m1_entrance") return q2m1_entrance;
	else if (sLocation == "q3m1_entrance") return q3m1_entrance;
	else if (sLocation == "q4m1_entrance") return q4m1_entrance;
	else if (sLocation == "military_placeholder") return military_placeholder;
	else if (sLocation == "petty_officer") return petty_officer;
	else if (sLocation == "carta_thieves") return carta_thieves;
	else if (sLocation == "lowly_soldier") return lowly_soldier;
	else if (sLocation == "captain") return captain;
	else if (sLocation == "crime_lord_commons") return crime_lord_commons;
	else if (sLocation == "q3m2_camp") return q3m2_camp;
	else if (sLocation == "merchant_commons") return merchant_commons;
	else if (sLocation == "armorer") return armorer;
	else if (sLocation == "q4m1_dead_smugglers") return q4m1_dead_smugglers;
	else if (sLocation == "armorer_dust") return armorer_dust;
	else if (sLocation == "innkeeper_commons") return innkeeper_commons;
	else if (sLocation == "innkeeper_dust") return innkeeper_dust;
	else if (sLocation == "crime_lord_dust") return crime_lord_dust;
	else if (sLocation == "merchant_dust") return merchant_dust;
	else if (sLocation == "punk_dust") return punk_dust;
	else if (sLocation == "dust_to_quests") return dust_to_quests;
	else if (sLocation == "orz_bard") return orz_bard;
	else if (sLocation == "orz_healer") return orz_healer;
	else if (sLocation == "orz_warden") return orz_warden;
	else if (sLocation == "potion_vendor_commons") return potion_vendor_commons;
	else return -1;
}

int32 GetDestinationIntFromString(FString sDestination)
{
	if (sDestination == "tut_exit") return tut_exit;
	else if (sDestination == "q2m1_exit") return q2m1_exit;
	else if (sDestination == "q2m2_entrance") return q2m2_entrance;
	else if (sDestination == "q2m2_exit") return q2m2_exit;
	else if (sDestination == "q2m3_entrance") return q2m3_entrance;
	else if (sDestination == "dust_to_common") return dust_to_common;
	else if (sDestination == "common_to_dust") return common_to_dust;
	else if (sDestination == "q4m1_to_q4m2") return q4m1_to_q4m2;
	else if (sDestination == "q4m1_to_q4m3") return q4m1_to_q4m3;
	else if (sDestination == "q4m1_to_q4m4") return q4m1_to_q4m4;
	else if (sDestination == "q4m2_entrance") return q4m2_entrance;
	else if (sDestination == "q4m3_entrance") return q4m3_entrance;
	else if (sDestination == "q4m4_entrance") return q4m4_entrance;
	else if (sDestination == "q1m1_exit") return q1m1_exit;
	else if (sDestination == "tunnel_entrance") return tunnel_entrance;
	else if (sDestination == "q3m2_entrance") return q3m2_entrance;
	else if (sDestination == "transitionToOrz") return transitionToOrz;
	else if (sDestination == "transitionToQ1M1") return transitionToQ1M1;
	else if (sDestination == "transitionToQ2M1") return transitionToQ2M1;
	else if (sDestination == "transitionToQ3M1") return transitionToQ3M1;
	else if (sDestination == "transitionToQ3M2") return transitionToQ3M2;
	else if (sDestination == "transitionToDustTown") return transitionToDustTown;
	else if (sDestination == "transitionToQ4M1") return transitionToQ4M1;
	else if (sDestination == "transitionToTut2") return transitionToTut2;
	else return -1;
}

int32 GetConversationIntFromString(FString sConversation)
{
	if (sConversation == "orz_warden") return orz_warden;
	else if (sConversation == "orz_healer") return orz_healer;
	else if (sConversation == "orz_bard") return orz_bard;
	else if (sConversation == "potion_vendor_commons") return potion_vendor_commons;
	else if (sConversation == "q3m2_camp") return q3m2_camp;
	else if (sConversation == "tutorial_character") return tutorial_character;
	else if (sConversation == "petty_officer") return petty_officer;
	else if (sConversation == "armorer") return armorer;
	else if (sConversation == "merchant_commons") return merchant_commons;
	else if (sConversation == "innkeeper_commons") return innkeeper_commons;
	else if (sConversation == "armorer_dust") return armorer_dust;
	else if (sConversation == "merchant_dust") return merchant_dust;
	else if (sConversation == "innkeeper_dust") return innkeeper_dust;
	else if (sConversation == "carta_thieves") return carta_thieves;
	else if (sConversation == "conv_q2m3_warden") return conv_q2m3_warden;
	else if (sConversation == "lowly_soldier") return lowly_soldier;
	else if (sConversation == "orz_main_entrance") return orz_main_entrance;
	else if (sConversation == "common_to_dust") return common_to_dust;
	else if (sConversation == "dust_to_quests") return dust_to_quests;
	else if (sConversation == "q2m1_entrance") return q2m1_entrance;
	else if (sConversation == "q3m1_entrance") return q3m1_entrance;
	else if (sConversation == "q3m2_entrance") return q3m2_entrance;
	else if (sConversation == "q4m1_entrance") return q4m1_entrance;
	else if (sConversation == "captain") return captain;
	else if (sConversation == "q4m1_dead_smugglers") return q4m1_dead_smugglers;
	else if (sConversation == "crime_lord_commons") return crime_lord_commons;
	else if (sConversation == "crime_lord_dust") return crime_lord_dust;
	else if (sConversation == "punk_dust") return punk_dust;
	else if (sConversation == "military_placeholder") return military_placeholder;
	else return -1;
}

int32 GetBattleBackgroundForTileset(FString sTileset)
{
	if (sTileset == "Tunnels")
	{
		return 1;
	}
	else if (sTileset == "Cave")
	{
		return 4;
	}
	else
	{
		return 3;
	}
}
