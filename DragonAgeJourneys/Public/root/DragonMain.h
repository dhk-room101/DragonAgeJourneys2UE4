// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DragonMain.generated.h"

/* Stencil index mapping to PP_OutlineColored */
const int32 STENCIL_FRIENDLY_OUTLINE = 252;
const int32 STENCIL_NEUTRAL_OUTLINE = 253;
const int32 STENCIL_ENEMY_OUTLINE = 254;
const int32 STENCIL_ITEMHIGHLIGHT = 255;

const float PerspectiveFOV = 45;
const float OrthoWidthExplore = 512;
const float OrthoWidthCombat = 704;
const FRotator RotPerspective = FRotator(0, 0, 0);
const FRotator RotOrtho = FRotator(-45, -90, 0);
const float CameraBoomLengthPerspective = 200;
const float CameraBoomLengthOrtho = 800;
const FVector TopDownCameraLocMainMenu = FVector(0, -155, 30);
const FVector TopDownCameraLocInGame = FVector(0, 0, 0);
const FRotator RotationMainMenu = FRotator(0, -45, 0);
const FRotator RotationInGame = FRotator(0, 0, 0);
const FVector LocationOutGame = FVector(0, 0, 96);
const FVector LocationInGame = FVector(0, 0, 0);

const float SpeedWalk = 112.5;
const float SpeedRun = 450;

//sockets
const FName fnLongswordOff = "LongswordOff";
const FName fnShieldOff = "ShieldOff";
const FName fnQuiver = "Quiver";
const FName fnLongbowOff = "LongbowOff";
const FName fnStaffOff = "StaffOff";
const FName fnDaggerRGTOff = "DaggerRGTOff";
const FName fnDaggerLFTOff = "DaggerLFTOff";
const FName fnWeaponRGTOn = "WeaponRGTOn";
const FName fnWeaponLFTOn = "WeaponLFTOn";
const FName fnShieldOn = "ShieldOn";

class UMapLoader;
class UGameData;
class UMap;
class UGroup;
class AMapHex;
class UBattlefield;

UCLASS()
class UDragonMain : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void Added();
	void StartGame();
	void CreationFinished();
	void BeginSinglePlayer();
	void BeginBattle(UGroup* enemyGroup, AMapHex* argHex = nullptr, int32 nTileset = -1, bool bIsScripted = false);

	UPROPERTY()
		UMapLoader* map_loader = nullptr;
	UPROPERTY()
		UGameData* gameData = nullptr;
	UPROPERTY()
		UMap* LevelMap = nullptr;
	UPROPERTY()
		UBattlefield* Battlefield = nullptr;

	//materials strings
	TArray<FString> metal;
	TArray<FString> wood;
	TArray<FString> leather;
	TArray<FString> precious;
	TArray<FString> cloth;
	TArray<FString> debug;

private:
	virtual ~UDragonMain();

	void MaterialConfigure();

	int32 _startTime = 0;
	int32 _previousTime = 0;

	UFUNCTION()
		void OnGameDataLoaded();

};



