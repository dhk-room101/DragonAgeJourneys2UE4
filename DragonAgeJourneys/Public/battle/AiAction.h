// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AiAction.generated.h"

class UGameCharacter;
class USkill;
class AHex;

UCLASS()
class UAiAction : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void InitializeAiAction(UGameCharacter* gameChar);
	int32 GetActionValue() const;

	UPROPERTY()
		USkill* skill = nullptr;
	UPROPERTY()
		UGameCharacter* targetCharacter = nullptr;
	UPROPERTY()
		UGameCharacter* owner = nullptr;
	UPROPERTY()
		AHex* targetHex = nullptr;
	UPROPERTY()
		AHex* specialHex = nullptr;
	UPROPERTY()
		AHex* moveToHex = nullptr;
	int32 distance = -1;

private:
	virtual ~UAiAction();

	int32 GetRightmostEnemyPosition() const;
	bool DoesEnemyHaveThisVerticalPosition(int32 nGridY) const;

};



