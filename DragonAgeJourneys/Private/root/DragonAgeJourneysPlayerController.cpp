// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DragonAgeJourneysPlayerController.h"
#include <WidgetLayoutLibrary.h>
#include <WidgetBlueprintLibrary.h>
#include "Kismet/KismetMathLibrary.h"
#include <Engine.h>
#include <Paths.h>
#include <ConstructorHelpers.h>
#include "AI/Navigation/NavigationSystem.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Engine/StaticMesh.h"
#include "DragonAgeJourneysCharacter.h"
#include "BuildSettings.h"
#include "DragonAgeJourneysLibrary.h"
#include "DragonAgeJourneysSingleton.h"
#include "Scripting.h"
#include "EmbedList.h"
#include "../XmlParser/Public/XmlParser.h"
#include "MapHex.h"
#include "Group.h"
#include "GameCharacter.h"
#include "Hex.h"
#include "UserWidgetQuickBar.h"
#include "UserWidgetMainMenu.h"
#include "UserWidgetCharGen.h"
#include "UserWidgetFloatyText.h"
#include "WidgetLayoutLibrary.h"
#include "DragonAgeJourneysAnimInstance.h"

ADragonAgeJourneysPlayerController::ADragonAgeJourneysPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	LoadMapMaterials();
	LoadBattleHexMaterials();
	LoadSkillTextures();
}

void ADragonAgeJourneysPlayerController::BeginPlay()
{
	InitializeUI();

	SetBuildSettings();

	GetAssets();
	GetAssets()->embed_list = NewObject<UEmbedList>();

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("%s: New Assets created!"), *GetClass()->GetName());
#endif // DEBUG

	//set current world
	bool IsValid;
	UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysCurrentWorld = GetWorld();
#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("%s: Setting stage!"), *GetWorld()->GetClass()->GetName());
#endif // DEBUG

	//create DragonMain
	GetMain()->Added();

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("%s: Setting Main"), *UDragonAgeJourneysLibrary::GetDragonAgeJourneysData(IsValid)->DragonAgeJourneysMain->GetClass()->GetName());
#endif // DEBUG

	/*
	dragon_main Begin Play
	dragon_main added
	dragon_main onGameDataLoaded
	dragon_main mainMenu
	TitleScreen
	dragon_main startGame
	CharacterCreation
	OriginStory
	dragon_main creationFinished
	dragon_main beginSinglePlayer
	*/

	/*
	added
	this.map_loader = new MapLoader();

	newGameClick
	startGame
	areaString = tut_area

	creationFinished()
	beginSinglePlayer
	this.map = new Map(dm.party.areaString, _loc_1);
	new LoadingScreen(this.beginMapLoad);
	MapLoader.PTR.load_map(this.areaString, this.load_callback);
	initialize()
	*/

	//DragonMain

	//TODO USE_DEBUG_PANE
	/*if (USE_DEBUG_PANE)
	{
		stage.addChild(new DebugLogPane(DebugLog.PTR));
	}*/

}

void ADragonAgeJourneysPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (floaties.Num() > 0)
	{
		HandleFloatyTick();
	}

	// keep updating the destination every tick while desired
	/*if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}*/
}

void ADragonAgeJourneysPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ADragonAgeJourneysPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ADragonAgeJourneysPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ADragonAgeJourneysPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ADragonAgeJourneysPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ADragonAgeJourneysPlayerController::OnResetVR);
}

void ADragonAgeJourneysPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ADragonAgeJourneysPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ADragonAgeJourneysCharacter* MyPawn = Cast<ADragonAgeJourneysCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ADragonAgeJourneysPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ADragonAgeJourneysPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ADragonAgeJourneysPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	//bMoveToMouseCursor = true;

	if (!GetMain()->Battlefield)
	{
		AMapHex* mapHex = NewObject<AMapHex>();
		mapHex = GetMapHex();
		if (mapHex)
			mapHex->MapHexClicked();
	}
	else
	{
		AHex* battleHex = NewObject<AHex>();
		battleHex = GetBattleHex();
		if (battleHex)
			battleHex->BattleHexClicked();
	}
}

void ADragonAgeJourneysPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	//bMoveToMouseCursor = false;
}

