#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"

#include "STypes.h"
#include "Hex.generated.h"

const int32 OBSTACLE_NONE = 0;
const int32 OBSTACLE_SMALL = 1;
const int32 OBSTACLE_LARGE = 2;
const int32 HEX_WIDTH = 62;
const int32 HEX_HEIGHT = 50;
const int32 HEX_TIP_HEIGHT = 13;
const int32 HEX_MID_HEIGHT = HEX_HEIGHT - 2 * HEX_TIP_HEIGHT;
const int32 HEX_UP_RIGHT = 0;
const int32 HEX_RIGHT = 1;
const int32 HEX_DOWN_RIGHT = 2;
const int32 HEX_DOWN_LEFT = 3;
const int32 HEX_LEFT = 4;
const int32 HEX_UP_LEFT = 5;
const int32 HEX_OBSTACLE_NONE = 0;
const int32 HEX_OBSTACLE_SMALL = 1;
const int32 HEX_OBSTACLE_LARGE = 2;
const int32 HEX_MODE_NORMAL = 1;
const int32 HEX_MODE_MOVE = 2;
const int32 HEX_MODE_SELECTED = 3;
const int32 HEX_MODE_TARGET = 4;
const int32 HEX_MODE_MOVEATTACK = 5;
const int32 HEX_MODE_RANGED = 6;
const int32 HEX_MODE_SHADOWED = 7;
const int32 HEX_MODE_SHOWPATH = 8;//DHK
const float LOS_MID_PERCENT = HEX_TIP_HEIGHT / (HEX_MID_HEIGHT + HEX_TIP_HEIGHT);

class ADragonAgeJourneysCharacter;
class UGameCharacter;

UCLASS()
class AHex : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	AHex();
	void BattleHexClicked();
	void AttachAvatar(ADragonAgeJourneysCharacter* avatar);
	void DetachAvatar();
	void HexOut();
	void HexClicked();
	void HexOver();
	void UpdateHex();
	void SetMode(int32 nMode, int32 nSkill);
	void ShowDirection();
	void RemoveDirection();
	int32 GetMode();
	TArray<ADragonAgeJourneysCharacter*> GetNeighbourAvatars(bool bLarge = false);
	ADragonAgeJourneysCharacter* GetAnAvatar();
	FString StringData();
	bool CanShoot(UGameCharacter* gameCharacter);
	bool CanMove(UGameCharacter* gameCharacter);
	bool CanEndMoveHere(UGameCharacter* gameCharacter);
	bool ModeSkillUpToDate();

	//indexField : TextField;
	//direction : MovieClip;
	//stageDirection : MovieClip;
	int32 gridX = 0;
	int32 gridY = 0;
	int32 index = -1;
	int32 nHeight = 0;
	int32 nWidth = 0;
	UPROPERTY()
		TArray<AHex*> attackerHexes;
	UPROPERTY()
		TArray<AHex*> neighbours;
	bool obstructedTestFailed = false;
	int32 obstacle = 0;
	//visualObstacle : DisplayObject = nullptr;
	UPROPERTY()
		AHex* directionHex = nullptr;
	UPROPERTY()
		ADragonAgeJourneysCharacter* avatar = nullptr;
	UPROPERTY()
		ADragonAgeJourneysCharacter* largeAvatar = nullptr;

private:
	void CheckTouch();
	bool SameSide(FVector2D p1, FVector2D p2, FVector2D a, FVector2D b);


	EHexMode _mode = EHexMode::INVALID;
	int32 _modeSkillId = -1;

	UPROPERTY()
		FTimerHandle tTouch;
	int32 nTouch = 0;
};