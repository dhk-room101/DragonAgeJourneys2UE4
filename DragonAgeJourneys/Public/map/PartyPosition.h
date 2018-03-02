// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PartyPosition.generated.h"

class ADragonAgeJourneysCharacter;

UCLASS()
class UPartyPosition : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	float X = 0.f;
	float Y = 0.f;
	UPROPERTY()
		TArray<ADragonAgeJourneysCharacter*> avatars;

	void EndMove();
	void FaceTowards(float fPosX);
	void BeginMove(float fFacing = 0);
	void UpdatePartyPosition(FVector fVector);

	virtual ~UPartyPosition();

protected:
	bool moving = false;

private:
	bool _attacking = false;
};



