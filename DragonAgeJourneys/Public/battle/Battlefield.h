// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "STypes.h"
#include "GameCharacter.h"
#include "Battlefield.generated.h"

class USkill;
class UGroup;
class UAction;
class UAiAction;
class UInitiativeStack;
class UDijkstra;
class AHex;
class ADragonAgeJourneysCharacter;
class ATrap;

const int32 ANNOUNCE_TEXT_COLOR = 13547636;//CEB874
const int32 LOW_HEALTH_PERCENT = 30;
const int32 HEX_Y_MIN = 165;
const int32 HEX_X_MIN_EVEN = 170;
const int32 HEX_X_MIN_ODD = 201;
const int32 NUM_HEXES_X = 8;
const int32 NUM_HEXES_Y = 9;
const float LOS_TEST_FREQ = 5;
const float BATTLESPEED = 19;//DHK

UCLASS()
class UBattlefield : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	inline static bool LatestHateSorter(const UGameCharacter& character1, const UGameCharacter& character2)
	{
		return character1.latestHate < character2.latestHate;
	}

	void InitializeBattlefield(TArray<UGroup*> battleGroups, int32 nTileset);
	void BattlefieldTick();
	void PassTurn();
	void RequestMoveTo(AHex* hex);
	void HighlightSkillTargets(USkill* skill = nullptr);
	void UseInstantSkill(USkill* skill);
	void ToggleOffSkill(USkill* skill, bool bTurnOff = false, bool arg3 = true);
	void ShowMovementPath(AHex* hex);
	void ClearMovementPath();
	void SetMoveActionForHex(AHex* hex);
	void SetPlayerActionForHex(AHex* hex);
	void SetActionIndicatorForHex(AHex* hex);
	void SetActionTextForHex(AHex* hex);
	void ClearActionText();
	void SetAvatarRotation(ADragonAgeJourneysCharacter* avatar, AHex* hex);
	void SetAvatarSpeed(ADragonAgeJourneysCharacter* avatar, float fSpeed);
	int32 FindMovementIndexForPlayerSkill(AHex* hex, USkill* skill);
	int32 CountCorpses();
	int32 CalculateDamage(USkill* skill, UGameCharacter* attacker, UGameCharacter* target, bool bGlancing);
	int32 GetRawDistanceBetweenHexes(AHex* hex1, AHex* hex2);
	int32 GetClosestEnemyDistanceTo(UGameCharacter* gameCharacter);
	int32 GetDistanceFromMonster(UGameCharacter* gameCharacter, AHex* hex);
	float GetHexDistanceOccupied(AHex* hex, UDijkstra* dijkstra = nullptr);
	bool AreAllies(UGameCharacter* gameChar1, UGameCharacter* gameChar2);
	bool IsPlayerTurn();
	bool LineOfSightBetween(AHex* hexStart, AHex* hexEnd, UGameCharacter* gameCharacter);
	bool IsFlankAttack(UGameCharacter* gameChar1, UGameCharacter* gameChar2, USkill* skill, AHex* hex = nullptr);
	bool IsBackstab(UGameCharacter* gameChar1, UGameCharacter* gameChar2, USkill* skill);
	bool PlayerActionAllowed();
	bool IsMoving();
	UGroup* GetEnemyGroup();
	TArray<AHex*> GetHexesInRange(int32 nMoves);
	TArray<AHex*> GetHexesInMoveRange();

	//TODO TRAP
	ATrap* TrapOnHex(AHex* hex);
	TArray<ATrap*> traps;

	UPROPERTY()
		TSubclassOf<class AHexDirection> refHexDirection;
	UPROPERTY()
		TSubclassOf<class ATrap> refTrap;
	UPROPERTY()
		AHexDirection* directionArrow = nullptr;

	FString name;
	bool startedByScripting = false;
	bool allowObstacles = false; //TODO make it true
	int32 waitFrames = 0;
	float DeltaSeconds;
	UPROPERTY()
		UGameCharacter* currentTurnCharacter = nullptr;
	UPROPERTY()
		AHex* currentTurnStartHex = nullptr;
	UPROPERTY()
		AHex* targetHex = nullptr;
	UPROPERTY()
		UDijkstra* currentTurnMoveSearch = nullptr;
	UPROPERTY()
		ADragonAgeJourneysCharacter* currentAvatar = nullptr;
	UPROPERTY()
		UAction* currentAction = nullptr;
	UPROPERTY()
		TArray<UGameCharacter*> characters;
	UPROPERTY()
		TArray<UAction*> actionQueue;
	UPROPERTY()
		TArray<AHex*> hexes;
	UPROPERTY()
		TArray<FInt32Array> rawDistances;

