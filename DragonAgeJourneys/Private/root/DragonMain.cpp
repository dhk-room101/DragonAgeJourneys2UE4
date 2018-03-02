#include "DragonMain.h"
#include "DragonAgeJourneys.h"
#include "DragonAgeJourneysCharacter.h"
#include "MapLoader.h"
#include "Map.h"
#include "GameData.h"
#include "Scripting.h"
#include "DragonAgeJourneysLibrary.h"
#include "Locations.h"
#include "GameCharacter.h"
#include "Group.h"
#include "Party.h"
#include "Tutorial.h"
#include "Battlefield.h"
#include "Classes.h"
#include "NPC.h"
#include "UniqueItems.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraTypes.h"
#include "Components/SceneComponent.h"
#include "Gender.h"
#include "Race.h"
#include "DragonAgeJourneysPlayerController.h"

UDragonMain::UDragonMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	gameData = NewObject<UGameData>();
	gameData->GameDataLoad.AddDynamic(this, &UDragonMain::OnGameDataLoaded);

	map_loader = NewObject<UMapLoader>();

	MaterialConfigure();
}

void UDragonMain::Added()
{
	gameData->LoadData();
}

void UDragonMain::StartGame()
{
	//TODO complete StartGame
	//create party
	UParty* party = GetParty();
	party->version = 1.62;
	party->areaString = "tut_area2";
	party->locationId = "tut2_start";

	//temp
	UGameCharacter* gameChar = NewObject<UGameCharacter>();
	gameChar->charName = "PLAYER";

	//randomize starting character
	int32 nGender = FMath::RandRange(2, 3);
	int32 nRace = FMath::RandRange(1, 3);
	int32 nClass;
	if (nRace == RACE_DWARF)
		nClass = FMath::RandRange(1, 2); //Dwarf can't be mage
	else nClass = FMath::RandRange(1, 3);
	gameChar->gender = nGender;
	gameChar->SetRace(nRace);
	gameChar->classId = nClass;

	gameChar->InitializeGameCharacter(gameChar->classId, false /*!bIsAI*/);
	gameChar->SetDefaultEquipmentTemplate();

	GetParty()->members.Empty();
	GetParty()->members.Add(gameChar);
	gameChar->group = GetParty()->GetGroup();
	gameChar->party = GetParty();
	ADragonAgeJourneysCharacter* playerAvatar = Cast<ADragonAgeJourneysCharacter>(GetStage()->GetFirstPlayerController()->GetPawn());
	playerAvatar->InitializeAvatar(gameChar, true);

	//after avatar is init
	gameChar->DefaultEquipment();
}

void UDragonMain::CreationFinished()
{
	_previousTime = 0;
	GetCurrentPlayerController()->SetCurrentGameMode(EUI::EXPLORE);

	//TODO beginSinglePlayer
	BeginSinglePlayer();
}

void UDragonMain::BeginSinglePlayer()
{
	LevelMap = NewObject<UMap>();
	LevelMap->Added();
}

