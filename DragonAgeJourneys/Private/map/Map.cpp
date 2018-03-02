#include "Map.h"
#include "MapHex.h"
#include "Scripting.h"
#include "Party.h"
#include "PartyPosition.h"
#include "NeutralPosition.h"
#include "DragonMain.h"
#include "MapLoader.h"
#include "FSpecialsList.h"
#include "Locations.h"
#include "Group.h"
#include "GameCharacter.h"
#include <ConstructorHelpers.h>
#include "Algo/Reverse.h"
#include "Engine/World.h"
#include "DragonAgeJourneysPlayerController.h"
#include "DragonAgeJourneysCharacter.h"
#include "DragonAgeJourneysAnimInstance.h"
#include "Animation.h"
#include "Kismet/KismetMathLibrary.h"

UMap::UMap(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//static ConstructorHelpers::FObjectFinder<UBlueprint> BPMapHex(TEXT("Blueprint'/Game/Data/Blueprints/BPMapHex.BPMapHex'"));
	static ConstructorHelpers::FObjectFinder<UClass> BPMapHex(TEXT("Class'/Game/Data/Blueprints/BPMapHex.BPMapHex_C'"));

	if (BPMapHex.Object) {
		refMapHex = BPMapHex.Object;
		//refMapHex = (UClass*)BPMapHex.Object->GeneratedClass;
	}

	//static ConstructorHelpers::FObjectFinder<UBlueprint> BPBattleHex(TEXT("Blueprint'/Game/Data/Blueprints/BPBattleHex.BPBattleHex'"));
	static ConstructorHelpers::FObjectFinder<UClass> BPBattleHex(TEXT("Class'/Game/Data/Blueprints/BPBattleHex.BPBattleHex_C'"));

	if (BPBattleHex.Object) {
		refBattleHex = BPBattleHex.Object;
		//refBattleHex = (UClass*)BPBattleHex.Object->GeneratedClass;
	}

	//static ConstructorHelpers::FObjectFinder<UBlueprint> BPAvatar(TEXT("Blueprint'/Game/Data/Blueprints/BPAvatar.BPAvatar'"));
	static ConstructorHelpers::FObjectFinder<UClass> BPAvatar(TEXT("Class'/Game/Data/Blueprints/BPAvatar.BPAvatar_C'"));

	if (BPAvatar.Object) {
		refAvatar = BPAvatar.Object;
		//refAvatar = (UClass*)BPAvatar.Object->GeneratedClass;
	}
}

void UMap::Added()
{
	GetMain()->map_loader->MapDataLoad.AddDynamic(this, &UMap::OnMapDataLoaded);
	MapZ = LocationOutGame.Z / 3 + 5;

	//TODO add main player avatar to the party, manually?//DHK
	partyPosition = NewObject<UPartyPosition>();
	partyPosition->avatars.Add(Cast<ADragonAgeJourneysCharacter>(GetStage()->GetFirstPlayerController()->GetPawn()));

	BeginMapLoad();
}

