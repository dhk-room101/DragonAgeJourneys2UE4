// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"

#include "STypes.h"

#include "MapHex.generated.h"

const int32 SEEN_FLAG = 1;
const int32 VISITED_FLAG = 2;
const int32 HIGHLIGHT_FLAG = 3;

class UGroup;
class UNeutralPosition;

UCLASS()
class AMapHex : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	AMapHex();
	virtual void BeginPlay() override;
	void AddNeighbour(AMapHex* aNeighbourHex);
	void SetHexVisible(int32 nFlag);
	void MapHexClicked();
	bool AddEnemies(FMapHexStruct mapHexStruct, FMapEncounterStruct encounterStruct);
	bool AdjacentTo(AMapHex* adjHex);
	FString Visit(bool bMoving);

	FMapHexStruct map_data;
	UPROPERTY()
		UGroup* enemyGroup = nullptr;
	UPROPERTY()
		AMapHex* astar_parent = nullptr;
	UPROPERTY()
		UNeutralPosition* npc = nullptr;
	UPROPERTY()
		TArray<AMapHex*> neighbours;
	bool obstructed = false;
	bool seen = false;
	bool visited = false;
	int32 pathIndex = -1;
	int32 index = -1;
	FString locationName;
	FString type = "empty";
	FString enemyGroupId;
};