void ADragonAgeJourneysPlayerController::SetCurrentGameMode(EUI eUI)
{
	currentGameMode = eUI;
	SetGameModeCamera();
	UIPositionWidget(currentGameMode);

	if (currentGameMode == EUI::MAINMENU)
	{
		UIMainMenu->SetVisibility(ESlateVisibility::Visible);
		//UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, UIMainMenu);
	}
	else if (currentGameMode == EUI::CHARGEN)
	{
		UICharGen->CharGenUpdateSelection();
		UIMainMenu->SetVisibility(ESlateVisibility::Hidden);
		UICharGen->SetVisibility(ESlateVisibility::Visible);
		//UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, UICharGen);
	}
	else if (currentGameMode == EUI::EXPLORE)
	{
		UIQuickBar->SetVisibility(ESlateVisibility::Hidden);
		UICharGen->SetVisibility(ESlateVisibility::Hidden);
		//UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	}
	else if (currentGameMode == EUI::COMBAT)
	{
		UIQuickBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void ADragonAgeJourneysPlayerController::SetGameModeCamera()
{
	ADragonAgeJourneysCharacter* playerAvatar = Cast<ADragonAgeJourneysCharacter>(GetPawn());

	if (currentGameMode == EUI::MAINMENU)
	{
		playerAvatar->SetActorRelativeRotation(RotationMainMenu);
		playerAvatar->GetTopDownCameraComponent()->SetProjectionMode(ECameraProjectionMode::Perspective);
		playerAvatar->GetTopDownCameraComponent()->FieldOfView = PerspectiveFOV;

		playerAvatar->GetCameraBoom()->SetWorldRotation(RotPerspective);
		playerAvatar->GetCameraBoom()->TargetArmLength = CameraBoomLengthPerspective;
		playerAvatar->GetTopDownCameraComponent()->SetRelativeLocationAndRotation(TopDownCameraLocMainMenu, RotPerspective);
	}
	else if (currentGameMode == EUI::CHARGEN)
	{
		float _z = GetPawn()->GetActorRotation().Yaw;

		playerAvatar->PlayCustomAnimation("mh_dg_tn_90_l");
	}
	else if (currentGameMode == EUI::EXPLORE)
	{
		playerAvatar->SetActorRelativeRotation(RotationMainMenu);
		playerAvatar->GetTopDownCameraComponent()->SetProjectionMode(ECameraProjectionMode::Orthographic);
		playerAvatar->GetTopDownCameraComponent()->OrthoWidth = OrthoWidthExplore;

		playerAvatar->GetCameraBoom()->SetWorldRotation(RotOrtho);
		playerAvatar->GetCameraBoom()->TargetArmLength = CameraBoomLengthOrtho;
		playerAvatar->GetTopDownCameraComponent()->SetRelativeLocation(TopDownCameraLocInGame);
	}
}

void ADragonAgeJourneysPlayerController::SetMapMaterialInstance(FString sMaterial, AActor* aActor)
{
	UMaterial** mat = mapMaterials.Find(sMaterial);
	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(*mat, aActor);

	TArray<UStaticMeshComponent*> Components;
	aActor->GetComponents<UStaticMeshComponent>(Components);
	for (int32 i = 0; i < Components.Num(); i++)
	{
		UStaticMeshComponent* StaticMeshComponent = Components[i];
		UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
		StaticMeshComponent->SetMaterial(0, MaterialInstance);
	}
}

void ADragonAgeJourneysPlayerController::SetBattleHexMaterialInstance(int32 nMaterial, AActor* aActor)
{
	UMaterial** mat = battleHexMaterials.Find(nMaterial);
	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(*mat, aActor);

	TArray<UStaticMeshComponent*> Components;
	aActor->GetComponents<UStaticMeshComponent>(Components);
	for (int32 i = 0; i < Components.Num(); i++)
	{
		UStaticMeshComponent* StaticMeshComponent = Components[i];
		UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
		StaticMeshComponent->SetMaterial(0, MaterialInstance);
	}
}

void ADragonAgeJourneysPlayerController::DisplayFloaty(ADragonAgeJourneysCharacter* avatar, FString sMessage, int32 nStyle /*= 0 /*FLOATY_MESSAGE*/, int32 nColor /*= 16777215 /*White*/, float fDuration /*= 1.f*/)
{

	if (wUIFloatyText)
	{
		UUserWidgetFloatyText* UIFloatyText = CreateWidget<UUserWidgetFloatyText>(this, wUIFloatyText);

		if (UIFloatyText)
		{
			UIFloatyText->SetVisibility(ESlateVisibility::Hidden);
			UIFloatyText->AddToViewport();

			UIFloatyText->Floaty->SetText(FText::FromString(sMessage));
			UIFloatyText->Floaty->SetJustification(ETextJustify::Center);

			//set color, convert from 0-255/0xFF to 0-1 range
			float r = ((nColor >> 16) & 255) / 255.f;
			float g = ((nColor >> 8) & 255) / 255.f;
			float b = (nColor & 255) / 255.f;
			float a = 255.f / 255.f;
			UIFloatyText->Floaty->SetColorAndOpacity(FLinearColor(r, g, b, a));

			//set owner
			UIFloatyText->avatar = avatar;
			//timestamp and duration
			UIFloatyText->fTimestamp = GetWorld()->GetTimeSeconds();
			UIFloatyText->fDuration = fDuration;
			//set initial position relative to owner
			float BoundingRadius = avatar->GetRootComponent()->Bounds.SphereRadius;
			UIFloatyText->vPosition = avatar->GetActorLocation() + *(new FVector(0, 0, BoundingRadius));

			UIFloatyText->SetVisibility(ESlateVisibility::Visible);
			floaties.Add(UIFloatyText);
		}
	}
}

void ADragonAgeJourneysPlayerController::UIPositionWidget(EUI eUI)
{
	if (eUI == EUI::MAINMENU)
	{
		UIMainMenu->SetPositionInViewport(UIGetPosition(EUI::MAINMENU));
		UIMainMenu->SelectDefaultButton();
	}
	else if (eUI == EUI::COMBAT)
		UIQuickBar->SetPositionInViewport(UIGetPosition(EUI::QUICKBAR));
	else if (eUI == EUI::CHARGEN)
		UICharGen->SetPositionInViewport(UIGetPosition(EUI::CHARGEN));
}

FVector2D ADragonAgeJourneysPlayerController::UIGetPosition(EUI eUI)
{
	UCanvasPanel* Buttons = nullptr;
	if (eUI == EUI::MAINMENU)
		Buttons = (UCanvasPanel*)UIMainMenu->GetWidgetFromName(TEXT("Buttons"));
	else if (eUI == EUI::QUICKBAR)
		Buttons = (UCanvasPanel*)UIQuickBar->GetWidgetFromName(TEXT("Buttons"));
	else if (eUI == EUI::CHARGEN)
		Buttons = (UCanvasPanel*)UICharGen->GetWidgetFromName(TEXT("Buttons"));

	if (Buttons)
	{
		FVector2D vSize = UWidgetLayoutLibrary::SlotAsCanvasSlot(Buttons)->GetSize();
		float fScale = UWidgetLayoutLibrary::GetViewportScale(UIQuickBar);
		FVector2D vSizeScaled = FVector2D(vSize.X*fScale, vSize.Y*fScale);

		FVector2D vDefaultSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

		if (eUI == EUI::MAINMENU || eUI == EUI::CHARGEN)
			return FVector2D(0, vDefaultSize.Y / 2 - vSizeScaled.Y / 2);
		else if (eUI == EUI::QUICKBAR)
			return FVector2D(vDefaultSize.X / 2 - vSizeScaled.X / 2, vDefaultSize.Y - vSizeScaled.Y);
	}

	return FVector2D();
}

void ADragonAgeJourneysPlayerController::InitializeUI()
{
	//prepare main menu UI
	if (wUIMainMenu)
	{
		UIMainMenu = CreateWidget<UUserWidgetMainMenu>(this, wUIMainMenu);

		if (UIMainMenu)
		{
			UIMainMenu->SetVisibility(ESlateVisibility::Hidden);
			UIMainMenu->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(tTimerMainMenu, this, &ADragonAgeJourneysPlayerController::RepositionUIMainMenu, 0.25f, false);

	//QuickBar
	if (wUIQuickBar)
	{
		UIQuickBar = CreateWidget<UUserWidgetQuickBar>(this, wUIQuickBar);

		if (UIQuickBar)
		{
			UIQuickBar->SetVisibility(ESlateVisibility::Hidden);
			UIQuickBar->AddToViewport();
		}
	}

	if (wUICharGen)
	{
		UICharGen = CreateWidget<UUserWidgetCharGen>(this, wUICharGen);

		if (UICharGen)
		{
			UICharGen->SetVisibility(ESlateVisibility::Hidden);
			UICharGen->AddToViewport();
		}
	}

	//the last one is rendered on top of the rest
}

//stupid delay :D
void ADragonAgeJourneysPlayerController::RepositionUIMainMenu()
{
	SetCurrentGameMode(EUI::MAINMENU);
}

void ADragonAgeJourneysPlayerController::SetBuildSettings()
{
	Config();

#ifdef DEBUG
	FString sBool = (RELEASE_BUILD == true) ? "TRUE" : "FALSE";
	UE_LOG(LogTemp, Warning, TEXT("%s: %i - %s"), *GetClass()->GetName(), CONFIGURATION, *sBool);
#endif // DEBUG
}

void ADragonAgeJourneysPlayerController::LoadMapMaterials()
{
	//Tunnels
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderE1_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderE1_Mat"));
	mapMaterials.Add("tunnel_borderE1", tunnel_borderE1_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderN1_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderN1_Mat"));
	mapMaterials.Add("tunnel_borderN1", tunnel_borderN1_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderNE1_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderNE1_Mat"));
	mapMaterials.Add("tunnel_borderNE1", tunnel_borderNE1_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderNE2_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderNE2_Mat"));
	mapMaterials.Add("tunnel_borderNE2", tunnel_borderNE2_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderNE3_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderNE3_Mat"));
	mapMaterials.Add("tunnel_borderNE3", tunnel_borderNE3_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderNW1_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderNW1_Mat"));
	mapMaterials.Add("tunnel_borderNW1", tunnel_borderNW1_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderNW2_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderNW2_Mat"));
	mapMaterials.Add("tunnel_borderNW2", tunnel_borderNW2_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderS1_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderS1_Mat"));
	mapMaterials.Add("tunnel_borderS1", tunnel_borderS1_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderSE1_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderSE1_Mat"));
	mapMaterials.Add("tunnel_borderSE1", tunnel_borderSE1_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderSE2_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderSE2_Mat"));
	mapMaterials.Add("tunnel_borderSE2", tunnel_borderSE2_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderSE3_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderSE3_Mat"));
	mapMaterials.Add("tunnel_borderSE3", tunnel_borderSE3_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderSW1_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderSW1_Mat"));
	mapMaterials.Add("tunnel_borderSW1", tunnel_borderSW1_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderSW2_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderSW2_Mat"));
	mapMaterials.Add("tunnel_borderSW2", tunnel_borderSW2_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderSW3_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderSW3_Mat"));
	mapMaterials.Add("tunnel_borderSW3", tunnel_borderSW3_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_borderW1_Mat(TEXT("/Game/Materials/Tunnels/tunnel_borderW1_Mat"));
	mapMaterials.Add("tunnel_borderW1", tunnel_borderW1_Mat.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> tunnel_Floor1_Mat(TEXT("/Game/Materials/Tunnels/tunnel_Floor1_Mat"));
	mapMaterials.Add("tunnel_Floor1", tunnel_Floor1_Mat.Object);
}

void ADragonAgeJourneysPlayerController::LoadBattleHexMaterials()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> hex_mode_normal(TEXT("/Game/Materials/Battle/Hex/hex_mode_normal"));
	battleHexMaterials.Add(HEX_MODE_NORMAL, hex_mode_normal.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> hex_mode_move(TEXT("/Game/Materials/Battle/Hex/hex_mode_move"));
	battleHexMaterials.Add(HEX_MODE_MOVE, hex_mode_move.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> hex_mode_selected(TEXT("/Game/Materials/Battle/Hex/hex_mode_selected"));
	battleHexMaterials.Add(HEX_MODE_SELECTED, hex_mode_selected.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> hex_mode_target(TEXT("/Game/Materials/Battle/Hex/hex_mode_target"));
	battleHexMaterials.Add(HEX_MODE_TARGET, hex_mode_target.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> hex_mode_moveattack(TEXT("/Game/Materials/Battle/Hex/hex_mode_moveattack"));
	battleHexMaterials.Add(HEX_MODE_MOVEATTACK, hex_mode_moveattack.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> hex_mode_ranged(TEXT("/Game/Materials/Battle/Hex/hex_mode_ranged"));
	battleHexMaterials.Add(HEX_MODE_RANGED, hex_mode_ranged.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> hex_mode_shadowed(TEXT("/Game/Materials/Battle/Hex/hex_mode_shadowed"));
	battleHexMaterials.Add(HEX_MODE_SHADOWED, hex_mode_shadowed.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> hex_mode_showpath(TEXT("/Game/Materials/Battle/Hex/hex_mode_showpath"));
	battleHexMaterials.Add(HEX_MODE_SHOWPATH, hex_mode_showpath.Object);
}

void ADragonAgeJourneysPlayerController::LoadSkillTextures()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> qkb_transparent(TEXT("/Game/Images/QuickBar/qkb_transparent"));
	SkillEmpty = qkb_transparent.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> MOVETexture(TEXT("/Game/Images/Icons/Skills/MOVE")); skillTextures.Add(1, MOVETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> STRIKETexture(TEXT("/Game/Images/Icons/Skills/STRIKE")); skillTextures.Add(2, STRIKETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> WEAPON_SWEEPTexture(TEXT("/Game/Images/Icons/Skills/WEAPON_SWEEP")); skillTextures.Add(3, WEAPON_SWEEPTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SHOCKTexture(TEXT("/Game/Images/Icons/Skills/SHOCK")); skillTextures.Add(4, SHOCKTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> FIREBALLTexture(TEXT("/Game/Images/Icons/Skills/FIREBALL")); skillTextures.Add(5, FIREBALLTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> FLAMING_WEAPONTexture(TEXT("/Game/Images/Icons/Skills/FLAMING_WEAPON")); skillTextures.Add(6, FLAMING_WEAPONTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> BERSERKTexture(TEXT("/Game/Images/Icons/Skills/BERSERK")); skillTextures.Add(7, BERSERKTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> FINAL_BLOWTexture(TEXT("/Game/Images/Icons/Skills/FINAL_BLOW")); skillTextures.Add(8, FINAL_BLOWTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> OVERPOWERTexture(TEXT("/Game/Images/Icons/Skills/OVERPOWER")); skillTextures.Add(9, OVERPOWERTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> OVERRUNTexture(TEXT("/Game/Images/Icons/Skills/OVERRUN")); skillTextures.Add(10, OVERRUNTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SUNDER_ARMORTexture(TEXT("/Game/Images/Icons/Skills/SUNDER_ARMOR")); skillTextures.Add(11, SUNDER_ARMORTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> ALPHA_STRIKETexture(TEXT("/Game/Images/Icons/Skills/ALPHA_STRIKE")); skillTextures.Add(12, ALPHA_STRIKETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> BLIZZARDTexture(TEXT("/Game/Images/Icons/Skills/BLIZZARD")); skillTextures.Add(13, BLIZZARDTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> BLOOD_SACRIFICETexture(TEXT("/Game/Images/Icons/Skills/BLOOD_SACRIFICE")); skillTextures.Add(14, BLOOD_SACRIFICETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DEATH_CLOUDTexture(TEXT("/Game/Images/Icons/Skills/DEATH_CLOUD")); skillTextures.Add(15, DEATH_CLOUDTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DRAIN_LIFETexture(TEXT("/Game/Images/Icons/Skills/DRAIN_LIFE")); skillTextures.Add(16, DRAIN_LIFETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> BELOW_THE_BELTTexture(TEXT("/Game/Images/Icons/Skills/BELOW_THE_BELT")); skillTextures.Add(17, BELOW_THE_BELTTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> LETHALITYTexture(TEXT("/Game/Images/Icons/Skills/LETHALITY")); skillTextures.Add(18, LETHALITYTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> COUP_DE_GRACETexture(TEXT("/Game/Images/Icons/Skills/COUP_DE_GRACE")); skillTextures.Add(19, COUP_DE_GRACETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DEADLY_STRIKETexture(TEXT("/Game/Images/Icons/Skills/DEADLY_STRIKE")); skillTextures.Add(21, DEADLY_STRIKETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DIRTY_FIGHTINGTexture(TEXT("/Game/Images/Icons/Skills/DIRTY_FIGHTING")); skillTextures.Add(22, DIRTY_FIGHTINGTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> FEIGN_DEATHTexture(TEXT("/Game/Images/Icons/Skills/FEIGN_DEATH")); skillTextures.Add(23, FEIGN_DEATHTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DUAL_DOUBLE_STRIKETexture(TEXT("/Game/Images/Icons/Skills/DUAL_DOUBLE_STRIKE")); skillTextures.Add(24, DUAL_DOUBLE_STRIKETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DUAL_CRIPPLETexture(TEXT("/Game/Images/Icons/Skills/DUAL_CRIPPLE")); skillTextures.Add(25, DUAL_CRIPPLETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DUAL_RIPOSTETexture(TEXT("/Game/Images/Icons/Skills/DUAL_RIPOSTE")); skillTextures.Add(26, DUAL_RIPOSTETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> PUNISHERTexture(TEXT("/Game/Images/Icons/Skills/PUNISHER")); skillTextures.Add(27, PUNISHERTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> AIMTexture(TEXT("/Game/Images/Icons/Skills/AIM")); skillTextures.Add(28, AIMTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> INDOMITABLETexture(TEXT("/Game/Images/Icons/Skills/INDOMITABLE")); skillTextures.Add(29, INDOMITABLETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> STUNNING_BLOWSTexture(TEXT("/Game/Images/Icons/Skills/STUNNING_BLOWS")); skillTextures.Add(30, STUNNING_BLOWSTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CRITICAL_STRIKETexture(TEXT("/Game/Images/Icons/Skills/CRITICAL_STRIKE")); skillTextures.Add(31, CRITICAL_STRIKETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SUNDER_ARMSTexture(TEXT("/Game/Images/Icons/Skills/SUNDER_ARMS")); skillTextures.Add(32, SUNDER_ARMSTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SHATTERING_BLOWSTexture(TEXT("/Game/Images/Icons/Skills/SHATTERING_BLOWS")); skillTextures.Add(33, SHATTERING_BLOWSTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DESTROYERTexture(TEXT("/Game/Images/Icons/Skills/DESTROYER")); skillTextures.Add(34, DESTROYERTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> MIGHTY_BLOWTexture(TEXT("/Game/Images/Icons/Skills/MIGHTY_BLOW")); skillTextures.Add(35, MIGHTY_BLOWTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> STEALTH2Texture(TEXT("/Game/Images/Icons/Skills/STEALTH2")); skillTextures.Add(38, STEALTH2Texture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> STEALTH3Texture(TEXT("/Game/Images/Icons/Skills/STEALTH3")); skillTextures.Add(39, STEALTH3Texture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> STEALTH4Texture(TEXT("/Game/Images/Icons/Skills/STEALTH4")); skillTextures.Add(40, STEALTH4Texture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> STEALTH1Texture(TEXT("/Game/Images/Icons/Skills/STEALTH1")); skillTextures.Add(41, STEALTH1Texture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> TAUNTTexture(TEXT("/Game/Images/Icons/Skills/TAUNT")); skillTextures.Add(42, TAUNTTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> THREATENTexture(TEXT("/Game/Images/Icons/Skills/THREATEN")); skillTextures.Add(43, THREATENTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> POWERFULTexture(TEXT("/Game/Images/Icons/Skills/POWERFUL")); skillTextures.Add(44, POWERFULTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> BRAVERYTexture(TEXT("/Game/Images/Icons/Skills/BRAVERY")); skillTextures.Add(45, BRAVERYTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DEATH_BLOWTexture(TEXT("/Game/Images/Icons/Skills/DEATH_BLOW")); skillTextures.Add(46, DEATH_BLOWTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DISENGAGETexture(TEXT("/Game/Images/Icons/Skills/DISENGAGE")); skillTextures.Add(47, DISENGAGETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> PRECISE_STRIKINGTexture(TEXT("/Game/Images/Icons/Skills/PRECISE_STRIKING")); skillTextures.Add(48, PRECISE_STRIKINGTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> PERFECT_STRIKETexture(TEXT("/Game/Images/Icons/Skills/PERFECT_STRIKE")); skillTextures.Add(49, PERFECT_STRIKETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DUAL_WEAPON_SWEEPTexture(TEXT("/Game/Images/Icons/Skills/DUAL_WEAPON_SWEEP")); skillTextures.Add(50, DUAL_WEAPON_SWEEPTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> HEALTexture(TEXT("/Game/Images/Icons/Skills/HEAL")); skillTextures.Add(51, HEALTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> GROUP_HEALTexture(TEXT("/Game/Images/Icons/Skills/GROUP_HEAL")); skillTextures.Add(52, GROUP_HEALTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> WAR_CRYTexture(TEXT("/Game/Images/Icons/Skills/WAR_CRY")); skillTextures.Add(53, WAR_CRYTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> FLAME_BLASTTexture(TEXT("/Game/Images/Icons/Skills/FLAME_BLAST")); skillTextures.Add(54, FLAME_BLASTTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> MELEE_ARCHERTexture(TEXT("/Game/Images/Icons/Skills/MELEE_ARCHER")); skillTextures.Add(55, MELEE_ARCHERTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DEFENSIVE_FIRETexture(TEXT("/Game/Images/Icons/Skills/DEFENSIVE_FIRE")); skillTextures.Add(56, DEFENSIVE_FIRETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> PINNING_SHOTTexture(TEXT("/Game/Images/Icons/Skills/PINNING_SHOT")); skillTextures.Add(57, PINNING_SHOTTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CRIPPLING_SHOTTexture(TEXT("/Game/Images/Icons/Skills/CRIPPLING_SHOT")); skillTextures.Add(58, CRIPPLING_SHOTTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SUPPRESSING_FIRETexture(TEXT("/Game/Images/Icons/Skills/SUPPRESSING_FIRE")); skillTextures.Add(59, SUPPRESSING_FIRETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CRITICAL_SHOTTexture(TEXT("/Game/Images/Icons/Skills/CRITICAL_SHOT")); skillTextures.Add(60, CRITICAL_SHOTTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SONG_OF_COURAGETexture(TEXT("/Game/Images/Icons/Skills/SONG_OF_COURAGE")); skillTextures.Add(62, SONG_OF_COURAGETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DISTRACTIONTexture(TEXT("/Game/Images/Icons/Skills/DISTRACTION")); skillTextures.Add(63, DISTRACTIONTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CAPTIVATETexture(TEXT("/Game/Images/Icons/Skills/CAPTIVATE")); skillTextures.Add(64, CAPTIVATETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SONG_OF_VALORTexture(TEXT("/Game/Images/Icons/Skills/SONG_OF_VALOR")); skillTextures.Add(65, SONG_OF_VALORTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> RESILIENCETexture(TEXT("/Game/Images/Icons/Skills/RESILIENCE")); skillTextures.Add(66, RESILIENCETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CONSTRAINTTexture(TEXT("/Game/Images/Icons/Skills/CONSTRAINT")); skillTextures.Add(67, CONSTRAINTTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> BLOOD_MAGICTexture(TEXT("/Game/Images/Icons/Skills/BLOOD_MAGIC")); skillTextures.Add(68, BLOOD_MAGICTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> BLOOD_WOUNDTexture(TEXT("/Game/Images/Icons/Skills/BLOOD_WOUND")); skillTextures.Add(69, BLOOD_WOUNDTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> BLOOD_CONTROLTexture(TEXT("/Game/Images/Icons/Skills/BLOOD_CONTROL")); skillTextures.Add(70, BLOOD_CONTROLTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> MARK_OF_DEATHTexture(TEXT("/Game/Images/Icons/Skills/MARK_OF_DEATH")); skillTextures.Add(71, MARK_OF_DEATHTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> EXPLOIT_WEAKNESSTexture(TEXT("/Game/Images/Icons/Skills/EXPLOIT_WEAKNESS")); skillTextures.Add(72, EXPLOIT_WEAKNESSTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> LACERATETexture(TEXT("/Game/Images/Icons/Skills/LACERATE")); skillTextures.Add(73, LACERATETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> FEAST_OF_THE_FALLENTexture(TEXT("/Game/Images/Icons/Skills/FEAST_OF_THE_FALLEN")); skillTextures.Add(74, FEAST_OF_THE_FALLENTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DUAL_WEAPON_FLURRYTexture(TEXT("/Game/Images/Icons/Skills/DUAL_WEAPON_FLURRY")); skillTextures.Add(75, DUAL_WEAPON_FLURRYTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DUAL_WEAPON_MOMENTUMTexture(TEXT("/Game/Images/Icons/Skills/DUAL_WEAPON_MOMENTUM")); skillTextures.Add(76, DUAL_WEAPON_MOMENTUMTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> WHIRLWINDTexture(TEXT("/Game/Images/Icons/Skills/WHIRLWIND")); skillTextures.Add(77, WHIRLWINDTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> MASTER_ARCHERTexture(TEXT("/Game/Images/Icons/Skills/MASTER_ARCHER")); skillTextures.Add(78, MASTER_ARCHERTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> COMBAT_MOVEMENTTexture(TEXT("/Game/Images/Icons/Skills/COMBAT_MOVEMENT")); skillTextures.Add(79, COMBAT_MOVEMENTTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> EVASIONTexture(TEXT("/Game/Images/Icons/Skills/EVASION")); skillTextures.Add(80, EVASIONTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SPELL_BOOST1Texture(TEXT("/Game/Images/Icons/Skills/SPELL_BOOST1")); skillTextures.Add(81, SPELL_BOOST1Texture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SPELL_BOOST2Texture(TEXT("/Game/Images/Icons/Skills/SPELL_BOOST2")); skillTextures.Add(82, SPELL_BOOST2Texture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SPELL_BOOST3Texture(TEXT("/Game/Images/Icons/Skills/SPELL_BOOST3")); skillTextures.Add(83, SPELL_BOOST3Texture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SPELL_BOOST4Texture(TEXT("/Game/Images/Icons/Skills/SPELL_BOOST4")); skillTextures.Add(84, SPELL_BOOST4Texture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SHIELD_DEFENSETexture(TEXT("/Game/Images/Icons/Skills/SHIELD_DEFENSE")); skillTextures.Add(85, SHIELD_DEFENSETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SHIELD_BALANCETexture(TEXT("/Game/Images/Icons/Skills/SHIELD_BALANCE")); skillTextures.Add(86, SHIELD_BALANCETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SHIELD_WALLTexture(TEXT("/Game/Images/Icons/Skills/SHIELD_WALL")); skillTextures.Add(87, SHIELD_WALLTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SHIELD_EXPERTISETexture(TEXT("/Game/Images/Icons/Skills/SHIELD_EXPERTISE")); skillTextures.Add(88, SHIELD_EXPERTISETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SHIELD_BLOCKTexture(TEXT("/Game/Images/Icons/Skills/SHIELD_BLOCK")); skillTextures.Add(89, SHIELD_BLOCKTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SHIELD_COVERTexture(TEXT("/Game/Images/Icons/Skills/SHIELD_COVER")); skillTextures.Add(90, SHIELD_COVERTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SHIELD_TACTICSTexture(TEXT("/Game/Images/Icons/Skills/SHIELD_TACTICS")); skillTextures.Add(91, SHIELD_TACTICSTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SHIELD_MASTERYTexture(TEXT("/Game/Images/Icons/Skills/SHIELD_MASTERY")); skillTextures.Add(92, SHIELD_MASTERYTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> LIGHTNINGTexture(TEXT("/Game/Images/Icons/Skills/LIGHTNING")); skillTextures.Add(93, LIGHTNINGTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> TEMPESTTexture(TEXT("/Game/Images/Icons/Skills/TEMPEST")); skillTextures.Add(94, TEMPESTTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CHAIN_LIGHTNINGTexture(TEXT("/Game/Images/Icons/Skills/CHAIN_LIGHTNING")); skillTextures.Add(95, CHAIN_LIGHTNINGTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> INFERNOTexture(TEXT("/Game/Images/Icons/Skills/INFERNO")); skillTextures.Add(96, INFERNOTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CURETexture(TEXT("/Game/Images/Icons/Skills/CURE")); skillTextures.Add(97, CURETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> REGENERATIONTexture(TEXT("/Game/Images/Icons/Skills/REGENERATION")); skillTextures.Add(98, REGENERATIONTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> MASS_REJUVENATIONTexture(TEXT("/Game/Images/Icons/Skills/MASS_REJUVENATION")); skillTextures.Add(99, MASS_REJUVENATIONTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> RACIAL_DWARF_MTexture(TEXT("/Game/Images/Icons/Skills/RACIAL_DWARF_M")); skillTextures.Add(100, RACIAL_DWARF_MTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> RACIAL_DWARF_FTexture(TEXT("/Game/Images/Icons/Skills/RACIAL_DWARF_F")); skillTextures.Add(101, RACIAL_DWARF_FTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> RACIAL_HUMAN_MTexture(TEXT("/Game/Images/Icons/Skills/RACIAL_HUMAN_M")); skillTextures.Add(102, RACIAL_HUMAN_MTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> RACIAL_HUMAN_FTexture(TEXT("/Game/Images/Icons/Skills/RACIAL_HUMAN_F")); skillTextures.Add(103, RACIAL_HUMAN_FTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> RACIAL_ELF_MTexture(TEXT("/Game/Images/Icons/Skills/RACIAL_ELF_M")); skillTextures.Add(104, RACIAL_ELF_MTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> RACIAL_ELF_FTexture(TEXT("/Game/Images/Icons/Skills/RACIAL_ELF_F")); skillTextures.Add(105, RACIAL_ELF_FTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> DEATH_MAGICTexture(TEXT("/Game/Images/Icons/Skills/DEATH_MAGIC")); skillTextures.Add(106, DEATH_MAGICTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> MIND_BLASTTexture(TEXT("/Game/Images/Icons/Skills/MIND_BLAST")); skillTextures.Add(108, MIND_BLASTTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> TELEKENETIC_WEAPONSTexture(TEXT("/Game/Images/Icons/Skills/TELEKENETIC_WEAPONS")); skillTextures.Add(110, TELEKENETIC_WEAPONSTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> FORCE_FIELDTexture(TEXT("/Game/Images/Icons/Skills/FORCE_FIELD")); skillTextures.Add(111, FORCE_FIELDTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> RALLYTexture(TEXT("/Game/Images/Icons/Skills/RALLY")); skillTextures.Add(112, RALLYTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> MOTIVATETexture(TEXT("/Game/Images/Icons/Skills/MOTIVATE")); skillTextures.Add(113, MOTIVATETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> SUPERIORITYTexture(TEXT("/Game/Images/Icons/Skills/SUPERIORITY")); skillTextures.Add(114, SUPERIORITYTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> POWERFUL_SWINGSTexture(TEXT("/Game/Images/Icons/Skills/POWERFUL_SWINGS")); skillTextures.Add(115, POWERFUL_SWINGSTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> ORIGIN_HUMAN_NOBLETexture(TEXT("/Game/Images/Icons/Skills/ORIGIN_HUMAN_NOBLE")); skillTextures.Add(116, ORIGIN_HUMAN_NOBLETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> ORIGIN_MAGETexture(TEXT("/Game/Images/Icons/Skills/ORIGIN_MAGE")); skillTextures.Add(117, ORIGIN_MAGETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> ORIGIN_ELF_DALISHTexture(TEXT("/Game/Images/Icons/Skills/ORIGIN_ELF_DALISH")); skillTextures.Add(118, ORIGIN_ELF_DALISHTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> ORIGIN_ELF_CITYTexture(TEXT("/Game/Images/Icons/Skills/ORIGIN_ELF_CITY")); skillTextures.Add(119, ORIGIN_ELF_CITYTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> ORIGIN_DWARF_NOBLETexture(TEXT("/Game/Images/Icons/Skills/ORIGIN_DWARF_NOBLE")); skillTextures.Add(120, ORIGIN_DWARF_NOBLETexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> ORIGIN_DWARF_COMMONERTexture(TEXT("/Game/Images/Icons/Skills/ORIGIN_DWARF_COMMONER")); skillTextures.Add(121, ORIGIN_DWARF_COMMONERTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CRUSHING_PRISONTexture(TEXT("/Game/Images/Icons/Skills/CRUSHING_PRISON")); skillTextures.Add(125, CRUSHING_PRISONTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CURSE_OF_MORTALITYTexture(TEXT("/Game/Images/Icons/Skills/CURSE_OF_MORTALITY")); skillTextures.Add(126, CURSE_OF_MORTALITYTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> LIFEWARDTexture(TEXT("/Game/Images/Icons/Skills/LIFEWARD")); skillTextures.Add(127, LIFEWARDTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> CLEANSING_AURATexture(TEXT("/Game/Images/Icons/Skills/CLEANSING_AURA")); skillTextures.Add(128, CLEANSING_AURATexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> REVIVALTexture(TEXT("/Game/Images/Icons/Skills/REVIVAL")); skillTextures.Add(129, REVIVALTexture.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> GREY_WARDENTexture(TEXT("/Game/Images/Icons/Skills/GREY_WARDEN")); skillTextures.Add(130, GREY_WARDENTexture.Object);

}

AMapHex* ADragonAgeJourneysPlayerController::GetMapHex()
{
	FHitResult TraceResult(ForceInit);
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, false, TraceResult);
	FString TraceString;
	if (TraceResult.GetActor() != nullptr &&
		TraceResult.GetActor()->IsA(AMapHex::StaticClass()))
	{
		return Cast<AMapHex>(TraceResult.GetActor());
	}

	if (TraceResult.GetActor() != nullptr &&
		TraceResult.GetActor()->IsA(ADragonAgeJourneysCharacter::StaticClass()))
	{
		return (Cast<ADragonAgeJourneysCharacter>(TraceResult.GetActor()))->GetCharRef()->group->mapHex;
	}

	return nullptr;
}

AHex* ADragonAgeJourneysPlayerController::GetBattleHex()
{
	FHitResult TraceResult(ForceInit);
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, false, TraceResult);

	BattleTraceImpact = TraceResult.ImpactPoint;

	/*if (TraceResult.GetActor() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Location %s"), *TraceResult.Location.ToCompactString());
		UE_LOG(LogTemp, Warning, TEXT("Impact %s"), *TraceResult.ImpactPoint.ToCompactString());
		UE_LOG(LogTemp, Warning, TEXT("actor location %s"), *TraceResult.GetActor()->GetActorLocation().ToCompactString());
	}*/

	if (TraceResult.GetActor() != nullptr &&
		TraceResult.GetActor()->IsA(AHex::StaticClass()))
	{
		return Cast<AHex>(TraceResult.GetActor());
	}

	if (TraceResult.GetActor() != nullptr &&
		TraceResult.GetActor()->IsA(ADragonAgeJourneysCharacter::StaticClass()))
	{
		return (Cast<ADragonAgeJourneysCharacter>(TraceResult.GetActor()))->hex;
	}

	return nullptr;
}

void ADragonAgeJourneysPlayerController::HandleFloatyTick()
{
	for (int32 i = 0; i < floaties.Num(); i++)
	{
		UUserWidgetFloatyText* _floaty = floaties[i];
		if (_floaty->avatar)
		{
			//remove
			if (GetWorld()->GetTimeSeconds() > _floaty->fTimestamp + _floaty->fDuration)
			{
				floaties.Remove(_floaty);
				_floaty->RemoveFromViewport();
			}
			else //reposition
			{
				//prepare 2D position
				FVector2D v2d = FVector2D(0, 0);
				FVector2D& ScreenLocation = v2d;
				//project 3D to 2D
				float xx = _floaty->vPosition.X + 0.5f;
				float yy = _floaty->vPosition.Y;
				float zz = _floaty->vPosition.Z + 0.5f;
				_floaty->vPosition = FVector(xx, yy, zz);

				float Scale = UWidgetLayoutLibrary::GetViewportScale(_floaty);

				ProjectWorldLocationToScreen(_floaty->vPosition, ScreenLocation);

				if (_floaty->PosX == -1) _floaty->PosX = ScreenLocation.X;

				UCanvasPanelSlot* _canvas = UWidgetLayoutLibrary::SlotAsCanvasSlot(_floaty->Floaty);
				FVector2D _loc = FVector2D(_floaty->PosX / Scale, ScreenLocation.Y / Scale);
				_canvas->SetPosition(_loc);
				//UE_LOG(LogTemp, Warning, TEXT("floaty %s"), *_loc.ToString())
			}
		}
	}
}
