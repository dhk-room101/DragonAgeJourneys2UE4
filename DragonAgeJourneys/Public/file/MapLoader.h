// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "STypes.h"
#include "MapLoader.generated.h"

class UGroup;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMapDataLoad);

UCLASS()
class UMapLoader : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	FMapDataLoad MapDataLoad;
	void LoadMap(FString areaString);
	UGroup* GetEncounter(FString sEncounter);

private:
	virtual ~UMapLoader();
	void ParseMapDataXML(FName fMapDataXML);

	FMapData last_map;
};



