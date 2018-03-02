// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "InitiativeStack.generated.h"

class UGameCharacter;

UCLASS()
class UInitiativeStack : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	inline static bool BattleInitiativeSorter(const UGameCharacter& character1, const UGameCharacter& character2)
	{
		return character1.GetBattleInitiative() < character2.GetBattleInitiative();
	}

	void AddCharacter(UGameCharacter* gameCharacter);
	void OrderStack();
	void BattleStart();
	void BuildStack();
	UGameCharacter* PopStack();
	TArray<UGameCharacter*> GetStack();
	TArray<UGameCharacter*>  GetOrderedList();

private:
	virtual ~UInitiativeStack();

	int32 SORT_ARGS = 16;
	UPROPERTY()
		TArray<UGameCharacter*> _array;
	UPROPERTY()
		TArray<UGameCharacter*> _characters;

};



