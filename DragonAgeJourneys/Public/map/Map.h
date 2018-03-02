// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "STypes.h"
#include "Map.generated.h"

class UGameCharacter;
class UPartyPosition;
class UNeutralPosition;
class UGroup;
class AMapHex;
class ADragonAgeJourneysCharacter;

const int32 GLOBAL_VIEW_DISTANCE = 3;
//DHK
const float MAPSPEED = 45;//party moving speed

UCLASS()
class UMap : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void Added();
	void MapTick();
	void UpdateVisitedCache();
	void EndPlayerWalkSounds();
	void RemoveBattleIcon();
	void RemoveSpeechBubbleIcon();
	void RemoveRouteLine(bool bBool = false);
	void SelectCharacter(UGameCharacter* gameCharacter);
	void TriggerTransitionArea(FString sLocation);
	void AddEnemyGroup(UGroup* group);
	void UpdateMonsterVisibility();
	void RequestTravelAdjacentTo(AMapHex* mapHex, int32 nHops = -1);
	void RequestTravelTo(AMapHex* mapHex, int32 nHops = -1);
	bool MoveToLocation(AMapHex* mapHex, bool bTrigger = false);
	bool IsNPCVisible(FString sName);
	float h_distance(AMapHex* hexA, AMapHex* hexB);
	FVector MoveAlongRoute(AMapHex* mapHex, FVector vLoc, float fNormalize);
	AMapHex* GetLocationById(FString sString);
	TArray<AMapHex*> FindRouteBetween(AMapHex* hexA, AMapHex* hexB);

	FMapData map_data;
	UPROPERTY()
		TSubclassOf<class AMapHex> refMapHex;
	UPROPERTY()
		TSubclassOf<class AHex> refBattleHex;
	UPROPERTY()
		TSubclassOf<class ADragonAgeJourneysCharacter> refAvatar;
	UPROPERTY()
		AMapHex* currentLocation = nullptr;
	UPROPERTY()
		AMapHex* battlePendingHex = nullptr;
	UPROPERTY()
		AMapHex* conversationPendingHex = nullptr;
	UPROPERTY()
		AMapHex* targetLocation = nullptr;
	UPROPERTY()
		UPartyPosition* partyPosition = nullptr;
	UPROPERTY()
		UGameCharacter* selectedChar = nullptr;
	FString tileset;
	int32 selectedCharIndex = 0;
	bool suspendInput = false;
	bool bBattle = false;
	float MapZ = 0.f;
	float DeltaSeconds;

protected:
	void RunAggroTrigger(UGroup* group, bool bAlert, float fDistance = 1.f);

private:
	virtual ~UMap();
	void BeginMapLoad();
	void Initialize();
	void UpdateVisibility();
	void InitVisibility();
	void CalculateAdjacency();
	void FindHexNeighbours(AMapHex* mapHex);
	void UpdateCurrentLocation(AMapHex* mapHex);
	void AggroCheck();
	void AddAvatars();
	void AddSpecials();
	void CreatePlayerAvatars();
	void SetGroupRotation(UPartyPosition* partyPosition, AMapHex* mapHex, bool bSwap = false);
	void SetGroupSpeed(UPartyPosition* partyPosition, float fSpeed);
	bool IsInitialized();
	bool IsMoving();
	bool CheckForTriggers(AMapHex* hex);
	int32 GetHexDistance(int32 nHexIndexA, int32 nHexIndexB);
	AActor* CreateMapHex();
	
	bool _initialized = false;
	bool _visibilityEnabled = true;
	bool _loadedVisitCache = false;
	bool inTransit = false;
	FString areaString;
	UPROPERTY()
		UGroup* aggroGroup = nullptr;
	UPROPERTY()
		AMapHex* battleHex = nullptr;
	UPROPERTY()
		TArray<AMapHex*> route;
	UPROPERTY()
		TArray<AMapHex*> hexes;
	UPROPERTY()
		TArray<AMapHex*> pathHexes;
	UPROPERTY()
		TArray<AMapHex*> transitions;
	UPROPERTY()
		TArray<AMapHex*> conversations;
	UPROPERTY()
		TArray<AMapHex*> specialHexes;
	UPROPERTY()
		TArray<UNeutralPosition*> npcs;
	UPROPERTY()
		TArray<UGroup*> enemyGroups;
	UPROPERTY()
		TArray<ADragonAgeJourneysCharacter*> enemyAvatars;
	//UPROPERTY()
		//TMap<FString, AMapHex*> dialogs;
	UPROPERTY()
		TMap<FString, bool> questLocs;

	UFUNCTION()
		void OnMapDataLoaded();
};