void UMap::MapTick()
{
	int32 loc1 = 75;
	bool loc2 = false;
	int32 nStack = 0;
	auto notification = nullptr;//Notification

	AMapHex* currentHex = nullptr;
	FVector currentVectorLocation;
	FVector aggroVectorLocation;
	bool bOnRoute = false;
	float currentVLength = 0.f;
	bool bNextHop = false;
	bool bNotArrived = false;

	//TODO UpdateLootStack();
	//TODO notificationStack

	/*if (notificationStack.Num())
	{
		loc2 = false;
		nStack = notificationStack.Num() - 1;
		while (nStack >= 0)
		{
			notification = notificationStack[nStack] as packages.gui.Notification;
			if (!!notification)
			{
				if (notification.frameLife >= loc1)
				{
					notification.alpha = 1 - (notification.frameLife - loc1) * 0.1;
					if (notification.alpha <= 0)
					{
						if (notification.parent)
						{
							notification.parent.removeChild(notification);
						}
					}
				}
				if (notification.parent == nullptr)
				{
					notificationStack.splice(nStack, 1);
				}
				auto loc12 ;
				auto loc13 = ((loc12 = notification).frameLife + 1);
				loc12.frameLife = loc13;
			}
			--nStack;
		}
	}*/

	if (inTransit && route.Num() > 0)
	{
		currentHex = route[0];
		currentVectorLocation = MoveAlongRoute(currentHex, FVector(partyPosition->X, partyPosition->Y, MapZ), MAPSPEED);

		SetGroupSpeed(partyPosition, SpeedWalk);

		partyPosition->BeginMove(currentVectorLocation.X);
		//TODO UpdatePartyFaceDirection();
		partyPosition->UpdatePartyPosition(currentVectorLocation);
		bOnRoute = true;
		currentVLength = (partyPosition->X - currentHex->GetActorLocation().X) * (partyPosition->X - currentHex->GetActorLocation().X) + (partyPosition->Y - currentHex->GetActorLocation().Y) * (partyPosition->Y - currentHex->GetActorLocation().Y);
		bNextHop = false;
		bNextHop = currentVLength == 0;

		/*if (currentLocation == currentHex)
		{
			bNextHop = currentVLength == 0;
		}
		else
		{
			bNextHop = currentVLength < 100;
		}*/

		if (!bNextHop)
		{
			SetGroupRotation(partyPosition, currentHex);
		}
		else //if (bNextHop)
		{
			if (currentLocation == currentHex)
			{
				route.RemoveAt(0, 1);
				bNotArrived = false;

				if (route.Num() != 0)
				{
					if (currentLocation != route[route.Num() - 1])
					{
						bNotArrived = true;
					}
				}

				if (MoveToLocation(currentHex, bNotArrived) || route.Num() == 0)
				{
					bOnRoute = false;
					inTransit = false;
					RemoveRouteLine(true);
					route.Empty();
					EndPlayerWalkSounds();
					targetLocation = nullptr;
					//TODO movementIndicator.visible = false;
				}
			}
			else if (route.Num() > 0)
			{
				UpdateCurrentLocation(route[0]);
				CheckForTriggers(currentLocation);
				if (currentLocation->obstructed == true)
				{
#ifdef DEBUG
					UE_LOG(LogTemp, Error, TEXT("Where you going? Is Obstructed"));
#endif // DEBUG
				}
			}
		}
		/*if (loc8)
		{
			UpdateMapView();
		}*/
	}
	else
	{
		SetGroupSpeed(partyPosition, 0);
	}

	//TODO aggroGroup
	if (aggroGroup != nullptr && !bBattle) //TODO replace with Battlefield not nullptr
	{
		//DHK reset movement if not trying to flee
		if (!IsMoving())
		{
			SetGroupSpeed(partyPosition, 0);
		}

		if (aggroGroup->pursuitRoute.Num() > 0)
		{
			aggroVectorLocation = MoveAlongRoute(aggroGroup->pursuitRoute[0], FVector(aggroGroup->groupPosition->X, aggroGroup->groupPosition->Y, MapZ), MAPSPEED * 2.5);
			aggroGroup->groupPosition->BeginMove(aggroVectorLocation.X);
			aggroGroup->groupPosition->UpdatePartyPosition(aggroVectorLocation);
			if (aggroGroup->groupPosition->X == aggroGroup->pursuitRoute[0]->GetActorLocation().X &&
				aggroGroup->groupPosition->Y == aggroGroup->pursuitRoute[0]->GetActorLocation().Y)
			{
				aggroGroup->pursuitHex = aggroGroup->pursuitRoute[0];
				aggroGroup->pursuitRoute.RemoveAt(0, 1);
			}

			SetGroupRotation(aggroGroup->groupPosition, aggroGroup->pursuitHex, true);
			SetGroupSpeed(aggroGroup->groupPosition, SpeedRun);
		}
		else //(aggroGroup->pursuitRoute.Num() == 0)
		{
			//TODO attackAnim
			//aggroGroup->groupPosition.attackAnim(partyPosition->X);
#ifdef DEBUG
			UE_LOG(LogTemp, Warning, TEXT("Attack!"));
#endif // DEBUG

			//DHK
			//reset movement speed
			SetGroupSpeed(partyPosition, 0);
			SetGroupSpeed(aggroGroup->groupPosition, 0);

			/*battlePendingHex = aggroGroup->pursuitHex;
			battlePendingHex->enemyGroup = aggroGroup;
			MoveToLocation(battlePendingHex, false);*/
			bBattle = true;
			GetMain()->BeginBattle(aggroGroup, aggroGroup->pursuitHex);
			partyPosition->EndMove();
		}
	}

	/*if (frameCount == 1)
	{
		partyPosition->EndMove();
	}*/

	/*if (frameCount <= 1)
	{
		loc13 = ((loc12 = this).frameCount + 1);
		loc12.frameCount = loc13;
	}*/

	//layers.update();
	if (bOnRoute == false)
	{
		return;
	}

	//DrawMonsterRoutes();
	//UpdateMonsterVisibility();
}

void UMap::UpdateVisitedCache()
{
	AMapHex* hex = nullptr;

	if (!_visibilityEnabled)
	{
		return;
	}

	if (!_loadedVisitCache)
	{
		return;
	}

	if (IsCityLocation(areaString))
	{
		return;
	}

	TArray<int32> arrFlags;
	arrFlags.SetNum(hexes.Num());

	for (int32 i = 0; i < arrFlags.Num(); i++)
	{
		hex = hexes[i];
		if (hex->visited)
		{
			arrFlags[i] = VISITED_FLAG;
		}
		else if (hex->seen)
		{
			arrFlags[i] = SEEN_FLAG;
		}
		else
		{
			arrFlags[i] = 0;
		}
	}
	//TODO
	//GetParty()->ExplorationLocations[areaString] = arrFlags;
}

void UMap::EndPlayerWalkSounds()
{
	//TODO EndPlayerWalkSounds
}

void UMap::RemoveBattleIcon()
{
	//TODO RemoveBattleIcon
}

void UMap::RemoveSpeechBubbleIcon()
{
	//TODO RemoveSpeechBubbleIcon
}

void UMap::RemoveRouteLine(bool bBool /*= false*/)
{
	//TODO RemoveRouteLine
}

void UMap::SelectCharacter(UGameCharacter* gameCharacter)
{
	//TODO SelectCharacter
	selectedChar = GetParty()->members[0];
}

void UMap::TriggerTransitionArea(FString sLocation)
{
	//TODO TriggerTransitionArea
}

void UMap::AddEnemyGroup(UGroup* group)
{
	enemyGroups.Add(group);
}