void UDragonMain::BeginBattle(UGroup* enemyGroup, AMapHex* argHex /*= nullptr*/, int32 nTileset /*= -1*/, bool bIsScripted /*= false*/)
{
	UGameCharacter* uGameChar = nullptr;
	UGameCharacter* uEnemyChar = nullptr;
	bool bTutorial = false;

	if (enemyGroup == nullptr)
	{
		return;
	}

	if (nTileset == -1)
	{
		if (GetMain()->LevelMap)
		{
			nTileset = GetBattleBackgroundForTileset(GetMain()->LevelMap->tileset);
		}
	}

	// 	if (GetMain()->LevelMap != nullptr)
	// 	{
	// 		//TODO battle map streaming
	// 	}

	//TODO TooltipController 
		//DestroyTooltip();
		//ClearMouseWord();
		//RemoveNavPages();
		//CloseGameMenu();

	UGroup* playerGroup = GetParty()->GetGroup();

	//DHK not needed
	/*if (enemyGroup->id == "vs_tut_emissary")
	{
		//TODO TutorialHintBox
		playerGroup = NewObject<UGroup>();
		playerGroup.Add(GetTutorialAlly(2));
		playerGroup.Add(GetTutorialAlly(1));
		playerGroup.Add(GetTutorialAlly(3));
		playerGroup->id = "tutorialAllies";
		playerGroup->player = true;
		for (int32 i = 0; i < playerGroup->chars.Num(); i++)
		{
			uGameChar = playerGroup->chars[i];
			uGameChar->group = playerGroup;
		}
	}*/

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("DragonMain: resetting health and power!"));
#endif // DEBUG

	for (int32 i = 0; i < enemyGroup->chars.Num(); i++)
	{
		uEnemyChar = enemyGroup->chars[i];
		uEnemyChar->health = uEnemyChar->GetHealthMax();
		uEnemyChar->power = uEnemyChar->GetPowerMax();
#ifdef DEBUG
		UE_LOG(LogTemp, Warning, TEXT("DragonMain: char name %s"), *uEnemyChar->charName);
#endif // DEBUG
	}

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("Character ID %s"), *enemyGroup->id);
	UE_LOG(LogTemp, Warning, TEXT("begin battle"));
#endif // DEBUG

	Battlefield = NewObject<UBattlefield>();

	//DHK
	TArray<UGroup*> groups;
	groups.Add(playerGroup);
	groups.Add(enemyGroup);

	Battlefield->InitializeBattlefield(groups /*[playerGroup, enemyGroup]*/, nTileset);
	Battlefield->name = "battlefield";
	Battlefield->startedByScripting = bIsScripted;

	if (enemyGroup->id != "vs_tut_emissary")
	{
		bTutorial = false;
		bTutorial = TriggerTutorial(TUT_BATTLE1);
		if (!bTutorial && GetParty()->GetHeroCharacter()->classId != CLASS_MAGE)
		{
			bTutorial = TriggerTutorial(TUT_DUAL_EQUIP);
		}
		if (!bTutorial)
		{
			bTutorial = TriggerTutorial(TUT_BATTLE2);
		}
		if (!bTutorial && GetParty()->members.Num() > 1)
		{
			bTutorial = TriggerTutorial(TUT_BATTLE_COMPANION);
		}
	}

	//TODO add/stream Battlefield level
#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("add/stream Battlefield level"));
#endif // DEBUG
}

UDragonMain::~UDragonMain()
{
}

void UDragonMain::MaterialConfigure()
{
	metal.Add("Iron"); metal.Add("Grey Iron"); metal.Add("Steel"); metal.Add("Veridium"); metal.Add("Red Steel"); metal.Add("Silverite"); metal.Add("Dragonbone");
	wood.Add("Elm"); wood.Add("Ash"); wood.Add("Yew"); wood.Add("Whitewood"); wood.Add("Ironbark"); wood.Add("Sylvanwood"); wood.Add("Dragonthorn");
	leather.Add("Rough Hide"); leather.Add("Cured Hide"); leather.Add("Leather"); leather.Add("Hardened Leather"); leather.Add("Reinforced Leather"); leather.Add("Inscribed Leather"); leather.Add("Drakeskin");
	precious.Add("Silver"); precious.Add("Gold"); precious.Add("Platinum"); precious.Add("Obsidian"); precious.Add("Crystalline"); precious.Add("Other");
	cloth.Add("Linen"); cloth.Add("Wool"); cloth.Add("Velvet"); cloth.Add("Silk"); cloth.Add("Suncloth"); cloth.Add("Aetherweave");
	debug.Add("Debug1"); debug.Add("Debug2"); debug.Add("Debug3"); debug.Add("Debug4"); debug.Add("Debug5"); debug.Add("Debug6");
}

void UDragonMain::OnGameDataLoaded()
{
#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("On Game Data Loaded !"));
#endif // DEBUG

	//TODO change start to main menu
	StartGame();
}
