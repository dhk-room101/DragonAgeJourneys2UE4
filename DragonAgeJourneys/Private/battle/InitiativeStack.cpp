#include "InitiativeStack.h"
#include "DragonAgeJourneys.h"
#include "GameCharacter.h"
#include "Algo/Reverse.h"

UInitiativeStack::UInitiativeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UInitiativeStack::AddCharacter(UGameCharacter* gameCharacter)
{
	_characters.Add(gameCharacter);
}

void UInitiativeStack::OrderStack()
{
	//_array.sortOn("battleInitiative", SORT_ARGS);
	_array.Sort(BattleInitiativeSorter);
}

void UInitiativeStack::BattleStart()
{
	float fInitiative = 0;
	for (int32 i = 0; i < _characters.Num(); i++)
	{
		UGameCharacter* gameChar = _characters[i];
		gameChar->initiativeRoll = FMath::FRandRange(0, 10) + fInitiative;
		fInitiative = fInitiative + 0.1;
	}
}

void UInitiativeStack::BuildStack()
{
	UE_LOG(LogTemp, Warning, TEXT("InitiativeStack.build() - %i characters"), _characters.Num());
	for (int32 i = 0; i < _characters.Num(); i++)//slice
	{
		_array.Add(_characters[i]);
	}
	OrderStack();
	UE_LOG(LogTemp, Warning, TEXT("new array length: %i"), _array.Num());
}

UGameCharacter* UInitiativeStack::PopStack()
{
	if (_array.Num() < 1)
	{
		BuildStack();
	}
	
	return _array.Pop();
}

TArray<UGameCharacter*> UInitiativeStack::GetStack()
{
	return _array;
}

TArray<UGameCharacter*> UInitiativeStack::GetOrderedList()
{
	TArray<UGameCharacter*> orderedListAll;
	for (int32 i = 0; i < _characters.Num(); i++)
	{
		orderedListAll.Add(_characters[i]);
	}
	
	//orderedListAll.sortOn("battleInitiative", SORT_ARGS | Array.DESCENDING);
	orderedListAll.Sort(BattleInitiativeSorter);
	Algo::Reverse(orderedListAll);
	
	int32 nIndex = orderedListAll.Num() - 1;
	while (nIndex >= 0)
	{
		UGameCharacter* gameChar = orderedListAll[nIndex];
		if (gameChar->health <= 0)
		{
			orderedListAll.RemoveAt(nIndex, 1);
		}
		--nIndex;	
	}

	TArray<UGameCharacter*> orderedList;
	for (int32 i = 0; i < orderedListAll.Num(); i++)
	{
		orderedList.Add(orderedListAll[i]);
	}

	while (orderedListAll.Num() < 10)
	{
		orderedListAll.Append(orderedList);
	}

	return orderedListAll;
}

UInitiativeStack::~UInitiativeStack()
{
}