void UMap::UpdateMonsterVisibility()
{
	//TODO doublecheck
	float relX;
	float relY;
	int32 nCounter = 0;
	ADragonAgeJourneysCharacter* monsterAvatar = nullptr;
	for (int32 i = 0; i < enemyGroups.Num(); i++)
	{
		UGroup* group = enemyGroups[i];

		/*if (group->displayed == false || group->GetDefeated() == true)
		{
			continue;
		}*/
		if (GetHexDistance(currentLocation->index, group->mapHex->index) > GLOBAL_VIEW_DISTANCE)
		{
			continue;
		}
		group->displayed = true;
		//enemyLayer.addChild(group->groupPosition);
		relX = 20 - (group->chars.Num() - 1) * 5;
		relY = 0 - (group->chars.Num() - 1) * 5;
		nCounter = 0;
		while (nCounter < group->chars.Num())
		{
			FActorSpawnParameters SpawnParams;
			monsterAvatar = GetStage()->SpawnActor<ADragonAgeJourneysCharacter>(refAvatar, SpawnParams);
			monsterAvatar->InitializeAvatar(group->chars[nCounter], true);
			FVector playerLoc = GetStage()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
			FVector avatarLoc = monsterAvatar->GetActorLocation();
			FVector direction = playerLoc - avatarLoc;
			direction = FVector(direction.X, direction.Y, 0);
			auto avatarRot = FRotationMatrix::MakeFromX(direction).Rotator();
			monsterAvatar->SetActorRotation(avatarRot);
			monsterAvatar->SetActorLocation(FVector(group->groupPosition->X, group->groupPosition->Y, playerLoc.Z));
			/*if (group->chars.Num() != 1)
			{
				monsterAvatar->x = relX - nCounter * 10;
				monsterAvatar->y = relY + nCounter * 10;
			}
			else
			{
				monsterAvatar->x = 0;
				monsterAvatar->y = 0;
			}
			monsterAvatar->scaleX = 0.3;
			monsterAvatar->scaleY = 0.3;
			monsterAvatar->mouseEnabled = false;
			monsterAvatar->mouseChildren = false;
			monsterAvatar->SetFacingLeft(group->mapHex->x > currentLocation->x);*/
			group->groupPosition->avatars.Add(monsterAvatar);
			//group->groupPosition.addChild(monsterAvatar);
			enemyAvatars.Add(monsterAvatar);
			monsterAvatar->AnimateAvatar(ANIMATION_IDLE);
			++nCounter;
		}
	}
}

void UMap::RequestTravelAdjacentTo(AMapHex* mapHex, int32 nHops /*= -1*/)
{
	AMapHex* hexDestination = nullptr;
	int32 nDistance = 0;
	int32 nMaxDistance = 999;

	for (int32 i = 0; i < mapHex->neighbours.Num(); i++)
	{
		AMapHex* neighbourHex = mapHex->neighbours[i];

		if (neighbourHex == nullptr || neighbourHex->obstructed)
		{
			continue;
		}

		nDistance = GetHexDistance(currentLocation->index, neighbourHex->index);

		if (nDistance < nMaxDistance)
		{
			hexDestination = neighbourHex;
			nMaxDistance = nDistance;
		}
	}
	if (hexDestination == nullptr)
	{
#ifdef DEBUG
		UE_LOG(LogTemp, Warning, TEXT("ERROR! Map RequestTravelAdjacentTo() - we couldn\'t find somewhere adjacent to move to!"));
#endif // DEBUG

	}
	else
	{
		RequestTravelTo(hexDestination, nHops);
	}
}

void UMap::RequestTravelTo(AMapHex* mapHex, int32 nHops /*= -1*/)
{
	if (suspendInput)
	{
		return;
	}

	if (mapHex == targetLocation)
	{
		if (IsMoving())
		{
			return;
		}

		MoveToLocation(mapHex, false);
	}

	TArray<AMapHex*> arRoute = FindRouteBetween(currentLocation, mapHex);

	if (arRoute.Num() == 0)
	{
#ifdef DEBUG
		UE_LOG(LogTemp, Warning, TEXT("WARNING: failed to find player route from %s to %s"), *currentLocation->locationName, *mapHex->locationName);
#endif // DEBUG

		return;
	}

	if (arRoute.Num() == 2)
	{
		if (arRoute[0]->AdjacentTo(arRoute[1]) == false)
		{
#ifdef DEBUG
			UE_LOG(LogTemp, Warning, TEXT("SORRY! no valid route!"));
#endif // DEBUG

			return;
		}
	}
	if (nHops != -1 && arRoute.Num() > nHops)
	{
#ifdef DEBUG
		UE_LOG(LogTemp, Warning, TEXT("WARNING: Stopping route change due to path found being too long. Path length: %i ; Max length given: %i "), arRoute.Num(), nHops);
#endif // DEBUG

		return;
	}

	route = arRoute;

	targetLocation = mapHex;

	if (route.Num() > 1)
	{
		route.RemoveAt(0, 1);
	}

	inTransit = true;

	//TODO avatar sound
	/*if (selectedChar && selectedChar->avatar)
	{
		selectedChar->avatar->StartWalkSound(0.2, 15);
	}*/

	//TODO movementIndicator
	/*movementIndicator.visible = true;
	movementIndicator->X = mapHex->x;
	movementIndicator->Y = mapHex->y;*/

}

