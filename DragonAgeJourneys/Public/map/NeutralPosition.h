// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PartyPosition.h"
#include "NeutralPosition.generated.h"

UCLASS()
class UNeutralPosition : public UPartyPosition
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
		AMapHex* homeHex = nullptr;
	bool active = false;
	FString name;
	//UPROPERTY()
		//AMapInfoIndicator* infoIndicator = nullptr;

private:
	virtual ~UNeutralPosition() override;

	bool _initialized = false;
};



