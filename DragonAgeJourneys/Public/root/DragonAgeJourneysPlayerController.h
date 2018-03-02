// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "STypes.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DragonAgeJourneysPlayerController.generated.h"

class UMaterial;
class UDataTable;
class AMapHex;
class AHex;
class ADragonAgeJourneysCharacter;

UCLASS()
class ADragonAgeJourneysPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADragonAgeJourneysPlayerController();

	void SetCurrentGameMode(EUI eUI);
	void SetGameModeCamera();
	void SetMapMaterialInstance(FString sMaterial, AActor* aActor);
	void SetBattleHexMaterialInstance(int32 nMaterial, AActor* aActor);
	void DisplayFloaty(ADragonAgeJourneysCharacter* avatar, FString sMessage, int32 nStyle = 0 /*FLOATY_MESSAGE*/, int32 nColor = 16777215 /*White*/, float fDuration = 1.f);
	void UIPositionWidget(EUI eUI);
	FVector2D UIGetPosition(EUI eUI);

	//widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidgetQuickBar> wUIQuickBar;
	UPROPERTY()
		UUserWidgetQuickBar* UIQuickBar = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidgetMainMenu> wUIMainMenu;
	UPROPERTY()
		UUserWidgetMainMenu* UIMainMenu = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidgetCharGen> wUICharGen;
	UPROPERTY()
		UUserWidgetCharGen* UICharGen = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidgetFloatyText> wUIFloatyText;
	UPROPERTY()
		TArray<UUserWidgetFloatyText*> floaties;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tables")
		TMap<ETable, UDataTable*> tables;

	UPROPERTY()
		TMap<int32, UMaterial*> battleHexMaterials;
	UPROPERTY()
		TMap<int32, UTexture2D*> skillTextures;
	UPROPERTY()
		UTexture2D* SkillEmpty;
	UPROPERTY()
		FVector BattleTraceImpact;

	UPROPERTY()
		EUI currentGameMode = EUI::MAINMENU;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	
private:
	void HandleFloatyTick();
	void InitializeUI();
	void RepositionUIMainMenu();
	void SetBuildSettings();
	void LoadMapMaterials();
	void LoadBattleHexMaterials();
	void LoadSkillTextures();
	AMapHex* GetMapHex();
	AHex* GetBattleHex();

	UPROPERTY()
		TMap<FString, UMaterial*> mapMaterials;
	UPROPERTY()
		FTimerHandle tTimerMainMenu;
};