bool UMap::MoveToLocation(AMapHex* mapHex, bool bArrived /*= false*/)
{
	//TODO bTrigger? or bMoving? or bArrived?
	AMapHex* hex = nullptr;
	FString sLocation;
	FString sConversation;
	//auto loot = nullptr;
	UQuest* quest = nullptr;
	//auto guiDialogBox = nullptr;

	// 	bool bIsVisited = false;
	// 	if (mapHex != nullptr)
	// 	{
	// 		bIsVisited = mapHex->visited;
	// 	}

	UpdateCurrentLocation(mapHex);
	partyPosition->UpdatePartyPosition(FVector(
		currentLocation->GetActorLocation().X,
		currentLocation->GetActorLocation().Y,
		MapZ));

	//TODO UpdateDirectionIndicators
	//UpdateDirectionIndicators();

	FString sHexType = currentLocation->Visit(IsMoving());
	if (CheckForTriggers(mapHex))
	{
		bArrived = false;
	}

	if (!IsMoving())
	{
		partyPosition->EndMove();
		if (conversationPendingHex != nullptr)
			sHexType = "conversation";
		if (battlePendingHex != nullptr)
			sHexType = "battle";
	}

	if (!bArrived)
	{
		if (sHexType == "battle")
		{
			inTransit = false;
			EndPlayerWalkSounds();
			RemoveRouteLine(true);
			route.Empty();
			hex = currentLocation;
			if (battlePendingHex != nullptr)
			{
				hex = battlePendingHex;
				UE_LOG(LogTemp, Warning, TEXT("init chosen battle: party->X %f on hex->X %f"), partyPosition->X, battlePendingHex->GetActorLocation().X);
				partyPosition->FaceTowards(battlePendingHex->GetActorLocation().X);
				battlePendingHex = nullptr;
				RemoveBattleIcon();
				RunAggroTrigger(hex->enemyGroup, false);
			}
			/*else
			{
				GetMain()->BeginBattle(hex->enemyGroup, hex);
			}*/
			return true;
		}
		if (sHexType == "transition")
		{
			UE_LOG(LogTemp, Warning, TEXT("beginning area transition"));
			EndPlayerWalkSounds();
			TriggerTransitionArea(currentLocation->locationName);
			return true;
		}
		if (sHexType == "conversation")
		{
			inTransit = false;
			EndPlayerWalkSounds();
			RemoveRouteLine(true);
			//TODO movementIndicator.visible = false;
			route.Empty();
			sLocation = currentLocation->locationName;
			if (conversationPendingHex != nullptr)
			{
				sLocation = conversationPendingHex->locationName;
				UE_LOG(LogTemp, Warning, TEXT("init conversation: party->X %f and hex->X %f"),
					partyPosition->X, conversationPendingHex->GetActorLocation().X);
				partyPosition->FaceTowards(conversationPendingHex->GetActorLocation().X);
				if (conversationPendingHex->npc)
				{
					conversationPendingHex->npc->FaceTowards(partyPosition->X);
				}
				conversationPendingHex = nullptr;
				RemoveSpeechBubbleIcon();
			}
			sConversation = GetConversationFromLocation(sLocation, GetParty());
			if (sConversation != "")
			{
				TriggerConversation(sConversation, sLocation);
			}
			return true;
		}
		//TODO loot
		/*if (sHexType == "loot")
		{
			inTransit = false;
			EndPlayerWalkSounds();
			RemoveRouteLine(true);
			route = nullptr;
			loot = packages.items.LootGenerator.generateForChest(currentLocation->locationName, currentLocation->type);
			giveLootToParty(loot);
			GetParty()->hexesLooted[currentLocation->locationName] = true;
			removeSpecialFromHex(currentLocation);
			int32 group2 = 0;
			TArray<UQuest*> group3 = GetParty()->quests;
			for each (quest in group3)
			{
				if (quest->questType == LOOT && quest->questTarget == currentLocation->locationName)
				{
					TriggerQuestComplete(quest->questId);
				}
			}
			UpdatePortraits();
			Bars.current.quickBar.setChar(selectedChar);
			updateActiveQuest();
			return false;
		}*/

		//TODO recovery
		/*if (sHexType == "recovery")
		{
			inTransit = false;
			endPlayerWalkSounds();
			removeRouteLine(true);
			route = nullptr;
			if (currentLocation->type == "recovery_full")
				guiDialogBox = new GenericDialogBox(RECOVERY_FULL, currentLocation, useRecovery, nullptr);
			if (currentLocation->type == "recovery_partial")
				guiDialogBox = new GenericDialogBox(RECOVERY_PARTIAL, currentLocation, useRecovery, nullptr);
			if (currentLocation->type == "recovery_power")
				guiDialogBox = new GenericDialogBox(RECOVERY_POWER, currentLocation, useRecovery, nullptr);
			if (currentLocation->type == "recovery_health")
				guiDialogBox = new GenericDialogBox(RECOVERY_HEALTH, currentLocation, useRecovery, nullptr);
			if (guiDialogBox)
			{
				guiLayer.addChild(guiDialogBox);
			}
			return false;
		}*/
	}

	//TODO shop
	/*if (sHexType == "shop")
	{
		dm.main.addNavPage("shop");
	}*/

	return false;
}

