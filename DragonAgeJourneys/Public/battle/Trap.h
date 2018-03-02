#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"

#include "Trap.generated.h"

class UGameCharacter;
class AHex;

UCLASS()
class ATrap : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	ATrap();
	int32 GetDamage();

	UGameCharacter* owner = nullptr;
	AHex* hex = nullptr;
	int32 level = 1;
	bool active = true;
};