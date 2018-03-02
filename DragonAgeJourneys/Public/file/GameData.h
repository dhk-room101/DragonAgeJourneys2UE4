// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "STypes.h"
#include "GameData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameDataLoad);

UCLASS()
class UGameData : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void LoadData();
	FGameDataLoad GameDataLoad;

private:
	virtual ~UGameData();
	void ParseGameDataXML(FName fGameDataXML);
	FGameDataStruct GameDataXML;
};