bool UMap::IsNPCVisible(FString sName)
{
	UNeutralPosition* npc = nullptr;

	if (npcs.Num() == 0)
	{
		return false;
	}

	int32 nIndex = npcs.Num() - 1;
	while (nIndex >= 0)
	{
		npc = npcs[nIndex];
		if (npc != nullptr)
		{
			if (npc->name == sName && npc->active)
			{
				return true;
			}
		}
		--nIndex;
	}

	return false;
}

float UMap::h_distance(AMapHex* hexA, AMapHex* hexB)
{
	return FMath::Max(FMath::Abs(hexA->GetActorLocation().X - hexB->GetActorLocation().X),
		FMath::Abs(hexA->GetActorLocation().Y - hexB->GetActorLocation().Y));
}

FVector UMap::MoveAlongRoute(AMapHex* mapHex, FVector vLoc, float fNormalize)
{
	if (mapHex == nullptr)
	{
		return vLoc;
	}

	FVector vDest = FVector(mapHex->GetActorLocation().X, mapHex->GetActorLocation().Y, MapZ);
	FVector vResult = FMath::VInterpConstantTo(vLoc, vDest, DeltaSeconds, fNormalize);

	/*FVector vDelta = FVector(mapHex->GetActorLocation().X - vLoc.X, mapHex->GetActorLocation().Y - vLoc.Y, MapZ);
	vDelta.Normalize(fNormalize);
	FVector vResult = vLoc + vDelta;//loc1 = vLoc.Add(loc2);
	FVector vResult = FMath::LerpStable(vLoc, vDest, 0.01f);*/

	/*if (vLoc.X < mapHex->GetActorLocation().X)
	{
		vResult.X = FMath::Min(vResult.X, mapHex->GetActorLocation().X);
	}
	else
	{
		vResult.X = FMath::Max(vResult.X, mapHex->GetActorLocation().X);
	}

	if (vLoc.Y < mapHex->GetActorLocation().Y)
	{
		vResult.Y = FMath::Min(vResult.Y, mapHex->GetActorLocation().Y);
	}
	else
	{
		vResult.Y = FMath::Max(vResult.Y, mapHex->GetActorLocation().Y);
	}*/

	return vResult;
}

AMapHex* UMap::GetLocationById(FString sString)
{
	for (int32 i = 0; i < hexes.Num(); i++)
	{
		if (hexes[i]->locationName == sString)
		{
			return hexes[i];
		}
	}

	return nullptr;
}

TArray<AMapHex*> UMap::FindRouteBetween(AMapHex* hexA, AMapHex* hexB)
{
	float fDistance = 0.f;

	TArray<AMapHex*> arrRoute;
	AMapHex* candidateHex = nullptr;
	AMapHex* neighbourHex = nullptr;
	AMapHex* evalHex = nullptr;

	TArray<AMapHex*> arrEval;
	arrEval.Add(hexA);
	hexA->astar_parent = nullptr;
	TArray<AMapHex*> arrCompare;//compare eval vs neighbour
	TArray<float> arrSumDistance;
	TArray<int32> arrDifference;
	TArray<float> arrHDistance;

	//TODO init empty hexes.Num() long
	if (hexA->index > arrDifference.Num()) arrDifference.SetNum(hexes.Num());
	arrDifference[hexA->index] = 0;
	if (hexA->index > arrHDistance.Num()) arrHDistance.SetNum(hexes.Num());
	arrHDistance[hexA->index] = h_distance(hexA, hexB);
	if (hexA->index > arrSumDistance.Num()) arrSumDistance.SetNum(hexes.Num());
	arrSumDistance[hexA->index] = arrHDistance[hexA->index];
	while (arrEval.Num() > 0)
	{
		fDistance = 100000000000;
		evalHex = nullptr;
		for (int i = 0; i < arrEval.Num(); i++)
		{
			if (arrSumDistance[arrEval[i]->index] < fDistance)
			{
				fDistance = arrSumDistance[arrEval[i]->index];
				evalHex = arrEval[i];
			}
		}
		if (evalHex == hexB)
		{
			arrRoute.Empty();
			candidateHex = hexB;
			arrRoute.Add(candidateHex);
			while (candidateHex->astar_parent != nullptr)
			{
				candidateHex = candidateHex->astar_parent;
				arrRoute.Add(candidateHex);
			}
			Algo::Reverse(arrRoute);
			return arrRoute;
		}

		arrCompare.Add(evalHex);
		arrEval.RemoveAt(arrEval.IndexOfByKey(evalHex), 1);

		for (int32 i = 0; i < evalHex->neighbours.Num(); i++)
		{
			neighbourHex = evalHex->neighbours[i];
			if (evalHex != neighbourHex)
			{
				if (neighbourHex->obstructed != true)
				{
					if (arrCompare.IndexOfByKey(neighbourHex) == INDEX_NONE /*-1*/)
					{
						if (arrEval.IndexOfByKey(neighbourHex) != INDEX_NONE /*-1*/)
						{
							if (arrDifference[evalHex->index] + 50 < arrDifference[neighbourHex->index])
							{
								neighbourHex->astar_parent = evalHex;
								arrDifference[neighbourHex->index] = arrDifference[evalHex->index] + 50;
								arrSumDistance[neighbourHex->index] = arrDifference[neighbourHex->index] + arrHDistance[neighbourHex->index];
							}
						}
						else
						{
							neighbourHex->astar_parent = evalHex;
							arrEval.Add(neighbourHex);
							arrHDistance[neighbourHex->index] = h_distance(neighbourHex, hexB);
							arrDifference[neighbourHex->index] = arrDifference[evalHex->index] + 50;
							arrSumDistance[neighbourHex->index] = arrDifference[neighbourHex->index] + arrHDistance[neighbourHex->index];
						}
					}
				}
			}
		}
	}

	return arrRoute;
}