private:
	virtual ~UBattlefield();
	void SetupCharacters();
	void GenerateAvatars();
	void SetBattleView();
	void GenerateHexNeighbours();
	void PrecomputeDistances();
	void RemoveStealthEffects(UGameCharacter* gameCharacter);
	void SetLargeCreatureHexes(UGameCharacter* gameCharacter);
	void SetMovementHexes(/*int32 nSpeed*/);
	void SetDirectHexes(int32 nDistance, USkill* skill);
	void SetRangedHexes(USkill* skill);
	void SetShadowedHexes(AHex* hexFrom, AHex* hexTo, int32 nDistance);
	void MarkTargetHexForSkill(USkill* skill, int32 nDistance, AHex* hex);
	void AddBars(bool bCurrent = false);
	void ProcessActions();
	void EndMove();
	void EndTurn();
	void NextTurn();
	void AiTurn();
	void TestAuras();
	void ResetHexes();
	void UpdateHexes();
	void DoEndBattle();
	void ExitBattle();
	void AddActionsForAI(UAiAction* aiAction);
	void AddSkillActions(USkill* skill, UGameCharacter* gameCharacter, int32 nIndex, int32 nRepeat = -1, UAiAction* aiAction = nullptr, bool bCounterAttack = false);
	void AddAuraToFriendlies(USkill* skill, UGameCharacter* gameCharacter);
	void BeginSkillAnimation(UAction* action);
	void RequestSkillOn(AHex* hex, USkill* skill);
	void DoSkillEffect(AHex* hex, USkill* skill);
	void CheckForCounterAttack(UGameCharacter* attacker, UGameCharacter* target, USkill* skill);
	void HexNeighboursHelper(int32 nGridX, int32 nGridY, TArray<AHex*>& tArray);
	bool HexesAdjacent(AHex* hexA, AHex* hexB);
	bool IsBehindTargetBack(ADragonAgeJourneysCharacter* avatar1, ADragonAgeJourneysCharacter* avatar2);
	bool DoesSkillMiss(UGameCharacter* gameCharacter, USkill* skill, int32 nIndex);
	bool TrySpringTrap(UGameCharacter* gameCharacter, ATrap* trap);
	bool IsPlayingAnim();
	TArray<FInt32Array> BuildAdjacencyMap();
	TArray<AHex*> HexNeighbours(AHex* hex, bool bHelperIncludeNull = false);
	TArray<AHex*> HexNeighboursForLargeCreature(AHex* hex, UGameCharacter* gameCharacter);
	TArray<AHex*> GetHexesBetween(AHex* hexStart, AHex* hexEnd);
	TArray<AHex*> GetRouteTo(AHex* hex, UDijkstra* dijkstra = nullptr);
	TArray<AHex*> GetHexesUnderTemplate(AHex* hex, USkill* skill);
	TArray<AHex*> GetHexesInLine(AHex* hex, int32 nDirection, int32 nAmount );
	AHex* HexInGrid(int32 nGridX, int32 nGridY);
	AHex* GetCurrentHex();
	AHex* GetHexAtPoint(FVector vPoint);
	AHex* FindReviveHexFor(ADragonAgeJourneysCharacter* avatar);
	UGameCharacter* GetCurrentCharacter();
	float HexDistance(AHex* hex, UDijkstra* dijkstra = nullptr);
	int32 SubtractPowerForSkill(USkill* skill);
	int32 CalculateAccuracy(UGameCharacter* gameCharacter, AHex* hex, USkill* skill);

	UPROPERTY()
		ADragonAgeJourneysCharacter* battleView = nullptr;
	UPROPERTY()
		TArray<UGroup*> groups;
	UPROPERTY()
		TArray<FHexArray> grid;
	UPROPERTY()
		TArray<FInt32Array> movementMap;
	UPROPERTY()
		TArray<AHex*> moveRoute;
	UPROPERTY()
		TArray<AHex*> obstructedHexes;
	UPROPERTY()
		TArray<UGameCharacter*> allies;
	UPROPERTY()
		TArray<UGameCharacter*> enemies;
	UPROPERTY()
		TArray<ADragonAgeJourneysCharacter*> avatars;
	UPROPERTY()
		UInitiativeStack* initStack = nullptr;
	UPROPERTY()
		UDijkstra* dSearch = nullptr;
	UPROPERTY()
		UGroup* defeatedGroup = nullptr;
	UPROPERTY()
		USkill* _skillTargetsHighlightPendingFor = nullptr;
	bool helperIncludeNull = false;
	bool suspendTurnOrder = false;
	bool endBattle = false;
	bool paused = false;
	bool _battleEnded = false;
	bool _generatingCharacters = false;
	bool _highlightedSkillTargetsThisFrame = false;
	int32 _generatedCharacterCount = 0;
	int32 waitingForActionFrame = 0;
	int32 frameCount = 0;
	FString battleResult;
};



