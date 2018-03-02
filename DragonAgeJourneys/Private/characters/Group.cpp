#include "Group.h"
#include "Scripting.h"
#include "GameCharacter.h"
#include "Party.h"
#include "DialogueNode.h"
#include "DragonMain.h"
#include "Classes.h"
#include "MapLoader.h"

UGroup::UGroup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UGroup::InitializeGroup(TArray<int32> tChars)
{
	level = GetParty()->GetLevel();
	if (tChars.Num() > 0)
	{
		for (int32 i = 0; i < tChars.Num(); i++)
		{
			UGameCharacter* gameChar = NewObject<UGameCharacter>();
			gameChar->InitializeGameCharacter(tChars[i], true);
			gameChar->group = this;
			gameChar->SetMonsterLevel(level);
			chars.Add(gameChar);
		}

		//TODO displayed? visible?
		displayed = false;
	}
}

void UGroup::Add(UGameCharacter* uGameChar)
{
	uGameChar->group = this;
	chars.Add(uGameChar);
}

bool UGroup::IsDefeated()
{
	if (overrideDefeated)
	{
		return true;
	}
	
	for(int32 i=0;i<chars.Num();i++)
	{
		UGameCharacter* loc1 = chars[i];
		if (loc1->IsInactive() == false)
		{
			return false;
		}
	}

	overrideDefeated = true;
	
	return true;
}

bool UGroup::GetDefeated()
{
	return GetParty()->enemiesDefeated.Contains(id) && GetParty()->enemiesDefeated[id];
}

UGroup* UGroup::GenerateGroup(FString sGroup)
{
	UGroup* group = nullptr;
	TArray<int32> tChars;

	if (sGroup == "")
	{
#ifdef DEBUG
		UE_LOG(LogTemp, Error, TEXT("!!!ERROR!!!: group with no name!"));
#endif // DEBUG

		return nullptr;
	}

	group = GetMain()->map_loader->GetEncounter(sGroup);

	if (sGroup.Contains("ScareParty"))
	{
		group->preventFightWithDialogueId = TOO_STRONG;
	}

	if (group->id == "")
	{
		switch (GetGroupIntFromString(sGroup))
		{
		case vs_emissary_mine:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_HURLOCK_EMISSARY);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_carta_thieves:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_CARTA_ROGUE); tChars.Add(CLASS_BRIGAND_WARRIOR);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_thaig_golem1:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_STONE_GOLEM);
			group->aggroRadius = 0;
			group->InitializeGroup(tChars);
			break;
		}
		case vs_lake_workgroup1:
		case vs_lake_workgroup2:
		case vs_lake_workgroup3:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_GENLOCK_ALPHA); tChars.Add(CLASS_CORRUPTED_DWARF); tChars.Add(CLASS_CORRUPTED_DWARF); tChars.Add(CLASS_HURLOCK_ARCHER);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_thaig_golem2:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_STONE_GOLEM);
			group->aggroRadius = 0;
			group->InitializeGroup(tChars);
			break;
		}
		case vs_tut_emissary:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_OGRE); tChars.Add(CLASS_HURLOCK_EMISSARY);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_thaig_spiders1:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_SPIDER_GIANT); tChars.Add(CLASS_SPIDER_GIANT_SMALL); tChars.Add(CLASS_SPIDER_GIANT);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_thaig_spiders2:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_SPIDER_GIANT); tChars.Add(CLASS_SPIDER_GIANT); tChars.Add(CLASS_SPIDER_GIANT);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_thaig_spiders3:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_SPIDER_GIANT_SMALL); tChars.Add(CLASS_SPIDER_GIANT); tChars.Add(CLASS_SPIDER_GIANT_SMALL); tChars.Add(CLASS_SPIDER_GIANT_SMALL);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_deepstalkers_1:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_DEEP_STALKER); tChars.Add(CLASS_DEEP_STALKER);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_warden_guards:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_GENLOCK); tChars.Add(CLASS_GENLOCK_ARCHER); tChars.Add(CLASS_GENLOCK);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_warden_patrol:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_HURLOCK_ARCHER); tChars.Add(CLASS_GENLOCK_ARCHER);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_warden_ambush:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_GENLOCK_ARCHER_RUNT); tChars.Add(CLASS_GENLOCK_ALPHA);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_genlock_test:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_GENLOCK); tChars.Add(CLASS_GENLOCK_ALPHA); tChars.Add(CLASS_GENLOCK);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_test_ogre:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_HURLOCK); tChars.Add(CLASS_OGRE); tChars.Add(CLASS_HURLOCK);
			group->InitializeGroup(tChars);
			break;
		}
		case quest_test_spiders:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL); tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_spiders:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL); tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_pair:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL); tChars.Add(CLASS_HURLOCK);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_final:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_HURLOCK); tChars.Add(CLASS_HURLOCK); tChars.Add(CLASS_HURLOCK);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_test:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_HURLOCK); tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL); tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL); tChars.Add(CLASS_HURLOCK);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_hurlocks:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_HURLOCK); tChars.Add(CLASS_HURLOCK); tChars.Add(CLASS_HURLOCK); tChars.Add(CLASS_HURLOCK);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_patrol:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_HURLOCK); tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL); tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_spiderama:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL); tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL); tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL); tChars.Add(CLASS_SPIDER_CORRUPTED_SMALL);
			group->InitializeGroup(tChars);
			break;
		}
		case vs_tut_genlock:
		{
			group = NewObject<UGroup>();
			tChars.Add(CLASS_GENLOCK);
			group->InitializeGroup(tChars);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	
	group->id = sGroup;

	if (group->id == "")
	{
#ifdef DEBUG
		UE_LOG(LogTemp, Error, TEXT("!!!WARNING!!!: could not generate monster group for id: %s"), *sGroup);
#endif // DEBUG

		return group;
	}

	return group;
}