void UMap::RunAggroTrigger(UGroup* group, bool bAlert, float fDistance)
{
	battleHex = currentLocation;
	route.Empty(); route.Add(battleHex);//route = [battleHex];

	//TODO finish RunAggroTrigger
// 	movementIndicator.visible = false;
// 	suspendInput = true;
// 	if (hexB)
// 	{
// 		partyPosition->alertAnim();
// 	}
// 	packages.audio.Sounds.play("alert");
// 	auto loc2 = new LetterBoxOverlay();
// 	addChild(loc2);
// 	loc2.animateOn();

	aggroGroup = group;
	if (fDistance > 0)
	{
		group->pursuitRoute = FindRouteBetween(group->mapHex, battleHex);

		if (group->pursuitRoute.Num() > 1)
		{
			//group->pursuitRoute.Pop();
			group->pursuitRoute.RemoveAt(0, 1);
		}
		else
		{
			group->pursuitRoute.Empty();
		}

		group->pursuing = true;
	}
	else
	{
		group->pursuitRoute.Empty();
	}
}

UMap::~UMap()
{
}

void UMap::BeginMapLoad()
{
	GetMain()->map_loader->LoadMap(GetParty()->areaString);
}

void UMap::Initialize()
{
	/*      loc7:Character
			loc8:FMapHex
			loc9:MovieClip
			mapHex:MapHex
			group1:int = 0;
			group2:int = 0;
			group3:Object
			group4:Rectangle
			group5:Number = NaN;
			group6:Number = NaN;
			group7:Object
			group8:Boolean = false;
			group9:Sprite
			loc20:MovieClip
			loc21:Boolean = false;
			loc22:Boolean = false;
			loc23:Boolean = false;
			loc24:int = 0;
			loc25:MovieClip
			loc26:Boolean = false;
			loc27:Boolean = false;
			loc28:NeutralPosition
			loc29:Object
			loc30:MovieClip
			loc31:FMapEncounter
			loc32:MovieClip
			loc33:MapHex
			loc34:NotificationR
			loc35:Music*/
	if (IsInitialized()) return;
	_initialized = true;
	inTransit = false;
	tileset = map_data.tileset;

	int32 nHexCounter = 0;
	int32 nHexSpecial = 0;
	while (nHexCounter < map_data.hex_list.Num())
	{
		FMapHexStruct mapHexStruct = map_data.hex_list[nHexCounter];
		if (mapHexStruct.ground_tile != "")
		{
			AMapHex* mapHex = Cast<AMapHex>(CreateMapHex());
			//TODO auto materials, currently messy UVs on border hexes
			GetCurrentPlayerController()->SetMapMaterialInstance("tunnel_Floor1" /*mapHexStruct.ground_tile*/, mapHex);
			mapHex->map_data = mapHexStruct;
			hexes.Add(mapHex);

			mapHex->SetActorRelativeLocation(FVector(mapHex->map_data.xc, mapHex->map_data.yc, 0));
			mapHex->SetActorRelativeRotation(FRotator(0, 90, 0));
			mapHex->SetActorRelativeScale3D(FVector(0.88 /*not 1?*/, 1.23, 0.001));

			mapHex->index = nHexSpecial;
			nHexSpecial++;

			mapHex->locationName = GetParty()->areaString + "_" + FString::FromInt(nHexCounter);
			mapHex->obstructed = mapHexStruct.obstructed;

			//TODO if obstructed, disable
			if (!mapHex->obstructed)
			{
				mapHex->pathIndex = pathHexes.Num() - 1;
				pathHexes.Add(mapHex);
			}

			//TODO detail tiles

			if (mapHexStruct.special_tile != "")
			{
				if (mapHexStruct.special_id != "")
				{
					mapHex->locationName = mapHexStruct.special_id;
				}

				bool isExit = false;
				if (mapHexStruct.special_type == FS_LOOT)
				{
					mapHex->type = "loot_" + mapHexStruct.special_tile;
				}
				else if (mapHexStruct.special_type == FS_ONESHOT_CAIRN)
				{
					mapHex->type = "recovery_full";
				}
				else if (mapHexStruct.special_type == FS_ONESHOT_ELFROOT)
				{
					mapHex->type = "recovery_health";
				}
				else if (mapHexStruct.special_type == FS_ONESHOT_SHROOMS)
				{
					mapHex->type = "recovery_power";
				}
				else if (mapHexStruct.special_type == FS_ONESHOT_SPRING)
				{
					mapHex->type = "recovery_partial";
				}
				else if (mapHexStruct.special_type == FS_NEUTRAL_POSITION)
				{
					UNeutralPosition* neutralPosition = NewObject<UNeutralPosition>();
					neutralPosition->name = mapHex->locationName;
					neutralPosition->homeHex = mapHex;
					npcs.Add(neutralPosition);
				}
				else if (mapHexStruct.special_type == FS_LEVEL_EXIT)
				{
					isExit = true;
				}

				//TODO If isExit = true;
			}

			int32 nEncounter = 0;
			while (nEncounter < map_data.encounter_list.Num())
			{
				FMapEncounterStruct encounterStruct = map_data.encounter_list[nEncounter];
				if (encounterStruct.hex_id == nHexCounter)
				{
					if (encounterStruct.ID.Contains("vs_"))
					{
						mapHex->locationName = "vs_" + encounterStruct.ID;
					}
					else
					{
						mapHex->locationName = encounterStruct.ID;
					}
					mapHex->type = "battle";
					mapHex->AddEnemies(mapHexStruct, encounterStruct);
					break;//?
				}

				nEncounter++;
			}

			if (mapHex->locationName.Contains("shop"))
			{
				mapHex->type = "shop";
			}
			else if (mapHex->locationName.Contains("dialog"))
			{
				mapHex->type = "dialog";
			}
			else
			{
				if ((mapHex->locationName.Contains("conv"))
					|| (IsConversationLocation(mapHex->locationName)))
				{
					mapHex->type = "conversation";
				}
				else if (GetDestination(mapHex->locationName).Num() > 0)
				{
					mapHex->type = "transition";
				}
			}

			if (mapHex->type == "transition")
			{
				transitions.Add(mapHex);
			}
			//TODO conversation vs dialog
			else if (mapHex->type == "conversation")
			{
				conversations.Add(mapHex);
			}
			//TODO dialog
			/*else if (mapHex->type == "dialog")
			{
				dialogs[mapHex->locationName]->hex = mapHex;
				dialogs[mapHex->locationName]->visited = false;
			}*/
			else if (mapHex->type.Contains("loot"))
			{
				specialHexes.Add(mapHex);
			}
			else if (mapHex->type.Contains("recovery"))
			{
				specialHexes.Add(mapHex);
			}
		}
		nHexCounter++;
	}

	UE_LOG(LogTemp, Warning, TEXT("AREA SCAN COMPLETE: hexes found %i ( %i objects ), hexes Num() %i"), nHexCounter, nHexSpecial, hexes.Num());

	CalculateAdjacency();
	//TODO Map Initialize
	//PrecomputeDistances();//TODO doublecheck, seems useless
	//partyPosition = layers.party_position;
	//add fog and lightMask
	//CreatePortraits();
	//UpdateQuestLocs();
	//for each (character in GetParty()->members)
	//GetParty()->ReceiveDelayedLoot();
	//UpdateNPCs(true);

	AMapHex* startHex = nullptr;
	if (hexes.Num() > 0)
	{
		//		TODO doublecheck
		// 		if (GetParty()->areaString == "")
		// 		{
		// 			startHex = GetLocationById(GetParty()->locationId);
		// 		}
		// 		else
		// 		{
		// 			startHex = GetLocationById(GetParty()->areaString);
		// 		}
		startHex = GetLocationById(GetParty()->locationId);

		if (startHex == nullptr)
		{
			startHex = hexes[0];
		}

		MoveToLocation(startHex, true);
	}

	SelectCharacter(GetParty()->members[selectedCharIndex]);

	//TODO
	AddSpecials();
	AddAvatars();

	//TODO activeQuest
	//TODO returnToTownButton
	//TODO CheckForQuestItems();
	//TODO LoadVisitedCache();
	InitVisibility();
	UpdateVisibility();
	UpdateMonsterVisibility();

	//TODO allow or not town travel
	//TODO update map objects based on quest results
	//TODO sound and music
	//TODO direction indicator
	//TODO remove Loading screen if needed, or other widgets
	//TODO do special actions
}

bool UMap::IsInitialized()
{
	return _initialized;
}

bool UMap::IsMoving()
{
	return route.Num() > 0;
}

bool UMap::CheckForTriggers(AMapHex* hex)
{
	//TODO dialog
	/*bool bHadDialog = false;
	if (dialogs[hex->locationName] != nullptr)
	{
		bHadDialog = TriggerDialog(hex->locationName, dialogs[hex->locationName]->visited);
		dialogs[hex->locationName]->visited = true;

		if (bHadDialog)
		{
			if (partyPosition)
			{
				partyPosition->EndMove();
			}

			route.Empty(); route.Add(hex);// = [hex];

			return true;
		}
	}*/

	if (questLocs.Contains(hex->locationName)
		&& questLocs[hex->locationName] == true)
	{
		if (partyPosition)
		{
			partyPosition->EndMove();
		}

		route.Empty(); route.Add(hex);

		return true;
	}

	return false;
}

int32 UMap::GetHexDistance(int32 nHexIndexA, int32 nHexIndexB)
{
	FVector2D vHexA = FVector2D(
		hexes[nHexIndexA]->GetTransform().GetLocation().X,
		hexes[nHexIndexA]->GetTransform().GetLocation().Y);
	FVector2D vHexB = FVector2D(
		hexes[nHexIndexB]->GetTransform().GetLocation().X,
		hexes[nHexIndexB]->GetTransform().GetLocation().Y);
	float fDistance = FVector2D::Distance(vHexA, vHexB);

	return FMath::CeilToInt(fDistance / 60);
}