TArray<int32> UGroup::GetBattlePositions()
{
	TArray<int32> battlePos;
	switch (chars.Num())
	{
	case 1:
	default:
	{
		battlePos.Add(4);
		break;
	}
	case 2:
	{
		battlePos.Add(2); battlePos.Add(6);
		break;
	}
	case 3:
	{
		battlePos.Add(1); battlePos.Add(4); battlePos.Add(7);
		break;
	}
	case 4:
	{
		battlePos.Add(1); battlePos.Add(3); battlePos.Add(5); battlePos.Add(7);
		break;
	}
	case 5:
	{
		battlePos.Add(0); battlePos.Add(2); battlePos.Add(4); battlePos.Add(6); battlePos.Add(8);
		break;
	}
	case 6:
	{
		battlePos.Add(0); battlePos.Add(2); battlePos.Add(3); battlePos.Add(5); battlePos.Add(6); battlePos.Add(8);
		break;
	}
	case 7:
	{
		battlePos.Add(0); battlePos.Add(1); battlePos.Add(3); battlePos.Add(4); battlePos.Add(5); battlePos.Add(7); battlePos.Add(8);
		break;
	}
	case 8:
	{
		battlePos.Add(0); battlePos.Add(1); battlePos.Add(2); battlePos.Add(3); battlePos.Add(5); battlePos.Add(6); battlePos.Add(7); battlePos.Add(8);
		break;
	}
	case 9:
	{
		battlePos.Add(0); battlePos.Add(1); battlePos.Add(2); battlePos.Add(3); battlePos.Add(4); battlePos.Add(5); battlePos.Add(6); battlePos.Add(7); battlePos.Add(8);
		break;
	}
	}

	return battlePos;
}

UGroup::~UGroup()
{
}

int32 UGroup::GetGroupIntFromString(FString sGroup)
{
	if (sGroup == "vs_emissary_mine") return vs_emissary_mine;
	else if (sGroup == "vs_carta_thieves") return vs_carta_thieves;
	else if (sGroup == "vs_thaig_golem1") return vs_thaig_golem1;
	else if (sGroup == "vs_lake_workgroup1") return vs_lake_workgroup1;
	else if (sGroup == "vs_lake_workgroup2") return vs_lake_workgroup2;
	else if (sGroup == "vs_lake_workgroup3") return vs_lake_workgroup3;
	else if (sGroup == "vs_thaig_golem2") return vs_thaig_golem2;
	else if (sGroup == "vs_tut_emissary") return vs_tut_emissary;
	else if (sGroup == "vs_thaig_spiders1") return vs_thaig_spiders1;
	else if (sGroup == "vs_thaig_spiders2") return vs_thaig_spiders2;
	else if (sGroup == "vs_thaig_spiders3") return vs_thaig_spiders3;
	else if (sGroup == "vs_deepstalkers_1") return vs_deepstalkers_1;
	else if (sGroup == "vs_warden_guards") return vs_warden_guards;
	else if (sGroup == "vs_warden_patrol") return vs_warden_patrol;
	else if (sGroup == "vs_warden_ambush") return vs_warden_ambush;
	else if (sGroup == "vs_genlock_test") return vs_genlock_test;
	else if (sGroup == "vs_test_ogre") return vs_test_ogre;
	else if (sGroup == "vs_spiders") return vs_spiders;
	else if (sGroup == "vs_pair") return vs_pair;
	else if (sGroup == "vs_final") return vs_final;
	else if (sGroup == "vs_test") return vs_test;
	else if (sGroup == "vs_hurlocks") return vs_hurlocks;
	else if (sGroup == "vs_patrol") return vs_patrol;
	else if (sGroup == "vs_spiderama") return vs_spiderama;
	else if (sGroup == "quest_test_spiders") return quest_test_spiders;
	else if (sGroup == "vs_tut_genlock") return vs_tut_genlock;
	else return -1;
}