void UMap::UpdateVisibility()
{
	if (!_visibilityEnabled)
	{
		return;
	}
	if (currentLocation)
	{
		int32 nCounter = 0;
		while (nCounter < hexes.Num())
		{
			if (FMath::Abs(hexes[nCounter]->GetTransform().GetLocation().X
				- currentLocation->GetTransform().GetLocation().X) > 500
				|| FMath::Abs(hexes[nCounter]->GetTransform().GetLocation().Y
					- currentLocation->GetTransform().GetLocation().Y) > 400)
			{

				hexes[nCounter]->SetActorHiddenInGame(true);
			}
			else
			{
				hexes[nCounter]->SetActorHiddenInGame(false);
			}
			if (!hexes[nCounter]->seen)
			{
				if (GetHexDistance(currentLocation->index, nCounter) <= GLOBAL_VIEW_DISTANCE)
				{
					hexes[nCounter]->seen = true;
					hexes[nCounter]->SetHexVisible(HIGHLIGHT_FLAG);
				}
			}
			++nCounter;
		}
	}
}

void UMap::InitVisibility()
{
	for (int32 i = 0; i < hexes.Num(); i++)
	{
		AMapHex* hex = hexes[i];
		if (hex->seen)
		{
			hex->SetHexVisible(HIGHLIGHT_FLAG);
		}
		else hex->SetHexVisible(0);
	}
}

void UMap::OnMapDataLoaded()
{
	Initialize();
}

AActor* UMap::CreateMapHex()
{
	FActorSpawnParameters SpawnParams;
	AMapHex* aHex = GetStage()->SpawnActor<AMapHex>(refMapHex, SpawnParams);

	return aHex;
}

void UMap::CalculateAdjacency()
{
	for (int32 i = 0; i < hexes.Num(); i++)
	{
		FindHexNeighbours(hexes[i]);
	}
}

void UMap::FindHexNeighbours(AMapHex* mapHex)
{
	float xLoc = 0.f;
	float yLoc = 0.f;
	int32 counter = 0;
	while (counter < hexes.Num())
	{
		xLoc = hexes[counter]->GetTransform().GetLocation().X - mapHex->GetTransform().GetLocation().X;
		yLoc = hexes[counter]->GetTransform().GetLocation().Y - mapHex->GetTransform().GetLocation().Y;
		if (xLoc * xLoc + yLoc * yLoc <= 3600)
		{
			AMapHex* aNeighbourHex = hexes[counter];
			mapHex->AddNeighbour(aNeighbourHex);
		}
		++counter;
	}
}

void UMap::UpdateCurrentLocation(AMapHex* mapHex)
{
	if (currentLocation != mapHex)
	{
		currentLocation = mapHex;
		(Cast<ADragonAgeJourneysCharacter>
			(GetStage()->GetFirstPlayerController()->GetPawn()))
			->GetCharRef()->group->mapHex = currentLocation; //DHK
		GetParty()->locationId = mapHex->locationName;
		AggroCheck();
		UpdateVisibility();
		UpdateVisitedCache();
	}
}

void UMap::AggroCheck()
{
	float fDistance = 0.f;

	if (aggroGroup != nullptr)
	{
		return;
	}

	for (int32 i = 0; i < enemyGroups.Num(); i++)
	{
		UGroup* group = enemyGroups[i];

		fDistance = GetHexDistance(currentLocation->index, group->mapHex->index);

		if (fDistance <= group->aggroRadius)
		{
			if (group->preventFightWithBarkId != "")
			{
				ShowBark(group->preventFightWithBarkId);
				continue;
			}

			if (group->preventFightWithDialogueId != "")
			{
				TriggerConversation("conv_tooweak", "");
				continue;
			}

			RunAggroTrigger(group, true, fDistance);
			return;
		}
	}
}

void UMap::AddAvatars()
{
	CreatePlayerAvatars();
}

void UMap::AddSpecials()
{
	//TODO AddSpecials
}

void UMap::CreatePlayerAvatars()
{

}

void UMap::SetGroupRotation(UPartyPosition* partyPosition, AMapHex* mapHex, bool bSwap)
{
	if (!GetMain()->Battlefield)
	{
		for (int32 l = 0; l < partyPosition->avatars.Num(); l++)
		{
			FRotator _rotation;
			if (bSwap)
				_rotation = UKismetMathLibrary::FindLookAtRotation(mapHex->GetActorLocation(), partyPosition->avatars[l]->GetActorLocation());
			else
				_rotation = UKismetMathLibrary::FindLookAtRotation(partyPosition->avatars[l]->GetActorLocation(), mapHex->GetActorLocation());

			_rotation.Pitch = 0;

			if (FMath::Abs(FMath::Abs(_rotation.Yaw) - FMath::Abs(partyPosition->avatars[l]->GetActorRotation().Yaw)) > 30)
			{
				partyPosition->avatars[l]->SetActorRotation(_rotation);
			}
		}
	}
}

void UMap::SetGroupSpeed(UPartyPosition* partyPosition, float fSpeed)
{
	if (!GetMain()->Battlefield)
	{
		for (int32 k = 0; k < partyPosition->avatars.Num(); k++)
		{
			if (partyPosition->avatars[k]->AnimInstance->GetMovementSpeed() != fSpeed)
			{
				partyPosition->avatars[k]->AnimInstance->SetMovementSpeed(fSpeed);
			}
		}
	}
}
