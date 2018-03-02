#include "Battlefield.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Algo/Reverse.h"
#include <EngineUtils.h>
#include "Engine/EngineTypes.h"
#include <ConstructorHelpers.h>
#include "DragonAgeJourneys.h"
#include "Group.h"
#include "GameCharacter.h"
#include "Hex.h"
#include "HexDirection.h"
#include "MapHex.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "Map.h"
#include "Dijkstra.h"
#include "InitiativeStack.h"
#include "Action.h"
#include "Skills.h"
#include "Skill.h"
#include "DragonAgeJourneysCharacter.h"
#include "Effect.h"
#include "Classes.h"
#include "Animation.h"
#include "Intelligence.h"
#include "AiAction.h"
#include "DialogueBark.h"
#include "Item.h"
#include "Consumable.h"
#include "Party.h"
#include "Race.h"
#include "UserWidgetQuickBar.h"
#include "DragonAgeJourneysPlayerController.h"
#include "Trap.h"
#include "DragonAgeJourneysAnimInstance.h"
#include "Slot.h"

UBattlefield::UBattlefield(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UClass> BPHexDirection(TEXT("Class'/Game/Data/Blueprints/BPHexDirection.BPHexDirection_C'"));
	if (BPHexDirection.Object) {
		refHexDirection = BPHexDirection.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass> BPTrap(TEXT("Class'/Game/Data/Blueprints/BPTrap.BPTrap_C'"));
	if (BPTrap.Object) {
		refTrap = BPTrap.Object;
	}
}

void UBattlefield::InitializeBattlefield(TArray<UGroup*> battleGroups, int32 nTileset)
{
	//DHK
	GetMain()->LevelMap->MapZ += 15;

	GetCurrentPlayerController()->SetCurrentGameMode(EUI::COMBAT);

	FActorSpawnParameters SpawnParams;
	directionArrow = GetStage()->SpawnActor<AHexDirection>(refHexDirection, SpawnParams);
	directionArrow->SetActorHiddenInGame(true);

	auto loc6 = false;
	auto loc18 = false;
	auto loc19 = false;
	//	tutorialLayer = new flash.display.Sprite();
	//	debugLayer = new flash.display.Sprite();

	groups = battleGroups;

	//DHK
	for (int32 i = 0; i < groups.Num(); i++)
	{
		UGroup* _group = groups[i];
		for (int32 j = 0; j < _group->chars.Num(); j++)
		{
			characters.Add(_group->chars[j]);
		}
	}

	//	actionText.mouseEnabled = false;
	//	bloodSpots = new Array();
	//	bars_component = new packages.gui.Bars(bars);
	/*endTurnContainer = new EndTurnButton(endTurnButtonContainer);
		endTurnButton = endTurnButtonContainer.hitButton;
		endTurnButtonContainer.parent.removeChild(endTurnButtonContainer);
		stageGUI.addChild(endTurnButtonContainer);
		endTurnButton.addEventListener(flash.events.MouseEvent.CLICK, endTurnClick);
		endTurnButton.addEventListener(flash.events.MouseEvent.MOUSE_OVER, endTurnMouseOver);
		endTurnButton.addEventListener(flash.events.MouseEvent.MOUSE_MOVE, endTurnMouseOver);
		endTurnButton.addEventListener(flash.events.MouseEvent.MOUSE_OUT, endTurnMouseOut);*/
		//	removeChild(queueText);
			//	fx = new packages.fx.ScreenFX();
			//	addChild(fx);
			//	avatarLayer = new flash.display.Sprite();
			//	avatarLayer.mouseEnabled = false;
			//	avatarLayer.mouseChildren = false;
			//	clearActionText();
			//	bars_component.setChar(nullptr);

		//	use CursorToWorld decal
		/*aei = new AreaEffectIndicator();
				aei.cacheAsBitmap = true;
				aei.mouseEnabled = false;
				aei.mouseChildren = false;
				aei.mouseEnabled = false;
				aei.mouseChildren = false;*/

				//	template = new SkillTemplate();
						//	template.cacheAsBitmap = true;
						//	UE_LOG(LogTemp, Warning, TEXT("Battlefield constructor");
						//	navbar = new packages.gui.NavBar(stageGUI.navbar);
						//	auto loc1 = new packages.audio.Music("battle", true, true);
						//	bg.gotoAndStop(nTileset);
						//	bg.cacheAsBitmap = true;
						//	removeBars();
						//	dm.stage.addEventListener(flash.events.KeyboardEvent.KEY_DOWN, onKeyPress);
						//	hexLayer = new flash.display.Sprite();
						//	hexLayer.mouseEnabled = false;
						//	addChild(hexLayer);
	grid.SetNum(NUM_HEXES_X);
	hexes.SetNum(NUM_HEXES_X * NUM_HEXES_Y);
	//	moveRoute = new Array();

	int32 nX = 0;
	int32 nIndex = 0;
	while (nX < NUM_HEXES_X)
	{
		grid[nX].arrHex.SetNum(NUM_HEXES_Y);
		int32 nY = 0;
		while (nY < NUM_HEXES_Y)
		{
			float tempX = 0.f;
			float tempY = 0.f;
			FActorSpawnParameters SpawnParams;
			AHex* loc4 = GetStage()->SpawnActor<AHex>(GetMain()->LevelMap->refBattleHex, SpawnParams);
			loc4->SetActorRelativeRotation(FRotator(0, 90, 0));
			loc4->SetActorRelativeScale3D(FVector(0.85 /*not 1?*/, 1.25, 0.025));
			//	loc4 = new Hex();
			if (nY % 2 != 1)
			{
				/*loc4->x*/ tempX = HEX_X_MIN_EVEN + nX * HEX_WIDTH;
			}
			else
			{
				/*loc4->x*/ tempX = HEX_X_MIN_ODD + nX * HEX_WIDTH;
			}
			/*loc4->y*/ tempY = HEX_Y_MIN + nY * (HEX_TIP_HEIGHT + HEX_MID_HEIGHT);
			loc4->SetActorLocation(FVector(tempX, tempY, 0));
			loc4->gridX = nX;
			loc4->gridY = nY;
			loc4->nHeight = HEX_HEIGHT;
			loc4->nWidth = HEX_WIDTH;
			loc4->index = nIndex /*hexes.Num()*/;
			grid[nX].arrHex[nY] = loc4;
			hexes[nIndex] = loc4;
			//	hexLayer.addChild(loc4);
			++nY;
			++nIndex;
		}
		++nX;
	}

	GenerateHexNeighbours();
	//OrderHexes(); //related to hexLayer visual

	if (nTileset == 3)
	{
		allowObstacles = false;
	}
	if (GetEnemyGroup()->id == "[q4m2_Ogre_Battle]" || GetEnemyGroup()->id == "[q4m3_Ogre_Battle]" || GetEnemyGroup()->id == "[q4m4_Ogre_Battle]" || GetEnemyGroup()->id == "vs_emissary_mine" || GetEnemyGroup()->id == "vs_tut_genlock" || GetEnemyGroup()->id == "vs_tut_emissary")
	{
		allowObstacles = false;
	}

	//	TODO obstacles
		/*if (allowObstacles)
		{
			loc5 = FMath::RandRange(0, 4);
			loc6 = false;
			obstacles = new Array();
			while (obstacles.Num() < loc5)
			{
				if (!loc6 && FMath::RandRange(1, 100) < 30)
				{
					if (nTileset != 1)
					{
						if (nTileset == 4)
						{
							loc8 = new TunnelRockLarge();
						}
					}
					else
					{
						loc8 = new CaveSpikeLarge();
					}
					loc11 = "large";
					loc6 = true;
				}
				else if (FMath::RandRange(1, 100) < 70)
				{
					if (nTileset != 1)
					{
						if (nTileset == 4)
						{
							loc8 = new TunnelRockSmall();
							loc11 = "small";
						}
					}
					else
					{
						loc8 = new CaveSpikeSmall();
						loc11 = "small_tall";
					}
				}
				else if (nTileset != 1)
				{
					if (nTileset == 4)
					{
						loc8 = new TunnelPlantSmall();
						loc11 = "horz2";
					}
				}
				else
				{
					loc8 = new CaveSpikeWide();
					loc11 = "horz2_tall";
				}
				if (!loc8)
				{
					break;
				}
				loc9 = getObstaclePosition(true);
				loc13 = 0;
				while (!obstacleCanFitHere(loc9, loc11) && loc13 < 100)
				{
					loc9 = getObstaclePosition(true);
					++loc13;
				}
				if (loc13 >= 100)
				{
					--loc5;
					continue;
				}
				loc10 = grid[loc9->x].arrInt32[loc9->y] as Hex;
				if (loc11 != "large")
				{
					if (loc11 != "small")
					{
						if (loc11 != "small_tall")
						{
							if (loc11 != "horz2")
							{
								if (loc11 == "horz2_tall")
								{
									loc10->obstacle = HEX_OBSTACLE_LARGE;
									loc10->neighbours[4].obstacle = HEX_OBSTACLE_LARGE;
									loc10->visualObstacle = loc8;
									if (loc10->neighbours[4])
									{
										loc10->neighbours[4].visualObstacle = loc8;
									}
									if (loc10->neighbours[5])
									{
										loc10->neighbours[5].visualObstacle = loc8;
									}
								}
							}
							else
							{
								loc10->obstacle = HEX_OBSTACLE_SMALL;
								loc10->neighbours[4].obstacle = HEX_OBSTACLE_SMALL;
							}
						}
						else
						{
							loc10->obstacle = HEX_OBSTACLE_LARGE;
							loc10->visualObstacle = loc8;
						}
					}
					else
					{
						loc10->obstacle = HEX_OBSTACLE_SMALL;
						loc10->visualObstacle = loc8;
					}
				}
				else
				{
					loc10->obstacle = HEX_OBSTACLE_LARGE;
					auto loc20 = 0;
					auto loc21 = loc10->neighbours;
					for(loc14 in loc21)
					{
						if (loc14 == nullptr)
						{
							continue;
						}
						loc14.obstacle = HEX_OBSTACLE_LARGE;
					}
					loc15 = new Array();
					loc15->Add(loc10);
					loc20 = 0;
					loc21 = loc10->neighbours;
					for(loc14 in loc21)
					{
						if (loc14 == nullptr)
						{
							continue;
						}
						loc15->Add(loc14);
					}
					if (loc10->neighbours[5])
					{
						if (loc10->neighbours[5].neighbours[5])
						{
							loc15->Add(loc10->neighbours[5].neighbours[5]);
						}
						if (loc10->neighbours[5].neighbours[0])
						{
							loc15->Add(loc10->neighbours[5].neighbours[0]);
							if (loc8 is CaveSpikeLarge && loc10->neighbours[5].neighbours[0].neighbours[5])
							{
								loc15->Add(loc10->neighbours[5].neighbours[0].neighbours[5]);
							}
						}
					}
					loc20 = 0;
					loc21 = loc15;
					for(loc16 in loc21)
					{
						loc16.visualObstacle = loc8;
					}
				}
				if (loc8 == nullptr)
				{
					continue;
				}
				obstacles.Add(loc8);
				loc8->mouseEnabled = false;
				loc8->mouseChildren = false;
				if (loc10)
				{
					loc8->x = loc10->x;
					loc8->y = loc10->y;
				}
				avatarLayer.addChild(loc8);
				Depth.check(loc8);
			}
			loc20 = 0;
			loc21 = obstacles;
			for(loc7 in loc21)
			{
				if (loc7->getChildByName("footprint_large"))
				{
					loc17 = loc7->getChildByName("footprint_large") as flash.display.Sprite;
					loc18 = true;
				}
				else if (loc7->getChildByName("footprint_small"))
				{
					loc17 = loc7->getChildByName("footprint_small") as flash.display.Sprite;
					loc18 = false;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("no footprint found for obstacle! " + loc7);
					continue;
				}
				loc19 = false;
				loc17->visible = false;
			}
		}*/

	PrecomputeDistances();
	UE_LOG(LogTemp, Warning, TEXT("board setup finished"));
	initStack = NewObject<UInitiativeStack>();
	GenerateAvatars();
	SetupCharacters();//TODO useless? or for initStack


	//	vfx = new Array();
	//	projectiles = new Array();
	//	traps = new Array();

		//TODO BattleInitiativeDisplay
		/*initStack.battleStart();
		initStack.build();
		initDisplay = new BattleInitiativeDisplay(initStack);
		stageGUI.addChild(initDisplay);*/

		//	dmf.childToTop(stageGUI);
		// 	frameCount = 0;
		// 	mouseEnabled = false;
		// 	mouseChildren = false;
		// 	tabEnabled = false;
		// 	tabChildren = false;

		//TODO ActionIndicator
			/*actionIndicator = new ActionIndicator();
			addChild(actionIndicator);
			removeActionIndicator();*/

			// 	turnHint.visible = false;
			// 	frameCount = 0;

				//handled in Level Tick
			//	addEventListener(flash.events.Event.ENTER_FRAME, onFrame, false, 0, true);

				// 	hexLayer.visible = false;
			// 	initDisplay.visible = false;
			// 	addChild(avatarLayer);
			// 	addChild(stageGUI);

				/*if (packages.gui.LoadingScreen.current)
				{
					packages.gui.LoadingScreen.current.remove();
				}*/

				/*if (GetEnemyGroup()->id == "vs_tut_emissary")
				{
					suspendTurnOrder = true;
					addSign("Meanwhile..", true);
				}*/

				//TODO add widget sign
				/*if (sign == nullptr)
				{
					addSign("Prepare for battle!", true);
				}*/
}

void UBattlefield::BattlefieldTick()
{
	//TODO update code for 'frame' related logic
	//TODO check code for 'Sound' related logic

	AHex* destinationHex = nullptr;

	_highlightedSkillTargetsThisFrame = false;
	if (_skillTargetsHighlightPendingFor != nullptr)
	{
		HighlightSkillTargets(_skillTargetsHighlightPendingFor);
		_skillTargetsHighlightPendingFor = nullptr;
	}

	/*if (_generatingCharacters)
	{
		if (_generatedCharacterCount >= characters.Num())
		{
			_generatingCharacters = false;
			//BattleLoadComplete();
			UE_LOG(LogTemp, Warning, TEXT("Battlefield _generatingCharacters done %i"), characters.Num());
		}
		else
		{
			GenerateNextCharacter();
			return;
		}
	}*/

	if (currentAction != nullptr)
	{
		waitingForActionFrame++;
	}

	if (waitFrames > 0)
	{
		waitFrames--;
	}
	else if (currentAction != nullptr &&
		(currentAction->actionType == ACTION_BARK_WAIT || currentAction->actionType == ACTION_WAIT))
	{
		currentAction->complete = true;
	}

	//TODO projectiles
	/*auto loc4 = projectiles.Num() - 1;
	while (loc4 >= 0)
	{
		loc8 = projectiles[loc4];
		if (loc8)
		{
			loc8->run();
			if (!loc8->active)
			{
				if (loc8->clip.parent)
				{
					loc8->clip.parent.removeChild(loc8->clip);
				}
				projectiles.splice(loc4, 1);
			}
		}
		else
		{
			projectiles.splice(loc4, 1);
		}
		--loc4;
	}*/
	// 	packages.audio.LowHealthSounds.current.update(dm.party);

		//TODO VFX
		/*auto loc5 = vfx.Num() - 1;
		while (loc5 >= 0)
		{
			if (vfx[loc5].currentFrame == vfx[loc5].totalFrames)
			{
				if (vfx[loc5].parent)
				{
					vfx[loc5].parent.removeChild(vfx[loc5]);
				}
				vfx.splice(loc5, 1);
			}
			--loc5;
		}*/

	if (moveRoute.Num() > 0)
	{
		destinationHex = moveRoute[moveRoute.Num() - 1];

		SetAvatarRotation(currentAvatar, destinationHex);
		SetAvatarSpeed(currentAvatar, SpeedWalk);

		currentAvatar->MoveTowards(destinationHex->GetActorLocation().X, destinationHex->GetActorLocation().Y);
		if (currentAvatar->GetActorLocation().X == destinationHex->GetActorLocation().X &&
			currentAvatar->GetActorLocation().Y == destinationHex->GetActorLocation().Y)
		{
			AHex* loc9 = moveRoute.Pop();

			for (int32 i = 0; i < traps.Num(); i++)
			{
				ATrap* loc1 = traps[i];

				if (!(loc1 && loc1->hex == loc9 && loc1->active))
				{
					continue;
				}

				UGameCharacter* loc15 = GetCurrentCharacter();
				bool loc16 = TrySpringTrap(loc15, loc1);

				if (!loc16)
				{
					continue;
				}

				loc15->usedSkill = true;
				loc15->moved = loc15->moved + GetCurrentCharacter()->GetMovesLeft();
				loc9->AttachAvatar(currentAvatar);
				EndMove();
				//RemoveBars();
				currentAction->complete = true;

				moveRoute.Empty();
				actionQueue.Empty();
			}

			TArray<ADragonAgeJourneysCharacter*> loc10 = loc9->GetNeighbourAvatars(true);
			bool bHidden = GetCurrentCharacter()->IsHidden();
			int32 loc12 = GetCurrentCharacter()->GetStealthStrength();
			int32 loc13 = 0;
			//loc22 = 0;
			//loc23 = loc10;

			for (int32 i = 0; i < loc10.Num(); i++)
			{
				ADragonAgeJourneysCharacter* loc14 = loc10[i];

				if (loc14 == nullptr)
				{
					continue;
				}

				UGameCharacter* loc17 = loc14->GetCharRef();

				if (loc14 == currentAvatar)
				{
					continue;
				}

				if (loc17->IsInactive())
				{
					continue;
				}

				if (AreAllies(GetCurrentCharacter(), loc17))
				{
					continue;
				}

				if (bHidden)
				{
					loc13 = loc17->GetPerceptionRoll();
					UE_LOG(LogTemp, Warning, TEXT("%s is hidden and moving, does %s detect him? roll (%i/%i)"), *GetCurrentCharacter()->charName, *loc17->charName, loc13, loc12);
					if (loc13 > loc12)
					{
						UE_LOG(LogTemp, Warning, TEXT("detected"));
						RemoveStealthEffects(GetCurrentCharacter());
						bHidden = false;
					}
				}

				if (!loc17->IsHidden())
				{
					continue;
				}

				loc13 = GetCurrentCharacter()->GetPerceptionRoll();
				UE_LOG(LogTemp, Warning, TEXT("%s is hidden and stationary, does the moving %s detect him? roll (%i/%i)"), *loc17->charName, *GetCurrentCharacter()->charName, loc13, loc17->GetStealthStrength());
				if (loc13 > loc17->GetStealthStrength())
				{
					UE_LOG(LogTemp, Warning, TEXT("detected"));
					RemoveStealthEffects(loc17);
					bHidden = false;
				}
			}
			if (moveRoute.Num() == 0 && currentAction && !currentAction->complete)
			{
				UE_LOG(LogTemp, Warning, TEXT("character reached final destination - %s"), *loc9->StringData());
				loc9->AttachAvatar(currentAvatar);
				GetCurrentCharacter()->moved = GetHexDistanceOccupied(hexes[currentAction->moveToHexIndex], dSearch);
				if (currentAction->actionType != ACTION_RETREAT)
				{
					EndMove();
					currentAction->complete = true;
				}
			}
		}
	}
	else if (currentAction != nullptr && currentAction->actionType == ACTION_RETREAT)
	{
		int32 loc18 = -50;
		int32 loc19 = currentAvatar->GetActorLocation().Y;

		currentAvatar->MoveTowards(loc18, loc19);

		if (currentAvatar->GetActorLocation().X == loc18 && currentAvatar->GetActorLocation().Y == loc19)
		{
			currentAction->complete = true;
			/*if (currentAvatar->parent)
			{
				currentAvatar->parent.removeChild(currentAvatar);
			}*/
			currentAvatar->SetActorHiddenInGame(true);
		}
	}

	for (int32 i = 0; i < avatars.Num(); i++)
	{
		if (currentAction)
		{
			ADragonAgeJourneysCharacter* _avatar = avatars[i];
			if (/*_avatar->parent == nullptr &&*/ _avatar->GetCharRef() == currentAction->gameCharacter)
			{
				if (currentAction->actionType == ACTION_MOVE && !IsMoving())
					currentAction->complete = true;
			}

			if (_avatar->currentAnimation == ANIMATION_IDLE)
			{
				if (currentAction != nullptr && _avatar->GetCharRef() == currentAction->gameCharacter && currentAction->actionType == ACTION_SKILL_WAIT_FOR_ANIMATION)
				{
					currentAction->complete = true;
					continue;//DHK
				}

				//continue;
			}

			if (currentAction)
			{
				if (currentAction->targetHexIndex != -1)
					destinationHex = hexes[currentAction->targetHexIndex];
			}

			if (currentAction != nullptr && _avatar->GetCharRef() == currentAction->gameCharacter && currentAction->actionType == ACTION_SKILL_ANIMATION && !currentAction->complete)
			{
				bool bToggledOn = currentAction->useSkill && currentAction->useSkill->type == SKILL_TYPE_TOGGLED && currentAction->useSkill->toggledOn;

				if (currentAction->useSkill && currentAction->useSkill->ignoreActionFrame)
				{
					currentAction->complete = true;
					if (!bToggledOn)
					{
						if (currentAction->useSkill && currentAction->useSkill->actionSoundEvenOnMiss || !currentAction->skillMiss)
						{
							//packages.audio.Sounds.play(currentAction->useSkill->sound_actionFrame);
						}
					}
				}
				else if (_avatar->currentLabel == "action_frame" || waitingForActionFrame > 75)
				{
					currentAction->complete = true;
					if (!bToggledOn)
					{
						if (currentAction->useSkill && currentAction->useSkill->actionSoundEvenOnMiss || !currentAction->skillMiss)
						{
							//packages.audio.Sounds.play(currentAction->useSkill->sound_actionFrame);
						}
					}

					//TODO grab avatar 
					/*if (grabAvatar && !(grabAvatar.parent == avatarLayer))
					{
						grabAvatar.parent.removeChild(grabAvatar);
						grabAvatar.x = grabHex.x;
						grabAvatar.y = grabHex.y;
						grabAvatar.setFacingLeft(grabFacing);
						avatarLayer.addChild(grabAvatar);
						grabAvatar = nullptr;
						grabHex = nullptr;
					}*/
				}
				//TODO grab_frame
				/*else if (_avatar->currentLabel == "grab_frame")
				{
					if (_avatar->ca && _avatar->ca.enemyContainer)
					{
						if (grabHex != hexes[currentAction->targetHexIndex])
						{
							grabHex = hexes[currentAction->targetHexIndex] as Hex;
							grabAvatar = grabHex.avatar;
							grabAvatar.x = 0;
							grabAvatar.y = 95;
							grabFacing = grabAvatar.facingLeft;
							grabAvatar.setFacingLeft(!grabFacing);
							grabAvatar->AnimateAvatar(ANIMATION_INJURY, nullptr, false, 0, 3);
						}
						if (grabAvatar != nullptr)
						{
							if (grabAvatar.parent)
							{
								grabAvatar.parent.removeChild(grabAvatar);
							}
							_avatar->ca.enemyContainer.addChild(grabAvatar);
						}
					}
				}*/
				else if (_avatar->currentLabel == "air_frame")
				{
					destinationHex = hexes[currentAction->intArg];
					if (destinationHex)
					{
						if (_avatar->GetActorLocation().X == destinationHex->GetActorLocation().X)
						{
							//TODO play anim
							//_avatar->anim.play();
							_avatar->hex->DetachAvatar();
							destinationHex->AttachAvatar(_avatar);
							_avatar->SetFacingLeft(destinationHex->GetActorLocation().X > hexes[currentAction->targetHexIndex]->GetActorLocation().X);
						}
						else
						{
							//_avatar->anim.stop();
							_avatar->MoveTowards(destinationHex->GetActorLocation().X, destinationHex->GetActorLocation().Y, 50);
						}
					}
				}
				else if (_avatar->currentLabel == "leap_frame")
				{
					//TODO leap_frame
				}
				else if (_avatar->IsAnimationAtFinalFrame())
				{
					UE_LOG(LogTemp, Error, TEXT("*ERROR* Animation has no \'action_frame\'! : %s"), *_avatar->GetCharRef()->charName /*_avatar->character.currentFrame*/);
					currentAction->complete = true;
				}
			}

			//TODO blood_frame
			/*if (_avatar->currentLabel == "blood_frame" && !_avatar->bloodSpawned && _avatar->showBlood)
			{
				loc21 = BloodSpray.spawn(_avatar, true);
				if (loc21 != nullptr)
				{
					avatarLayer.addChild(loc21);
					Depth.check(loc21, _avatar->y - 1);
					bloodSpots.Add(loc21);
				}
				_avatar->bloodSpawned = true;
			}*/

			/*if (_avatar->anim.currentFrame != _avatar->anim.totalFrames)
			{
				continue;
			}*/

			if (IsLoopAnimation(_avatar->currentAnimation) != false)
			{
				continue;
			}

			if (_avatar->currentAnimation != ANIMATION_DEATH && _avatar->currentAnimation != ANIMATION_INSTANT_DEATH)
			{
				_avatar->AnimateAvatar(ANIMATION_IDLE);
			}
			else /*if (!(_avatar->parent == nullptr) && _avatar->parent == avatarLayer)*/
			{
				//_avatar->anim.stop();
				if (_avatar->hex != nullptr && _avatar->GetCharRef()->health <= 0)
				{
					_avatar->deathHex = _avatar->hex;
					_avatar->hex->DetachAvatar();
				}
			}

			if (currentAction != nullptr && currentAction->actionType == ACTION_SKILL_WAIT_FOR_ANIMATION && _avatar->GetCharRef() == currentAction->gameCharacter)
			{
				currentAction->complete = true;
			}
		}
	}

	//TODO traps
	/*loc7 = traps.Num() - 1;
	while (loc7 >= 0)
	{
		loc1 = traps[loc7] as Trap;
		if (loc1 && !loc1->active)
		{
			if (loc1->clip.parent)
			{
				loc1->clip.parent.removeChild(loc1->clip);
			}
			traps.splice(loc7, 1);
		}
		--loc7;
	}*/

	if (frameCount < 50)
	{
		if (frameCount == 1)
		{
			if (endBattle)
			{
				DoEndBattle();
			}
			else
			{
				_generatingCharacters = true;
				_generatedCharacterCount = 0;
			}
		}
		else if (frameCount == 45)
		{
			if (endBattle)
			{
				if (battleResult == "defeat")
				{
					ExitBattle();
				}
			}
			else if (!suspendTurnOrder)
			{
				/*mouseEnabled = true;
				mouseChildren = true;
				tabEnabled = true;
				tabChildren = true;*/
				NextTurn();
			}
		}

		frameCount++;
	}

	if (frameCount >= 50 && (currentAction == nullptr || currentAction->complete) && !paused)
	{
		ProcessActions();
	}

	/*if (showQueueDebug)
	{
		queueDebugText();
	}*/
}

void UBattlefield::RequestMoveTo(AHex* hex)
{
	UE_LOG(LogTemp, Warning, TEXT("requestMoveTo"));

	if (currentAction != nullptr)
	{
		currentAvatar = currentAction->gameCharacter->avatar;
	}
	if (hex->avatar != nullptr && currentAvatar != hex->avatar)
	{
		return;
	}

	GetCurrentHex()->DetachAvatar();
	currentAvatar->ClearLargeHexes();
	ResetHexes();
	UpdateHexes();
	moveRoute = GetRouteTo(hex);
	Algo::Reverse(moveRoute);
	currentAvatar->movedY = 0;
	currentAvatar->AnimateAvatar(ANIMATION_WALK);
	//currentAvatar->StartWalkSound(0.7, 8);
	//RemoveBars();
}

void UBattlefield::HighlightSkillTargets(USkill* skill /*= nullptr*/)
{
	UE_LOG(LogTemp, Warning, TEXT("HighlightSkillTargets %s using skill %s"), *currentAvatar->GetCharRef()->charName, *skill->skillname);

	if (!IsPlayerTurn())
	{
		return;
	}

	//TODO HighlightSkillTargets
	/*if (skill == nullptr)
	{
		skill = skillbar.selected;
	}*/

	if (skill == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("!!WARNING!! highlightSkillTargets fails: skillbar.selected is nullptr"));
		return;
	}

	if (_highlightedSkillTargetsThisFrame)
	{
		_skillTargetsHighlightPendingFor = skill;
		return;
	}

	_highlightedSkillTargetsThisFrame = true;

	if (dSearch == nullptr)
	{
		movementMap = BuildAdjacencyMap();
		dSearch = NewObject<UDijkstra>();
		dSearch->InitializeDijkstra(movementMap, GetCurrentHex()->index);
		dSearch->FindShortestPath();
	}

	if (skill->skillId == SKILL_CONSUMABLE)
	{
		UE_LOG(LogTemp, Warning, TEXT("highlight for consumable"));
	}

	UGameCharacter* loc5 = currentAvatar->GetCharRef();
	ResetHexes();
	//clearActionText();

	/*if (DragonMain.DEBUG_LOS)
	{
		stageGUI.graphics.clear();
		auto loc19 = 0;
		auto loc20 = hexes;
		for(loc10 in loc20)
		{
			loc11 = GetHexBoundsForLOS(loc10);
			stageGUI.graphics.lineStyle(1, 65280, 0.75);
			stageGUI.graphics.drawRect(loc11->left, loc11->top, loc11->width, loc11->height);
			stageGUI.graphics.drawCircle(loc10.x, loc10.y, 3);
		}
	}*/

	FString sActionText;//DHK temp waiting for text widget

	if (!skill->IsUsable())
	{
		FString loc12 = skill->consumable == nullptr ? skill->skillname : skill->consumable->itemName;
		if (skill->powerCost > loc5->power)
		{
			//actionText.text = "Insufficient " + packages.characters.Classes.powerString(loc5->classId) + " to use " + loc12;
			sActionText = "Insufficient " + PowerString(loc5->classId) + " to use " + loc12;
		}
		else if (skill->currentCooldown > 0)
		{
			if (skill->currentCooldown != 1)
			{
				//actionText.text = "Must wait " + skill->currentCooldown + " turns to use " + loc12;
				sActionText = "Must wait " + FString::FromInt(skill->currentCooldown) + " turns to use " + loc12;
			}
			else
			{
				//actionText.text = "Must wait " + skill->currentCooldown + " turn to use " + loc12;
				sActionText = "Must wait " + FString::FromInt(skill->currentCooldown) + " turn to use " + loc12;
			}
		}
		else
		{
			//actionText.text = "You cannot use " + loc12 + " at this time";
			sActionText = "You cannot use " + loc12 + " at this time";
		}
		//dmf.boldText(actionText);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *sActionText);
		UpdateHexes();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("*****"));
	UE_LOG(LogTemp, Warning, TEXT("highlightSkillTargets - char %s"), *loc5->charName);

	for (int32 i = 0; i < hexes.Num(); i++)
	{
		AHex* loc8 = hexes[i];
		loc8->obstructedTestFailed = false;
	}

	bool loc9 = false;

	if (skill->moveOnly)
	{
		SetMovementHexes(/*loc5->GetSpeed() + skill->movementBonus*/);
	}
	else if (skill->targetsTemplate)
	{
		//template.gotoAndStop(skill->targetsTemplate);//TODO
		TArray<AHex*> loc13 = GetHexesUnderTemplate(GetCurrentHex()->neighbours[HEX_RIGHT], skill);
		loc13.Append(GetHexesUnderTemplate(GetCurrentHex()->neighbours[HEX_LEFT], skill));

		UE_LOG(LogTemp, Warning, TEXT("set up template targets: %i found"), loc13.Num());

		for (int32 i = 0; i < loc13.Num(); i++)
		{
			AHex* loc8 = loc13[i];
			loc8->SetMode(HEX_MODE_TARGET, skill->skillId);
		}
	}
	else
	{
		if (skill->hitsAllFriendlies)
		{
			SetDirectHexes(99, skill);
		}
		else if (skill->rangedAttack)
		{
			SetRangedHexes(skill);
		}
		else if (skill->directAttack || skill->pointBlankAE)
		{
			SetDirectHexes(skill->GetRange(), skill);
		}
		else if (!skill->targetsSelf)
		{
			SetMovementHexes(/*loc5->GetSpeed()*/);
			loc9 = true;
		}

		for (int32 i = 0; i < avatars.Num(); i++)
		{
			ADragonAgeJourneysCharacter* loc6 = avatars[i];
			AHex* loc14 = loc6->hex;
			UGameCharacter* loc15 = loc6->GetCharRef();

			UE_LOG(LogTemp, Warning, TEXT("---"));
			UE_LOG(LogTemp, Warning, TEXT("** testing skill use on %s"), *loc15->charName);

			//TODO stencil not working currently
			/*if (AreAllies(GetParty()->GetHeroCharacter(), loc15))
			{
				UE_LOG(LogTemp, Warning, TEXT("Stenciling friendly outline..."));
				loc15->avatar->SetOutline(STENCIL_FRIENDLY_OUTLINE, true);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Stenciling baddies outline..."));
				loc15->avatar->SetOutline(STENCIL_ENEMY_OUTLINE, true);
			}*/

			if (skill->targetsDead)
			{
				if (loc15->health > 0)
				{
					continue;
				}

				if (skill->targetsFriendlies)
				{
					if (loc15->group != loc5->group)
					{
						continue;
					}
				}
				else if (loc15->group == loc5->group)
				{
					continue;
				}

				if (!loc6->deathHex)
				{
					continue;
				}
				if (loc6->deathHex->GetAnAvatar() != nullptr)
				{
					continue;
				}

				loc14 = loc6->deathHex;
			}
			else if (loc15->IsInactive())
			{
				continue;
			}
			else if (!(loc15 == loc5) && skill->targetsSelf)
			{
				continue;
			}
			else if (loc15 == loc5 && !skill->targetsSelf && !skill->targetsFriendlies && !skill->targetsAllCharacters)
			{
				continue;
			}
			else if (!(loc15 == loc5) && loc15->group == loc5->group && !skill->targetsFriendlies && !skill->targetsAllCharacters)
			{
				continue;
			}
			else if (!(loc15->group == loc5->group) && skill->targetsFriendlies && !skill->targetsAllCharacters)
			{
				continue;
			}

			UDijkstra* loc17 = nullptr;
			int32 loc16;

			if (loc9)
			{
				loc17 = currentTurnMoveSearch;
			}

			if (skill->directAttack)
			{
				UE_LOG(LogTemp, Warning, TEXT("targetHex %i %i || CurrentHex %i %i"), loc14->gridX, loc14->gridY, GetCurrentHex()->gridX, GetCurrentHex()->gridY);
				loc16 = GetRawDistanceBetweenHexes(loc14, GetCurrentHex());
			}
			else
			{
				loc16 = GetHexDistanceOccupied(loc14, loc17);
				UE_LOG(LogTemp, Warning, TEXT("battlefield: %s has the hex of %i"), *loc15->charName, loc16);
			}

			if (loc6->GetCharRef()->IsLargeCreature())
			{
				for (int32 j = 0; j < loc6->largeHexes.Num(); j++)
				{
					AHex* loc18 = loc6->largeHexes[j];
					if (skill->directAttack)
					{
						loc16 = GetRawDistanceBetweenHexes(loc18, GetCurrentHex());
					}
					else
					{
						loc16 = GetHexDistanceOccupied(loc18, loc17);
					}

					MarkTargetHexForSkill(skill, loc16, loc18);
				}
				continue;
			}

			MarkTargetHexForSkill(skill, loc16, loc14);
		}
	}

	UpdateHexes();
}

void UBattlefield::UseInstantSkill(USkill* skill)
{
	AddSkillActions(skill, GetCurrentCharacter(), GetCurrentHex()->index);
	if (!(skill->type == SKILL_TYPE_TOGGLED && skill->toggleEndsTurn == false))
	{
		ResetHexes();
		//TODO RemoveBars
		//RemoveBars();
	}
}

void UBattlefield::ToggleOffSkill(USkill* skill, bool bTurnOff /*= false*/, bool arg3 /*= true*/)
{
	if (!bTurnOff)
	{
		skill->toggledOn = false;
	}
	if (skill->char_ref)
	{
		skill->char_ref->RemoveEffectWithId(skill->selfEffectId);
		if (skill->hitsAllFriendlies && skill->targetEffectId)
		{
			for (int32 i = 0; i < allies.Num(); i++)
			{
				UGameCharacter* loc1 = allies[i];
				loc1->RemoveEffectWithId(skill->targetEffectId);
			}
		}

		//TODO animation toggle off
		/*if (skill->selfEffectId && arg3)
		{
			AddAnimationToAvatar(new packages.battle.CasterAnim(new generic_toggle_off()), skill->char_ref.avatar, skill);
		}*/
	}

	//TODO visual/audio toggle off
	/*if (arg3)
	{
		packages.audio.Sounds.play("toggle_off");
	}

	skillbar.updateToggles();
	bars_component.effectBar.updateEffectIcons();*/
}

void UBattlefield::ShowMovementPath(AHex* hex)
{
	//reset hexes
	ClearMovementPath();

	TArray<AHex*> _path = GetRouteTo(hex);

	for (int32 i = 0; i < _path.Num(); i++)
	{
		GetCurrentPlayerController()->SetBattleHexMaterialInstance(HEX_MODE_SHOWPATH, _path[i]);
	}
}

void UBattlefield::ClearMovementPath()
{
	USkill* _skill = GetQuickBar()->GetSelectedSkill();
	GetMain()->Battlefield->HighlightSkillTargets(_skill);
	return;
}

void UBattlefield::SetMoveActionForHex(AHex* hex)
{
	UAction* _action = NewObject<UAction>();
	_action->InitializeAction(ACTION_MOVE, GetCurrentCharacter());
	_action->moveToHexIndex = hex->index;
	actionQueue.Add(_action);
}

void UBattlefield::SetPlayerActionForHex(AHex* hex)
{
	USkill* _selectedSkill = GetQuickBar()->GetSelectedSkill();

	UAction* moveAction = NewObject<UAction>();
	moveAction->InitializeAction(ACTION_MOVE, GetCurrentCharacter());

	int32 nMoveIndex = FindMovementIndexForPlayerSkill(hex, _selectedSkill);

	UE_LOG(LogTemp, Warning, TEXT("battlefield SetPlayerActionForHex:  find move my is %i"), nMoveIndex);

	if (nMoveIndex != -1)
	{
		AHex* moveHex = hexes[nMoveIndex];

		if (moveHex != nullptr && moveHex->CanEndMoveHere(GetCurrentCharacter()))
		{
			moveAction->moveToHexIndex = nMoveIndex;
			actionQueue.Add(moveAction);
		}
	}

	int32 nAttack = 1;
	while (nAttack <= _selectedSkill->attacksMultipleTimes)
	{
		AddSkillActions(_selectedSkill, GetCurrentCharacter(), hex->index, nAttack);
		++nAttack;
	}

	SetAvatarRotation(GetCurrentCharacter()->avatar, hex);

	ResetHexes();
	//RemoveBars();
	UpdateHexes();
}

void UBattlefield::SetActionIndicatorForHex(AHex* hex)
{
	//TODO SetActionIndicatorForHex
}

void UBattlefield::SetActionTextForHex(AHex* hex)
{
	FString sActionText;//DHK temp waiting for text widget

	USkill* loc1 = GetQuickBar()->GetSelectedSkill();
	if (loc1 == nullptr)
	{
		return;
	}

	if (!IsPlayerTurn())
	{
		ClearActionText();
		return;
	}

	if (hex->GetMode() == HEX_MODE_MOVE || hex->GetMode() == HEX_MODE_MOVEATTACK)
	{
		//actionText.text = "Move to empty space";
		sActionText = "Move to empty space";
		if (loc1 && loc1->moveOnly)
		{
			//actionText.appendText(" and end the turn");
			sActionText += " and end the turn";
		}
		//dmf.boldText(actionText);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *sActionText);
	}
	else if (hex->GetMode() != HEX_MODE_TARGET)
	{
		ClearActionText();
	}
	else if (hex->GetAnAvatar() == nullptr)
	{
		//actionText.text = "Use " + loc1->skillname;
		sActionText = "Use " + loc1->skillname;
		if (loc1->targetsEmpty)
		{
			//actionText.appendText(" on empty space");
			sActionText += " on empty space";
		}
		//dmf.boldText(actionText);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *sActionText);
	}
	else
	{
		if (GetHexDistanceOccupied(hex) > loc1->GetRange())
		{
			//actionText.text = "Move and use " + loc1->skillname + " on " + hex->GetAnAvatar()->GetCharRef()->charName;
			sActionText = "Move and use " + loc1->skillname + " on " + hex->GetAnAvatar()->GetCharRef()->charName;
		}
		else if (!loc1->rangedAttack && hex->directionHex != nullptr && hex->directionHex != GetCurrentHex())
		{
			//actionText.text = "Move and use " + loc1->skillname + " on " + hex->GetAnAvatar()->GetCharRef()->charName;
			sActionText = "Move and use " + loc1->skillname + " on " + hex->GetAnAvatar()->GetCharRef()->charName;
		}
		else
		{
			//actionText.text = "Use " + loc1->skillname + " on " + hex->GetAnAvatar()->GetCharRef()->charName;
			sActionText = "Use " + loc1->skillname + " on " + hex->GetAnAvatar()->GetCharRef()->charName;
		}
		//dmf.boldText(actionText);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *sActionText);
	}
}

void UBattlefield::ClearActionText()
{
	//TODO ClearActionText
	//actionText.text = "";
}

void UBattlefield::SetAvatarRotation(ADragonAgeJourneysCharacter* avatar, AHex* hex)
{
	FRotator rActor = avatar->GetActorRotation();
	FRotator _rotation;
	_rotation = UKismetMathLibrary::FindLookAtRotation(avatar->GetActorLocation(), hex->GetActorLocation());
	_rotation.Pitch = 0;

	if (FMath::Abs(FMath::Abs(_rotation.Yaw) - FMath::Abs(rActor.Yaw)) > 30)
	{
		avatar->SetActorRotation(FRotator(0, _rotation.Yaw, 0));
	}

	if (FMath::FloorToInt(FMath::Abs(FMath::Abs(_rotation.Yaw))) == FMath::FloorToInt(FMath::Abs(rActor.Yaw)))
	{
		avatar->SetActorRotation(FRotator(0, _rotation.Yaw, 0));
	}
}

void UBattlefield::SetAvatarSpeed(ADragonAgeJourneysCharacter* avatar, float fSpeed)
{
	if (avatar->AnimInstance->GetMovementSpeed() != fSpeed)
	{
		avatar->AnimInstance->SetMovementSpeed(fSpeed);
	}
}

int32 UBattlefield::FindMovementIndexForPlayerSkill(AHex* hex, USkill* skill)
{
	float loc2 = GetHexDistanceOccupied(hex);
	bool loc3 = false;

	if (skill->rangedAttack)
	{
		loc3 = false;
	}
	else if (skill->directAttack)
	{
		loc3 = false;
	}
	else if (GetCurrentCharacter()->IsMage())
	{
		loc3 = false;
	}
	else if (hex->directionHex != GetCurrentHex())
	{
		loc3 = true;
	}

	if (loc3 && hex->directionHex != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("battlefield FindMovementIndexForPlayerSkill:  true and hex direction is %i"), hex->directionHex->index);
		return hex->directionHex->index;
	}

	return -1;
}

int32 UBattlefield::CountCorpses()
{
	int32 nCorpse = 0;

	for (int32 i = 0; i < characters.Num(); i++)
	{
		UGameCharacter* battleCharacter = characters[i];

		if (battleCharacter && battleCharacter->health <= 0)
		{
			++nCorpse;
		}
	}

	return nCorpse;
}

int32 UBattlefield::CalculateDamage(USkill* skill, UGameCharacter* attacker, UGameCharacter* target, bool bGlancing)
{
	float loc1 = 1.5;
	float loc2 = 1.15;
	int32 loc3 = skill->GetDamage();

	if (loc3 <= 0)
	{
		return 0;
	}

	if (IsDarkspawn(target->GetRace()))
	{
		loc3 = loc3 + attacker->GetBonusDamageToDarkspawn();
	}

	float loc4 = 1 + (25 - FMath::RandRange(0, 50)) * 0.01;
	loc3 = loc3 * loc4;

	if (attacker->IsBerserk())
	{
		loc3 = loc3 * 1.1;
	}

	if (skill->addsPowerToDamageRatio)
	{
		loc3 = loc3 + GetCurrentCharacter()->old_power * skill->addsPowerToDamageRatio;
	}

	for (int32 i = 0; i < attacker->effects.Num(); i++)
	{
		UEffect* loc5 = attacker->effects[i];
		if (loc5->weaponDamageModifier == 0 && skill->IsWeaponAttack())
		{
			loc3 = loc3 + loc5->weaponDamageModifier;
			UE_LOG(LogTemp, Warning, TEXT("adding %i extra damage from effect %s"), loc5->weaponDamageModifier, *GetEffectName(loc5->effectId));
		}
	}

	if (bGlancing)
	{
		loc3 = loc3 * 0.2;
	}
	else
	{
		if (IsFlankAttack(attacker, target, skill))
		{
			currentAction->flank = true;
			loc3 = loc3 * loc2;
		}

		if (IsBackstab(attacker, target, skill))
		{
			currentAction->backstab = true;
			loc3 = loc3 * loc1;
		}

		bool loc9 = false;

		if (attacker->GetCriticalRate() > FMath::RandRange(0, 99) && !skill->IsSpell())
		{
			loc9 = true;
		}
		else if (skill->critsAgainstTargetsWithEffectId && target->HasEffectWithId(skill->critsAgainstTargetsWithEffectId))
		{
			loc9 = true;
		}

		if ((skill->automaticCritical || loc9) && !bGlancing)
		{
			loc3 = loc3 * 2 * attacker->GetCriticalDamageMultiplier();
			attacker->scoredCritical = true;
		}
		else
		{
			attacker->scoredCritical = false;
		}
	}

	int32 loc7 = target->GetIncomingDamageMultiplier();
	if (loc7 == 0)
	{
		return 0;
	}

	loc3 = loc3 * loc7;

	int32 loc8 = 0;
	if (skill->damageType == EFFECT_PHYSICAL)
	{
		loc8 = FMath::Max(0, target->GetArmor() - attacker->GetPenetration());
	}

	loc3 = loc3 - loc8;

	return FMath::Max(1, FMath::RoundToInt(loc3));
}

int32 UBattlefield::GetRawDistanceBetweenHexes(AHex* hex1, AHex* hex2)
{
	if (hex1 && hex2)
	{
		return rawDistances[hex1->index].arrInt32[hex2->index];
	}

	return 999;
}

int32 UBattlefield::GetClosestEnemyDistanceTo(UGameCharacter* gameCharacter)
{
	if (!gameCharacter->avatar)
	{
		UE_LOG(LogTemp, Warning, TEXT("!!!WARNING!!! - invalid param passed to getClosestEnemyDistanceTo()"));
		return 0;
	}

	auto loc3 = 999;
	auto loc4 = 999;

	if (gameCharacter->IsMonster())
	{
		for (int32 i = 0; i < allies.Num(); i++)
		{
			UGameCharacter* loc1 = allies[i];
			AHex* loc2 = loc1->avatar->hex;
			loc3 = GetDistanceFromMonster(gameCharacter, loc2);

			if (loc3 < loc4)
			{
				loc4 = loc3;
			}
		}
	}
	else
	{
		for (int32 i = 0; i < enemies.Num(); i++)
		{
			UGameCharacter* loc1 = enemies[i];
			AHex* loc2 = loc1->avatar->hex;
			loc3 = GetHexDistanceOccupied(loc2);
			if (loc3 < loc4)
			{
				loc4 = loc3;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("getClosestEnemyDistanceTo(%s) - found closest enemy %i hexes away"), *gameCharacter->charName, loc4);

	return loc4;
}

int32 UBattlefield::GetDistanceFromMonster(UGameCharacter* gameCharacter, AHex* hex)
{
	auto loc1 = 0;
	auto loc4 = 0;

	if (gameCharacter->IsLargeCreature())
	{
		loc1 = 999;
		TArray<FInt32Array> loc2 = BuildAdjacencyMap();

		for (int32 i = 0; i < gameCharacter->avatar->largeHexes.Num(); i++)
		{
			AHex* loc3 = gameCharacter->avatar->largeHexes[i];
			loc4 = 999;
			if (loc3 != GetCurrentHex())
			{
				UDijkstra* loc5 = NewObject<UDijkstra>();
				loc5->InitializeDijkstra(loc2, loc3->index);
				loc5->FindShortestPath();
				loc4 = GetHexDistanceOccupied(hex, loc5);
			}
			else
			{
				loc4 = GetHexDistanceOccupied(hex);
			}

			if (loc4 < loc1)
			{
				loc1 = loc4;
			}
		}

		return loc1;
	}
	return GetHexDistanceOccupied(hex);
}

float UBattlefield::GetHexDistanceOccupied(AHex* argHex, UDijkstra* dijkstra /*= nullptr*/)
{
	if (dijkstra == nullptr)
	{
		dijkstra = dSearch;
	}

	TArray<AHex*> arNeighbours = HexNeighbours(argHex);
	float fDistance = 99999999;

	for (int32 i = 0; i < arNeighbours.Num(); i++)
	{
		AHex* hex = arNeighbours[i];
		fDistance = FMath::Min(HexDistance(hex, dijkstra), fDistance);
	}

	return (fDistance++);
}

bool UBattlefield::AreAllies(UGameCharacter* gameChar1, UGameCharacter* gameChar2)
{
	if (gameChar1->group == gameChar2->group)
	{
		return true;
	}

	return false;
}

bool UBattlefield::IsPlayerTurn()
{
	bool bPlayerTurn = false;

	if (suspendTurnOrder)
	{
		return false;
	}

	if (actionQueue.Num() > 0)
	{
		bPlayerTurn = false;
	}
	else if (currentTurnCharacter == nullptr)
	{
		bPlayerTurn = false;
	}
	else if (currentTurnCharacter->ai != nullptr)
	{
		bPlayerTurn = false;
	}
	else if (currentAvatar == nullptr)
	{
		bPlayerTurn = false;
	}
	else if (moveRoute.Num() > 0)
	{
		bPlayerTurn = false;
	}
	/*else if (moveRoute.Num() == 0 && GetCurrentCharacter()->moved > 0)//DHK
	{
		bPlayerTurn = false;
	}*/
	else if (currentAvatar->GetCharRef()->group->player)
	{
		bPlayerTurn = true;
	}

	return bPlayerTurn;
}

bool UBattlefield::LineOfSightBetween(AHex* hexStart, AHex* hexEnd, UGameCharacter* gameCharacter)
{
	TArray<AHex*> loc1 = GetHexesBetween(hexStart, hexEnd);
	for (int32 i = 0; i < loc1.Num(); i++)
	{
		AHex* loc2 = loc1[i];
		if (loc2->CanShoot(gameCharacter))
		{
			continue;
		}

		return false;
	}

	return true;
}

bool UBattlefield::IsFlankAttack(UGameCharacter* gameChar1, UGameCharacter* gameChar2, USkill* skill, AHex* hex /*= nullptr*/)
{
	TArray<UGameCharacter*> loc2;
	UGameCharacter* loc1 = nullptr;

	if (gameChar2->IsImmuneToFlanking())
	{
		return false;
	}

	if (skill->GetDamage() <= 0)
	{
		return false;
	}

	if (skill->GetRange() > 1)
	{
		return false;
	}

	if (hex == nullptr)
	{
		hex = gameChar1->avatar->hex;
	}

	if (gameChar1->IsMonster())
	{
		for (int32 i = 0; i < enemies.Num(); i++)
		{
			loc2.Add(enemies[i]);//loc2 = enemies;
		}
	}
	else
	{
		for (int32 i = 0; i < allies.Num(); i++)
		{
			loc2.Add(allies[i]);//loc2 = allies;
		}
	}

	for (int32 i = 0; i < loc2.Num(); i++)
	{
		UGameCharacter* loc3 = loc2[i];

		if (loc3 == gameChar1)
		{
			continue;
		}

		if (loc3->IsInactive())
		{
			continue;
		}

		if (!loc3->avatar->hex)
		{
			continue;
		}

		for (int32 j = 0; j < loc3->avatar->hex->neighbours.Num(); j++)
		{
			AHex* loc5 = loc3->avatar->hex->neighbours[j];

			if (loc5 == nullptr)
			{
				continue;
			}

			if (loc5 == hex)
			{
				UE_LOG(LogTemp, Warning, TEXT("ally %s neighbours the attacker! not a valid flank assistant"), *loc3->charName);
				loc1 = nullptr;
				continue;
			}

			if (loc5->GetAnAvatar() != gameChar2->avatar)
			{
				continue;
			}

			loc1 = loc3;
		}

		if (!loc1)
		{
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("Flank attack found: our assistant is %s"), *loc1->charName);
		return true;
	}

	return false;
}

bool UBattlefield::IsBackstab(UGameCharacter* gameChar1, UGameCharacter* gameChar2, USkill* skill)
{
	for (int32 i = 0; i < gameChar1->effects.Num(); i++)
	{
		UEffect* effect = gameChar1->effects[i];
		if (effect->cancelBackstabs)
		{
			return false;
		}
	}

	if (gameChar1->IsHidden() && skill->skillId == SKILL_STRIKE)
	{
		return true;
	}

	if (IsBehindTargetBack(gameChar1->avatar, gameChar2->avatar) && gameChar1->moved == 0)
	{
		if (skill->skillId == SKILL_STRIKE)
		{
			return true;
		}
	}

	return false;
}

bool UBattlefield::PlayerActionAllowed()
{
	if (currentTurnCharacter && (currentTurnCharacter->IsMonster() || currentTurnCharacter->usedSkill))
	{
		return false;
	}

	return true;
}

bool UBattlefield::IsMoving()
{
	return moveRoute.Num() > 0;
}

UGroup* UBattlefield::GetEnemyGroup()
{
	if (groups.Num() < 2)
	{
		return nullptr;
	}
	return groups[1];
}

TArray<AHex*> UBattlefield::GetHexesInRange(int32 nMoves)
{
	TArray<AHex*> arRange;

	for (int32 i = 0; i < hexes.Num(); i++)
	{
		AHex* hex = hexes[i];
		if (dSearch->distance[hex->index] <= nMoves)
		{
			arRange.Add(hex);
		}
	}

	return arRange;
}

TArray<AHex*> UBattlefield::GetHexesInMoveRange()
{
	return GetHexesInRange(GetCurrentCharacter()->GetSpeed());
}

ATrap* UBattlefield::TrapOnHex(AHex* hex)
{
	//TODO TrapOnHex
	return nullptr;
}

UBattlefield::~UBattlefield()
{
}

void UBattlefield::SetupCharacters()
{
	for (int32 i = 0; i < characters.Num(); i++)
	{
		UGameCharacter* gameChar = characters[i];
		initStack->AddCharacter(gameChar);
	}
}

void UBattlefield::GenerateAvatars()
{
	SetBattleView();

	for (int32 i = 0; i < groups.Num(); i++)
	{
		for (int32 j = 0; j < groups[i]->chars.Num(); j++)
		{
			UGameCharacter* gameChar = groups[i]->chars[j];
			if (gameChar == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Battlefield.GenerateAvatars: Can\'t find character with index %i "), i);
			}

			UGroup* group = gameChar->group;

			TArray<int32> arPositions = group->GetBattlePositions();
			gameChar->preBattleHealth = gameChar->health;

			if (gameChar->ai != nullptr)
			{
				int32 nSkill = 1;
				USkill* skill = nullptr;
				while (nSkill < gameChar->GetSkills().Num())
				{
					skill = gameChar->GetSkills()[nSkill];
					if (skill && skill->skillId != SKILL_EMISSARY_ESCAPE)
					{
						skill->currentCooldown = FMath::RandRange(1, 3);
					}
					++nSkill;
				}
			}

			int32 posX = 0;
			int32 posY = arPositions[j];
			if (group->player)
			{
				allies.Add(gameChar);
			}
			else
			{
				enemies.Add(gameChar);
				posX = NUM_HEXES_X - 1;
			}

			//DHK
			ADragonAgeJourneysCharacter* avatar = nullptr;
			for (TActorIterator<ADragonAgeJourneysCharacter> ActorItr(GetStage()); ActorItr; ++ActorItr)
			{
				ADragonAgeJourneysCharacter *mapAvatar = *ActorItr;
				if (mapAvatar->GetCharRef() == gameChar)
				{
					avatar = mapAvatar;
					break;
				}
			}

			if (!avatar)
			{
				FActorSpawnParameters SpawnParams;
				avatar = GetStage()->SpawnActor<ADragonAgeJourneysCharacter>(GetMain()->LevelMap->refAvatar, SpawnParams);
				avatar->InitializeAvatar(gameChar, false);
			}
			//Set Combat Stance for alreay existing actors that already have AnimInstance
			else
			{
				if (avatar->MeshComponentStaticRGT->GetStaticMesh())
				{
					avatar->EquipWeapon(true);
				}

				if (avatar->MeshComponentStaticLFT->GetStaticMesh())
				{
					avatar->EquipWeapon(false);
				}

				avatar->SetCombatStance();

			}

			//DHK scale for Battle
			avatar->SetActorRelativeScale3D(FVector(0.5, 0.5, 0.5));

			if (gameChar->creatureWidth > 1)
			{
				posX = posX - (gameChar->creatureWidth - 2);
			}

			AHex* posHex = grid[posX].arrHex[posY];
			if (gameChar->health > 0)
			{
				posHex->AttachAvatar(avatar);
			}
			else
			{
				avatar->deathHex = posHex;
			}

			avatar->SetActorLocation(FVector(posHex->GetActorLocation().X, posHex->GetActorLocation().Y, GetMain()->LevelMap->MapZ));

			if (group->player)
			{
				avatar->SetFacingLeft(HEX_RIGHT);
			}
			else
			{
				avatar->SetFacingLeft(HEX_LEFT);
			}

			//TODO large creature needed in 3D?
			if (gameChar->IsLargeCreature())
			{
				SetLargeCreatureHexes(gameChar);
			}

			//TEMP rotation?
			FRotator _rotation = UKismetMathLibrary::FindLookAtRotation(avatar->GetActorLocation(), GetCurrentPlayerController()->GetPawn()->GetActorLocation());
			_rotation.Pitch = 0;
			avatar->SetActorRotation(_rotation);

			//InitAnimation(avatar);

			avatars.Add(avatar);

			//TODO Portraits
			/*auto loc9 = new packages.gui.Portrait(new PortraitSWF(), loc2);
			if (loc3->player)
			{
			loc9->clip.x = packages.gui.Portrait.PORTRAIT_GAP;
			}
			else
			{
			loc9->clip.x = DragonMain.APP_WIDTH - packages.gui.Portrait.PORTRAIT_GAP - packages.gui.Portrait.PORTRAIT_SIZE;
			}
			if (loc9->charRef && loc9->GetCharRef()->health <= 0)
			{
			UE_LOG(LogTemp, Warning, TEXT(loc9->GetCharRef()->name + " and battlefield called final dead");
			loc9->dead(true, true);
			}
			loc9->clip.y = 15 + loc1 * (packages.gui.Portrait.PORTRAIT_GAP + packages.gui.Portrait.PORTRAIT_SIZE);
			portraits.Add(loc9);
			loc9->update(true);
			if (loc3->player)
			{
			loc9->hit.addEventListener(flash.events.MouseEvent.CLICK, portraitClick, false, 0, true);
			}
			else
			{
			loc9->clip.buttonMode = false;
			}
			loc2->portrait = loc9;*/

			//sign.loadStageComplete(_generatedCharacterCount, characters.Num());
		}
	}
}

void UBattlefield::SetBattleView()
{
	//create invisible character
	FActorSpawnParameters SpawnParams;
	battleView = GetStage()->SpawnActor<ADragonAgeJourneysCharacter>(GetMain()->LevelMap->refAvatar, SpawnParams);

	/*FString sName = "battleView";
	const TCHAR* arChar = *sName;
	battleView->Rename(arChar, this->GetOuter(), REN_None);*/

	GetStage()->GetFirstPlayerController()->Possess(battleView);

	/*float maxX = 0.f;
	float maxY = 0.f;

	for (int32 i = 0; i < hexes.Num(); i++)
	{
	maxX = FMath::Max(maxX, hexes[i]->GetActorLocation().X);
	maxY = FMath::Max(maxY, hexes[i]->GetActorLocation().Y);
	}*/

	//battleView->SetActorLocation(FVector(maxX / 1.55, maxY / 1.45, GetMain()->LevelMap->MapZ));


	for (int32 i = 0; i < hexes.Num(); i++)
	{
		if (hexes[i]->gridX == NUM_HEXES_X / 2 && hexes[i]->gridY == NUM_HEXES_Y / 2)
		{
			float _x = hexes[i]->GetActorLocation().X;
			float _y = hexes[i]->GetActorLocation().Y;
			battleView->SetActorLocation(FVector(_x - HEX_WIDTH / 2, _y, -50 /*GetMain()->LevelMap->MapZ*/));
			battleView->GetTopDownCameraComponent()->OrthoWidth = OrthoWidthCombat;
			//battleView->GetCursorToWorld()->SetVisibility(false,true);
			//battleView->GetCursorToWorld() = nullptr;
		}
	}

	battleView->SetActorHiddenInGame(true);
}

void UBattlefield::GenerateHexNeighbours()
{
	AHex* hex = nullptr;

	for (int32 i = 0; i < hexes.Num(); i++)
	{
		hex = hexes[i];
		TArray<AHex*> arrHex = HexNeighbours(hex, true);
		for (int32 j = 0; j < arrHex.Num(); j++)
			hex->neighbours.Add(arrHex[j]);
	}
}

void UBattlefield::PrecomputeDistances()
{
	int32 loc4 = 0;
	UDijkstra* loc5 = NewObject<UDijkstra>();
	int32 loc1 = NUM_HEXES_X * NUM_HEXES_Y;

	TArray<FInt32Array> loc2; loc2.SetNum(loc1);

	int32 loc3 = 0;
	while (loc3 < loc1)
	{
		loc2[loc3].arrInt32.SetNum(loc1);// = arrHex /*new Array(loc1)*/;
		loc4 = 0;
		while (loc4 < loc1)
		{
			if (loc3 != loc4)
			{
				if (HexesAdjacent(hexes[loc3], hexes[loc4]))
				{
					loc2[loc3].arrInt32[loc4] = 1;
				}
				else
				{
					loc2[loc3].arrInt32[loc4] = NO_CONNECTION;
				}
			}
			else
			{
				loc2[loc3].arrInt32[loc4] = 0;
			}

			++loc4;
		}

		++loc3;
	}

	rawDistances.SetNum(loc1);

	loc3 = 0;
	while (loc3 < loc1)
	{
		loc5->InitializeDijkstra(loc2, loc3);
		loc5->FindShortestPath();
		for (int32 i = 0; i < loc5->distance.Num(); i++)
		{
			//rawDistances[loc3] = loc5->distance.slice();
			rawDistances[loc3].arrInt32.Add(loc5->distance[i]);
		}

		if (rawDistances[loc3].arrInt32.Num() != loc1)
		{
			UE_LOG(LogTemp, Warning, TEXT("!!!WARNING!!! - precompute: Num() mismatch! (i= %i, size= %i, Num()= %i)"), loc3, loc1, rawDistances[loc3].arrInt32.Num());
		}

		++loc3;
	}
}

void UBattlefield::RemoveStealthEffects(UGameCharacter* gameCharacter)
{
	gameCharacter->RemoveEffectWithId(EFFECT_STEALTH);
	gameCharacter->avatar->AddFloatingText("Stealth broken!");
	//Sounds.play("stealth_broken", 1);//TODO search for 'Sounds.'
	if (GetBaseClassOf(gameCharacter->classId) == CLASS_ROGUE)
	{
		USkill* loc1 = gameCharacter->FindSkillWithId(SKILL_STEALTH1);
		if (loc1)
		{
			loc1->toggledOn = false;
			loc1->currentCooldown = 1;
		}
	}
}

void UBattlefield::SetLargeCreatureHexes(UGameCharacter* gameCharacter)
{
	if (gameCharacter == nullptr) return;

	ADragonAgeJourneysCharacter* avatar = gameCharacter->avatar;

	avatar->ClearLargeHexes();

	int32 nWidth = gameCharacter->creatureWidth;
	int32 nHeight = gameCharacter->creatureHeight;

	AHex* hex = avatar->hex;
	avatar->AddLargeHex(hex);

	if (nWidth >= 3)
	{
		avatar->AddLargeHex(hex->neighbours[1]);
		avatar->AddLargeHex(hex->neighbours[4]);
	}
}

void UBattlefield::SetMovementHexes(/*int32 nSpeed*/)
{
	int32 loc1 = 0;
	if (currentTurnCharacter != nullptr)
	{
		loc1 = currentTurnCharacter->GetSpeed() + 1;
	}

	if (currentTurnMoveSearch == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("can\'t find cached movement map!"));
		return;
	}

	if (GetCurrentHex() != currentTurnStartHex)
	{
		currentTurnStartHex->SetMode(HEX_MODE_MOVE, -1);
	}

	auto loc2 = 0;
	while (loc2 < currentTurnMoveSearch->distance.Num())
	{
		if (currentTurnMoveSearch->distance[loc2] > 0 && currentTurnMoveSearch->distance[loc2] < loc1)
		{
			AHex* loc3 = hexes[loc2];
			if (loc3 != GetCurrentHex() && loc3->CanMove(currentTurnCharacter))
			{
				loc3->SetMode(HEX_MODE_MOVE, -1);
			}
		}
		++loc2;
	}
}

void UBattlefield::SetDirectHexes(int32 nDistance, USkill* skill)
{
	for (int32 i = 0; i < hexes.Num(); i++)
	{
		AHex* loc1 = hexes[i];

		if (loc1 == GetCurrentHex())
		{
			continue;
		}

		int32 loc2 = GetRawDistanceBetweenHexes(loc1, GetCurrentHex());

		if (loc2 <= nDistance)
		{
			if (loc1->obstructedTestFailed)
			{
				loc1->SetMode(HEX_MODE_NORMAL, skill->skillId);
				continue;
			}

			loc1->SetMode(HEX_MODE_RANGED, skill->skillId);
		}
	}
}

void UBattlefield::SetRangedHexes(USkill* skill)
{
	//TODO
	/*if (debugLayer.parent == nullptr)
	{
		addChild(debugLayer);
	}
	debugLayer.graphics.clear();*/

	if (skill == nullptr)
	{
		return;
	}

	for (int32 i = 0; i < obstructedHexes.Num(); i++)
	{
		AHex* loc1 = obstructedHexes[i];

		if (loc1 == GetCurrentHex())
		{
			continue;
		}

		SetShadowedHexes(GetCurrentHex(), loc1, skill->GetRange());
	}

	int32 loc2 = 0;
	while (loc2 < dSearch->distance.Num())
	{
		if (loc2 != GetCurrentHex()->index)
		{
			if (dSearch->distance[loc2] <= skill->GetRange() && dSearch->distance[loc2] >= skill->GetMinimumRange())
			{
				if (hexes[loc2]->obstructedTestFailed)
				{
					hexes[loc2]->SetMode(HEX_MODE_NORMAL, skill->skillId);
				}
				else if (skill->targetsEmpty)
				{
					hexes[loc2]->SetMode(HEX_MODE_TARGET, skill->skillId);
				}
				else
				{
					hexes[loc2]->SetMode(HEX_MODE_RANGED, skill->skillId);
				}
			}
		}

		++loc2;
	}
}

void UBattlefield::SetShadowedHexes(AHex* hexStart, AHex* hexEnd, int32 nDistance)
{
	float maxX = 0.f;
	float maxY = 0.f;

	for (int32 i = 0; i < hexes.Num(); i++)
	{
		maxX = FMath::Max(maxX, hexes[i]->GetActorLocation().X);
		maxY = FMath::Max(maxY, hexes[i]->GetActorLocation().Y);
	}

	if (hexStart == hexEnd)
	{
		return;
	}

	FVector loc2 = FVector(hexEnd->GetActorLocation().X - hexStart->GetActorLocation().X, hexEnd->GetActorLocation().Y - hexStart->GetActorLocation().Y, GetMain()->LevelMap->MapZ);
	loc2.Normalize(LOS_TEST_FREQ);
	FVector loc3 = FVector(hexStart->GetActorLocation().X, hexStart->GetActorLocation().Y, GetMain()->LevelMap->MapZ);

	while (loc3.X < maxX && loc3.X > 0 && loc3.Y > 0 && loc3.Y < maxY)
	{
		loc3 = FVector(loc3.X + loc2.X, loc3.Y + loc2.Y, GetMain()->LevelMap->MapZ);

		AHex* _hex = GetHexAtPoint(loc3);
		if (_hex == nullptr || _hex == hexEnd)
		{
			continue;
		}

		if (GetRawDistanceBetweenHexes(_hex, GetCurrentHex()) > nDistance)
		{
			return;
		}

		_hex->obstructedTestFailed = true;
	}
}

void UBattlefield::MarkTargetHexForSkill(USkill* skill, int32 nDistance, AHex* hex)
{
	int32 loc1 = -1;

	if (skill->hitsAllEnemies || skill->hitsAllFriendlies)
	{
		loc1 = 99;
	}
	else if (skill->rangedAttack)
	{
		loc1 = skill->GetRange() + 1;
	}
	else if (skill->directAttack)
	{
		loc1 = skill->GetRange() + 1;
	}
	else
	{
		loc1 = GetCurrentCharacter()->GetSpeed() + skill->GetRange() + 1;
	}

	FString loc2 = "";

	if (hex->GetAnAvatar() != nullptr)
	{
		loc2 = hex->GetAnAvatar()->GetCharRef()->charName;
	}

	UE_LOG(LogTemp, Warning, TEXT("---%s---"), *loc2);
	UE_LOG(LogTemp, Warning, TEXT("marking target hex %s for skill %s - param distance = %i and our range = %i (moves left: %i)"), *hex->StringData(), *skill->skillname, nDistance, loc1, GetCurrentCharacter()->GetMovesLeft());

	if (nDistance < loc1 && nDistance >= skill->GetMinimumRange())
	{
		UE_LOG(LogTemp, Warning, TEXT(" target in range"));

		if (skill->rangedAttack && !LineOfSightBetween(GetCurrentHex(), hex, GetCurrentCharacter()))
		{
			UE_LOG(LogTemp, Warning, TEXT("  no line of sight to %s"), *hex->StringData());
			return;
		}

		hex->SetMode(HEX_MODE_TARGET, skill->skillId);

		if (!skill->rangedAttack && skill->GetRange() == 1)
		{
			hex->attackerHexes.Empty();
			int32 loc3 = 0;
			while (loc3 < hex->neighbours.Num())
			{
				if (hex->neighbours[loc3] != nullptr)
				{
					if (hex->neighbours[loc3]->GetMode() == HEX_MODE_MOVE || hex->neighbours[loc3]->GetMode() == HEX_MODE_MOVEATTACK || hex->neighbours[loc3] == GetCurrentHex())
					{
						if (!(!(hex->neighbours[loc3]->avatar == nullptr) && !(hex->neighbours[loc3] == GetCurrentHex())))
						{
							hex->attackerHexes.Add(hex->neighbours[loc3]);
							UE_LOG(LogTemp, Warning, TEXT("battlefield: possible attacker hexes at %s"), *hex->neighbours[loc3]->StringData());
						}
					}
				}

				++loc3;
			}
		}
	}
}

void UBattlefield::AddBars(bool bCurrent /*= false*/)
{
	//TODO finish AddBars
	if (suspendTurnOrder)
	{
		return;
	}

	if (bCurrent)
	{
		//bars_component.setChar(currentCharacter);
		GetQuickBar()->SetGameCharacter(currentTurnCharacter);
	}
}

void UBattlefield::ProcessActions()
{
	if (endBattle)
	{
		return;
	}

	if (actionQueue.Num() == 0)
	{
		if (currentTurnCharacter && GetCurrentCharacter() != currentTurnCharacter)
		{
			currentAvatar = currentTurnCharacter->avatar;
		}

		if (!IsPlayingAnim() &&
			(GetCurrentCharacter() == nullptr || GetCurrentCharacter()->usedSkill || !IsPlayerTurn()))
		{
			EndTurn();
		}
		return;
	}

	currentAction = actionQueue[0];
	actionQueue.RemoveAt(0, 1);
	/*while (currentAction == nullptr && actionQueue.Num() > 0)
	{
		currentAction = actionQueue[0];
		actionQueue.RemoveAt(0, 1);
	}*/

	if (currentAction)
	{
		if (!IsPlayingAnim())
			UE_LOG(LogTemp, Warning, TEXT("ACTION || %s has action %i"), *currentTurnCharacter->charName, currentAction->actionType);
	}

	if (_battleEnded && actionQueue.Num() == 0 && GetEnemyGroup()->id == "vs_tut_emissary")
	{
		UAction* action = NewObject<UAction>();
		action->InitializeAction(ACTION_END_BATTLE, nullptr);
		actionQueue.Add(action);
	}

	if (currentAction == nullptr)
	{
		return;
	}

	if (currentAction->gameCharacter != nullptr)
	{
		currentAvatar = currentAction->gameCharacter->avatar;
	}

	switch (currentAction->actionType)
	{
	case ACTION_END_BATTLE:
	{
		/*TooltipController.destroyTooltip();
		if (Music.current)
		{
			Music.current.fadeOut(1500);
		}*/
		battleResult = "script";
		defeatedGroup = GetEnemyGroup();
		ExitBattle();
		break;
	}
	case ACTION_MOVE:
	{
		directionArrow->SetActorHiddenInGame(true);//DHK

		RequestMoveTo(hexes[currentAction->moveToHexIndex]);
		break;
	}
	case ACTION_ENTER:
	{
		moveRoute.Empty(); moveRoute.Add(currentAvatar->hex);
		currentAvatar->movedY = 0;
		currentAvatar->AnimateAvatar(ANIMATION_WALK);
		break;
	}
	case ACTION_RETREAT:
	{
		UE_LOG(LogTemp, Warning, TEXT("processing retreat action on %s"), *currentAction->gameCharacter->charName);
		ADragonAgeJourneysCharacter* loc1 = currentAction->gameCharacter->avatar;
		moveRoute.Empty(); moveRoute.Add(grid[0].arrHex[loc1->battlefieldY]);
		loc1->movedY = 0;
		loc1->AnimateAvatar(ANIMATION_WALK);
		break;
	}
	case ACTION_BARK_WAIT:
	{
		ShowBark(currentAction->stringArg, currentAction->gameCharacter);
		waitFrames = DURATION_FRAMES + 5;
		break;
	}
	case ACTION_WAIT:
	{
		waitFrames = currentAction->intArg;
		break;
	}
	case ACTION_PASS:
	{
		currentAvatar->AddFloatingText("Waiting...", ANNOUNCE_TEXT_COLOR);
		currentAction->complete = true;
		EndTurn();
		break;
	}
	case ACTION_SKILL_ANIMATION:
	{
		directionArrow->SetActorHiddenInGame(true);//DHK
		SetAvatarRotation(currentAvatar, hexes[currentAction->targetHexIndex]);
		BeginSkillAnimation(currentAction);
		break;
	}
	case ACTION_SKILL_RESULT:
	{
		RequestSkillOn(hexes[currentAction->targetHexIndex], currentAction->useSkill);
		break;
	}
	case ACTION_SKILL_WAIT_FOR_ANIMATION:
	{
		if (IsPlayingAnim())
		{
			UAction* _tempAction = NewObject<UAction>();
			_tempAction->InitializeAction(ACTION_SKILL_WAIT_FOR_ANIMATION, GetCurrentCharacter());
			actionQueue.Add(_tempAction);
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

void UBattlefield::EndMove()
{
	UE_LOG(LogTemp, Warning, TEXT("Battle endMove"));

	ADragonAgeJourneysCharacter* gameChar = currentAction->gameCharacter->avatar;
	AHex* moveToHex = hexes[currentAction->moveToHexIndex];

	if (currentAction->gameCharacter->IsLargeCreature())
	{
		SetLargeCreatureHexes(currentAction->gameCharacter);
	}

	movementMap = BuildAdjacencyMap();

	dSearch = NewObject<UDijkstra>();
	dSearch->InitializeDijkstra(movementMap, GetCurrentHex()->index);
	dSearch->FindShortestPath();

	if (gameChar->GetCharRef() && gameChar->GetCharRef()->health > 0)
	{
		SetAvatarSpeed(gameChar, 0);
		gameChar->AnimateAvatar(ANIMATION_IDLE);
	}

	//TODO gameChar->EndWalkSound();
	TestAuras();

	if (actionQueue.Num() > 0)
	{
		return;
	}

	if (IsPlayerTurn())
	{
		AddBars(true);
	}
	else
	{
		EndTurn();
	}
}

void UBattlefield::EndTurn()
{
	//TODO check visual stuff
	UE_LOG(LogTemp, Warning, TEXT("endTurn"));
	//turnHint.visible = false;
	//endTurnNextFrame = false;
	//endTurnPending = false;
	//ClearActionText();
	//RemoveActionIndicator();

	if (actionQueue.Num() > 0)
	{
		return;
	}

	ResetHexes();
	UpdateHexes();
	//GetMain()->RemoveNavPages();
	//RemovePortraitSkillTargets();

	for (int32 i = 0; i < groups.Num(); i++)
	{
		UGroup* group = groups[i];
		if (group->IsDefeated())
		{
			UE_LOG(LogTemp, Warning, TEXT("=================================="));
			UE_LOG(LogTemp, Warning, TEXT("group defeated - battle over!"));
			defeatedGroup = group;

			if (!group->player)
			{
				battleResult = "victory";
			}
			else
			{
				battleResult = "defeat";
			}

			//frameCount = 0;
			endBattle = true;

			UE_LOG(LogTemp, Warning, TEXT("%s"), *battleResult);

			return;
		}
	}

	bool bDizzy = false;
	if (currentTurnCharacter != nullptr && GetCurrentCharacter() == currentTurnCharacter)
	{
		ADragonAgeJourneysCharacter* currentGameAvatar = GetCurrentCharacter()->avatar;
		int32 nIndex = GetCurrentCharacter()->effects.Num() - 1;
		while (nIndex >= 0)
		{
			UEffect* effect = GetCurrentCharacter()->effects[nIndex];
			if (effect->caster != GetCurrentCharacter())
			{
				effect->createdThisTurn = false;
			}

			if (!bDizzy && effect->randomFacing)
			{
				if (FMath::RandRange(1, 100) > 30 && currentGameAvatar)
				{
					//TODO rotation if needed currentGameAvatar->SetFacingLeft(!currentGameAvatar->facingLeft);
					currentGameAvatar->AddFloatingText("Dizzy!", ANNOUNCE_TEXT_COLOR);
					bDizzy = true;
				}
			}

			if (effect->duration == EFFECT_DURATION_NEXT_END && !effect->createdThisTurn)
			{
				GetCurrentCharacter()->RemoveEffectWithIndex(nIndex);
			}
			else if (effect->duration <= 0)
			{
				GetCurrentCharacter()->RemoveEffectWithIndex(nIndex);
			}
			else if (effect->duration > 0 && effect->duration < 1000)
			{
				effect->duration--;
			}

			effect->createdThisTurn = false;

			--nIndex;
		}
	}

	NextTurn();
}

void UBattlefield::NextTurn()
{
	//TODO check visual stuff
	UE_LOG(LogTemp, Warning, TEXT("=========== nextTurn ================="));

	if (suspendTurnOrder)
	{
		return;
	}

	currentTurnCharacter = initStack->PopStack();
	//UE_LOG(LogTemp, Warning, TEXT("%s is current"), *currentTurnCharacter->charName);

	int32 loc2 = currentTurnCharacter->effects.Num() - 1;

	while (loc2 >= 0)
	{
		UEffect* loc6 = currentTurnCharacter->effects[loc2];
		if (loc6 && loc6->type == EFFECT_TYPE_PERIODIC)
		{
			if (!currentTurnCharacter->IsInactive())
			{
				if (loc6->healthDelta < 0)
				{
					//currentTurnCharacter->avatar->ShowBlood = false;
					int32 loc7 = -loc6->healthDelta;
					currentTurnCharacter->TakeDamage(loc7, nullptr);
					currentTurnCharacter->avatar->AddFloatingText(GetEffectName(loc6->effectId));
				}
			}
		}

		if (loc6 && loc6->duration == EFFECT_DURATION_NEXT_START)
		{
			currentTurnCharacter->RemoveEffectWithIndex(loc2);
		}

		--loc2;
	}

	for (int32 i = 0; i < currentTurnCharacter->GetSkills().Num(); i++)
	{
		USkill* loc3 = currentTurnCharacter->GetSkills()[i];
		if (loc3->currentCooldown > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s has skill %s with cooldown %i"), *currentTurnCharacter->charName, *loc3->skillname, loc3->currentCooldown);
			loc3->currentCooldown = FMath::Max(0, loc3->currentCooldown - 1);
			UE_LOG(LogTemp, Warning, TEXT("new value: %i"), loc3->currentCooldown);
		}
	}

	currentTurnCharacter->usedSkill = false;
	//currentTurnCharacter = currentTurnCharacter;
	currentAvatar = currentTurnCharacter->avatar;
	currentAvatar->GetCharRef()->moved = 0;
	currentTurnStartHex = currentAvatar->hex;

	//initDisplay.update();

	if (currentTurnCharacter->IsInactive())
	{
		UE_LOG(LogTemp, Warning, TEXT("character %s is inactive, skipping turn"), *currentTurnCharacter->charName);

		EndTurn();
		return;
	}

	/*if (indicator != nullptr)
	{
	setChildIndex(indicator, getChildIndex(hexLayer) + 1);
	}
	else
	{
	UE_LOG(LogTemp, Warning, TEXT("adding turn indicator"));
	indicator = new packages.battle.AvatarIndicator();
	addChildAt(indicator, getChildIndex(hexLayer) + 1);
	indicator.x = GetCurrentHex()->x;
	indicator.y = GetCurrentHex()->y;
	}
	indicator.setTarget(currentAvatar);*/

	/*for(loc4 in portraits)
	{
	auto loc4 = portraits[i];
	if (loc4->charRef == currentTurnCharacter)
	{
	loc4->setHighlight(true);
	continue;
	}
	loc4->setHighlight(false);
	}*/

	int32 loc5 = currentTurnCharacter->IncreaseHealth(currentTurnCharacter->GetHealthRegeneration());
	if (loc5 > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s regenerates %i health"), *currentTurnCharacter->charName, currentTurnCharacter->GetHealthRegeneration());
		currentTurnCharacter->avatar->AddFloatingHealing(loc5);
		//currentTurnCharacter->portrait.updateHealth();
	}
	if (currentTurnCharacter->IncreasePower(currentTurnCharacter->GetPowerRegeneration()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s regenerates %i power"), *currentTurnCharacter->charName, currentTurnCharacter->GetPowerRegeneration());
		//currentTurnCharacter->portrait.updatePower();
	}

	ResetHexes();

	if (currentTurnCharacter->IsStunned())
	{
		currentAvatar->AddFloatingText("Stunned!");
		//packages.audio.Sounds.play("skip_stunned", 1);
		EndTurn();
		return;
	}
	if (currentTurnCharacter->IsDisoriented())
	{
		currentAvatar->AddFloatingText("Confused!");
		//packages.audio.Sounds.play("skip_stunned", 1);
		EndTurn();
		return;
	}

	for (int32 i = 0; i < currentTurnCharacter->GetSkills().Num(); i++)
	{
		USkill* loc3 = currentTurnCharacter->GetSkills()[i];
		int32 loc8 = 0;
		if (loc3->IsUnlocked() && loc3->type == SKILL_TYPE_TOGGLED && loc3->toggledOn)
		{
			loc8 = SubtractPowerForSkill(loc3);
		}

		if (currentTurnCharacter->power <= 0 && loc8 < loc3->powerCost)
		{
			ToggleOffSkill(loc3);
		}
	}

	for (int32 i = 0; i < currentTurnCharacter->GetSkills().Num(); i++)
	{
		USkill* loc3 = currentTurnCharacter->GetSkills()[i];
		/*if (loc3->IsUnlocked() && loc3->type == SKILL_TYPE_TOGGLED && loc3->toggledOn)
		{
		}*/
		if (currentTurnCharacter->power < loc3->powerCost)
		{
			ToggleOffSkill(loc3);
		}
	}

	movementMap = BuildAdjacencyMap();
	dSearch = NewObject<UDijkstra>();
	dSearch->InitializeDijkstra(movementMap, GetCurrentHex()->index);
	dSearch->FindShortestPath();
	currentTurnMoveSearch = dSearch;

	AddBars(true);
	if (currentTurnCharacter->CanCounterattack())
	{
		currentTurnCharacter->SetCounterAttackReady(true);
	}
	else
	{
		currentTurnCharacter->SetCounterAttackReady(false);
	}

	if (currentTurnCharacter->group->player && currentTurnCharacter->ai == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("it's player %s turn"), *currentTurnCharacter->charName);//DHK

		//temp, not needed anymore
		/*USkill* _sk = NewObject<USkill>();
		//_sk->InitializeSkill(SKILL_MOVE, currentTurnCharacter);
		_sk->InitializeSkill(SKILL_STRIKE, currentTurnCharacter);
		HighlightSkillTargets(_sk);*/

		//TODO
		//if range/mage, setmode range, if melee/moves only setmode move, etc...

		if (currentTurnCharacter->GetHealthPercent() <= LOW_HEALTH_PERCENT)
		{
			//packages.audio.LowHealthSounds.current.playLowHealthSounds(currentTurnCharacter->gender);
		}
	}
	else
	{
		UpdateHexes();
		//turnHint.visible = false;
		//removeBars();

		UE_LOG(LogTemp, Warning, TEXT("it's AI %s turn"), *currentTurnCharacter->charName);//DHK

		AiTurn();
	}
}

void UBattlefield::AiTurn()
{
	UGameCharacter* loc1 = GetCurrentCharacter();
	loc1->ai->target = nullptr;

	if (loc1->IsBerserk())
	{
		loc1->ai->stance = STANCE_AGGRESSIVE;
	}

	//loc1->ai->charRef = loc1;
	loc1->ai->possibleActions.Empty();
	TArray<UGameCharacter*> arHate;

	for (int32 i = 0; i < loc1->effects.Num(); i++)
	{
		UEffect* loc4 = loc1->effects[i];
		if (loc4->mustTargetCaster && loc4->caster)
		{
			UE_LOG(LogTemp, Warning, TEXT("due to effect %s, target is assigned as %s"), *GetEffectName(loc4->effectId), *loc4->caster->charName);
			arHate.Empty(); arHate.Add(loc4->caster);/*loc2 = [loc4->caster];*/
			break;
		}
	}

	if (arHate.Num() == 0)
	{
		for (int32 i = 0; i < avatars.Num(); i++)
		{
			ADragonAgeJourneysCharacter* loc5 = avatars[i];
			UGameCharacter* loc8 = loc5->GetCharRef();
			bool loc9 = false;

			if (AreAllies(loc1, loc8))
			{
				loc9 = true;
			}

			if (loc8->IsInactive())
			{
				loc9 = true;
			}

			if (loc8->IsHidden())
			{
				loc9 = true;
			}

			if (!loc9)
			{
				for (int32 i = 0; i < loc8->effects.Num(); i++)
				{
					UEffect* loc10 = loc8->effects[i];
					if (loc10->effectId == EFFECT_FEIGN_DEATH)
					{
						loc9 = true;
					}
				}
			}

			if (loc9)
			{
				continue;
			}

			loc8->latestHate = loc1->ai->GetHateFor(loc8);

			arHate.Add(loc8);
		}

		UE_LOG(LogTemp, Warning, TEXT("aiTurn() - %i targets found"), arHate.Num());
		arHate.Sort(LatestHateSorter);//loc2 = loc2.sortOn("latestHate", Array.NUMERIC | Array.DESCENDING);
		Algo::Reverse(arHate);

		if (arHate.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("can't find any good targets"));
			PassTurn();
			return;
		}

		int32 loc6 = 999;
		int32 loc7 = 0;
		while (loc7 < arHate.Num())
		{
			UGameCharacter* loc11 = arHate[loc7];
			loc11->hateRanking = loc7 + 1;
			loc1->ai->MoveActionsTowards(loc11->avatar->hex);

			//for(loc12 in loc1->skills)
			for (int32 i = 0; i < loc1->GetSkills().Num(); i++)
			{
				USkill* loc12 = loc1->GetSkills()[i];
				if (GetCurrentCharacter()->group->id == "vs_tut_genlock" && loc12->skillId != SKILL_STRIKE)
				{
					UE_LOG(LogTemp, Warning, TEXT("AiTurn vs_tut_genlock"));
					continue;
				}

				if (loc12->hitsAllEnemies || loc12->hitsAllFriendlies || loc12->spawnsTrap || loc12->targetsSelf)
				{
					loc1->ai->SkillActionsUntargeted(loc12);
					continue;
				}

				loc1->ai->SkillActionsForTarget(loc11, loc12);
			}
			++loc7;
		}
	}

	if (loc1->ai->possibleActions.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO POSSIBLE ACTIONS!"));
		PassTurn();
		return;
	}

	UAiAction* loc3 = loc1->ai->SelectAction();
	AddActionsForAI(loc3);
}

void UBattlefield::PassTurn()
{
	//TODO check visual
	UE_LOG(LogTemp, Warning, TEXT("PassTurn - "), *GetCurrentCharacter()->charName);

	ResetHexes(/*false*/);
	//RemoveBars();
	//ClearMovementPath();
	//RemoveActionIndicator();

	/*for(int32 i=0;i<characters.Num();i++)
	{
		UGameCharacter* loc1 = characters[i];
		if (loc1 != nullptr)
		{
			loc1->MouseOut(nullptr);
		}
	}*/

	UAction* loc2 = NewObject<UAction>();
	loc2->InitializeAction(ACTION_PASS, GetCurrentCharacter());
	actionQueue.Add(loc2);
}

void UBattlefield::TestAuras()
{
	for (int32 i = 0; i < allies.Num(); i++)
	{
		UGameCharacter* ally = allies[i];
		USkill* skill = ally->GetSkill(SKILL_SHIELD_COVER);
		if (skill != nullptr && ally->HasShield())
		{
			AddAuraToFriendlies(skill, ally);
		}
		if (ally->classId == CLASS_CHAMPION)
		{
			skill = ally->GetSkill(SKILL_RALLY);
		}
		if (skill != nullptr)
		{
			AddAuraToFriendlies(skill, ally);
		}
	}
}

void UBattlefield::ResetHexes()
{
	//UE_LOG(LogTemp, Warning, TEXT("resetHexes"));
	for (int32 i = 0; i < hexes.Num(); i++)
	{
		AHex* hex = hexes[i];
		hex->SetMode(HEX_MODE_NORMAL, -1);
		hex->RemoveDirection();
	}
}

void UBattlefield::UpdateHexes()
{
	for (int32 i = 0; i < hexes.Num(); i++)
	{
		AHex* _hex = hexes[i];
		_hex->UpdateHex();
	}
}

void UBattlefield::DoEndBattle()
{
	//TODO DoEndBattle
}

void UBattlefield::ExitBattle()
{
	//TODO ExitBattle
}

void UBattlefield::AddActionsForAI(UAiAction* aiAction)
{
	if (aiAction->skill && aiAction->skill->skillId == SKILL_EMISSARY_ESCAPE)
	{
		UAction* actionWait = NewObject<UAction>();
		actionWait->InitializeAction(ACTION_BARK_WAIT, aiAction->owner);
		actionWait->stringArg = "bark_emissary_mine_escape";
		actionQueue.Add(actionWait);
	}

	if (aiAction->moveToHex != nullptr)
	{
		UAction* actionMove = NewObject<UAction>();
		actionMove->InitializeAction(ACTION_MOVE, aiAction->owner);
		actionMove->moveToHexIndex = aiAction->moveToHex->index;
		actionQueue.Add(actionMove);
	}

	if (aiAction->targetHex != nullptr && aiAction->skill != nullptr)
	{
		AddSkillActions(aiAction->skill, aiAction->owner, aiAction->targetHex->index, -1, aiAction);
	}
}

void UBattlefield::AddSkillActions(USkill* skill, UGameCharacter* gameCharacter, int32 nIndex, int32 nRepeat /*= -1*/, UAiAction* aiAction /*= nullptr*/, bool bCounterAttack /*= false*/)
{
	UAction* loc1 = nullptr;
	AHex* loc2 = hexes[nIndex];
	auto loc3 = DoesSkillMiss(gameCharacter, skill, nIndex);

	if (skill->characterAnimation != -1)
	{
		loc1 = NewObject<UAction>();
		loc1->InitializeAction(ACTION_SKILL_ANIMATION, gameCharacter);
		loc1->targetHexIndex = nIndex;
		loc1->useSkill = skill;
		loc1->skillMiss = loc3;

		if (skill->specialMovementSkill)
		{
			if (aiAction == nullptr || aiAction->specialHex == nullptr)
			{
				PassTurn();
				return;
			}

			loc1->intArg = aiAction->specialHex->index;
		}

		actionQueue.Add(loc1);
	}

	loc1 = NewObject<UAction>();
	loc1->InitializeAction(ACTION_SKILL_RESULT, gameCharacter);
	loc1->targetHexIndex = nIndex;
	loc1->useSkill = skill;
	loc1->skillMiss = loc3;
	loc1->counterattack = bCounterAttack;
	actionQueue.Add(loc1);

	if (skill->characterAnimation != -1)
	{
		loc1 = NewObject<UAction>();
		loc1->InitializeAction(ACTION_SKILL_WAIT_FOR_ANIMATION, gameCharacter);
		loc1->targetHexIndex = nIndex;
		loc1->useSkill = skill;
		loc1->skillMiss = loc3;
		actionQueue.Add(loc1);
	}
}

void UBattlefield::AddAuraToFriendlies(USkill* skill, UGameCharacter* caster)
{
	int32 nRadius = 0;

	for (int32 i = 0; i < allies.Num(); i++)
	{
		UGameCharacter* target = allies[i];
		if (skill->type == SKILL_TYPE_TOGGLED && !skill->toggledOn)
		{
			if (target->RemoveEffectWithId(skill->auraEffectId))
			{
				target->avatar->AddFloatingText("-" + GetEffectName(skill->auraEffectId), 16777215);
			}
			continue;
		}

		if (target->IsInactive())
		{
			continue;
		}

		if (target->avatar == nullptr)
		{
			continue;
		}

		if (target == caster)
		{
			if (skill->skillId == SKILL_RALLY)
			{
				continue;
			}
			if (skill->skillId == SKILL_SHIELD_COVER)
			{
				continue;
			}
		}

		nRadius = GetRawDistanceBetweenHexes(target->avatar->hex, caster->avatar->hex);
		if (nRadius <= skill->auraRadius)
		{
			UEffect* effect = NewObject<UEffect>();
			effect->InitializeEffect(skill->auraEffectId, caster, target, caster->level);
			target->AddEffect(effect);
			continue;
		}

		if (!target->RemoveEffectWithId(skill->auraEffectId))
		{
			continue;
		}

		target->avatar->AddFloatingText("-" + GetEffectName(skill->auraEffectId), 16777215);
	}
}

void UBattlefield::BeginSkillAnimation(UAction* action)
{
	if (IsPlayerTurn())
	{
		ResetHexes();
		UpdateHexes();
		//RemoveBars();
	}

	waitingForActionFrame = 0;

	if (action->useSkill->characterAnimation == -1)
	{
		action->complete = true;
		return;
	}

	ADragonAgeJourneysCharacter* loc1 = action->gameCharacter->avatar;
	AHex* loc2 = loc1->hex;
	AHex* loc3 = hexes[action->targetHexIndex];
	USkill* loc4 = action->useSkill;

	if (loc2 != nullptr)
	{
		if (loc2->GetActorLocation().X != loc3->GetActorLocation().X)
		{
			if (loc2->GetActorLocation().X < loc3->GetActorLocation().X)
			{
				loc1->SetFacingLeft(false);
			}
			else
			{
				loc1->SetFacingLeft(true);
			}
		}
		else if (action->gameCharacter->IsMonster())
		{
			loc1->SetFacingLeft(true);
		}
		else
		{
			loc1->SetFacingLeft(false);
		}
	}

	if (loc3->avatar != nullptr && loc3 != loc2 && !loc4->suppressTargetFacing)
	{
		if (loc2 && !AreAllies(loc3->avatar->GetCharRef(), action->gameCharacter))
		{
			loc3->avatar->FaceAttacker(loc1);
		}
	}

	int32 loc5 = 0;
	if (loc4->accelerateAnimation)
	{
		loc5 = 1;
	}

	loc1->AnimateAvatar(loc4->characterAnimation);//TODO , loc4, false, loc5);

	/*if (!loc4.specialMovementSkill)
	{
	};*/

	//TODO Sounds
	/*if (!(currentAction == nullptr) && !(loc4.sound_begin == ""))
	{
		packages.audio.Sounds.play(loc4.sound_begin);
	}
	else if (!(currentAction == nullptr) && !currentAction->skillMiss && loc4.type == SKILL_TYPE_PASSIVE)
	{
		packages.audio.Sounds.play("buff");
	}
	else if (loc4.skillId == SKILL_STRIKE)
	{
		if (param1.gameCharacter->classId == CLASS_WARRIOR)
		{
			Sounds.play("swing_edged");
		}
		else if (param1.gameCharacter->classId == CLASS_MAGE)
		{
			Sounds.play("swing_blunt");
		}
		else if (param1.gameCharacter->classId == CLASS_HURLOCK)
		{
			Sounds.play("swing_edged");
		}
		else
		{
			Sounds.play("swing_creature");
		}
	}*/

}

void UBattlefield::RequestSkillOn(AHex* hex, USkill* skill)
{
	UE_LOG(LogTemp, Warning, TEXT("using skill %s on %s"), *skill->skillname, *hex->StringData());

	SetAvatarRotation(currentAvatar, hex);

	if (skill->type != SKILL_TYPE_TOGGLED)
	{
		SubtractPowerForSkill(skill);
	}

	if (skill->untogglesSongs)
	{
		USkill* loc1 = GetCurrentCharacter()->GetSkill(SKILL_SONG_OF_COURAGE);
		if (loc1 && loc1 != skill)
		{
			loc1->toggledOn = false;
		}

		loc1 = GetCurrentCharacter()->GetSkill(SKILL_SONG_OF_VALOR);
		if (loc1 && loc1 != skill)
		{
			loc1->toggledOn = false;
		}

		for (int32 i = 0; i < allies.Num(); i++)
		{
			UGameCharacter* loc2 = allies[i];
			loc2->RemoveEffectWithId(EFFECT_SONG_COURAGE);
			loc2->RemoveEffectWithId(EFFECT_SONG_VALOR);
		}
	}

	if (skill->type == SKILL_TYPE_TOGGLED && skill->toggleEndsTurn == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("toggled skill which does not end turn - not disabling quickbar"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("requestSkillOn: resetting hexes"));
		GetCurrentCharacter()->usedSkill = true;
		//RemoveBars();
		ResetHexes();
	}

	if (skill->type != SKILL_TYPE_TARGETED || !skill->toggleEndsTurn)
	{
		if (skill->enablesOwnCounterAttack && GetCurrentCharacter()->CanCounterattack())
		{
			GetCurrentCharacter()->SetCounterAttackReady(true);
		}
		else
		{
			GetCurrentCharacter()->SetCounterAttackReady(false);
		}
	}

	if (skill->hasCasterAnimation)
	{
		if (skill->type != SKILL_TYPE_TOGGLED || !skill->toggledOn)
		{
			/*loc3 = skill->GetCasterAnimation();
			if (loc3)
			{
				AddAnimationToAvatar(loc3, currentAvatar, skill);
			}*/
		}
	}

	//TODO Traps
	/*if (skill->spawnsTrap)
	{
		loc4 = new packages.battle.Trap();
		loc4.owner = GetCurrentCharacter();
		loc4.hex = hex;
		loc4.level = GetCurrentCharacter()->level;
		loc4.active = true;
		traps.Add(loc4);
		loc4.clip = new gw_trap1();
		loc4.clip.mouseEnabled = false;
		loc4.clip.x = hex->x;
		loc4.clip.y = hex->y;
		hexLayer.addChild(loc4.clip);
	}*/

	//TODO Projectile
	/*if (skill->spawnsProjectile)
	{
		loc5 = skill->GetProjectileClip();
		if (loc5 != nullptr)
		{
			loc5.startHex = GetCurrentHex();
			loc5.targetHex = hex;
			loc5.init(skill->char_ref, skill, hex);
			projectiles.Add(loc5);
			avatarLayer.addChildAt(loc5.clip, avatarLayer->GetChildIndex(currentAvatar) + 1);
			UE_LOG(LogTemp, Warning, TEXT("projectile [" + String(loc5) + "] created and added to stage at " + loc5.clip.x + ", " + loc5.clip.y);
			return;
		}
	}*/

	//TODO Beam
	/*if (skill->spawnsBeam)
	{
		loc6 = skill->GetBeam();
		if (loc6 != nullptr)
		{
			loc6.startHex = GetCurrentHex();
			loc6.targetHex = hex;
			loc6.init(skill->char_ref, skill);
			projectiles.Add(loc6);
			avatarLayer.addChild(loc6.clip);
			Depth.check(loc6.clip, loc6.startHex.y);
		}
	}*/

	//TODO impact vfx
	/*if (skill->targetPointAnimation)
	{
		loc7 = skill->GetImpactClip();
		if (loc7 != nullptr)
		{
			loc7.x = hex->x;
			loc7.y = hex->y + 1;
			avatarLayer.addChild(loc7);
			vfx.Add(loc7);
			Depth.check(loc7);
		}
	}*/

	DoSkillEffect(hex, skill);
}

//TODO DoSkillEffect
void UBattlefield::DoSkillEffect(AHex* hex, USkill* skill)
{
	UE_LOG(LogTemp, Warning, TEXT("DoSkillEffect %s %s"), *hex->GetName(), *skill->skillname);

	UGameCharacter* loc1 = currentAction->gameCharacter;
	AHex* loc3 = loc1->avatar->hex;
	TArray<UGameCharacter*> loc4;
	UGameCharacter* loc5 = nullptr;

	if (hex->GetAnAvatar() == nullptr)
	{
		if (skill->targetsDead)
		{
			for (int32 i = 0; i < allies.Num(); i++)
			{
				UGameCharacter* loc5 = allies[i];
				if (loc5->avatar->deathHex == hex)
				{
					loc4.Add(loc5);
					break;
				}

			}
		}
	}
	else
	{
		loc4.Add(hex->GetAnAvatar()->GetCharRef());
	}

	if (skill->skillId == SKILL_EMISSARY_ESCAPE)
	{
		//currentAvatar.parent.removeChild(currentAvatar);
		//fx.blindingFlash();
		currentAction->complete = true;
		GetEnemyGroup()->overrideDefeated = true;
		return;
	}

	if (skill->pointBlankAE)
	{
		TArray<UGameCharacter*> loc13;

		for (int32 i = 0; i < enemies.Num(); i++)
		{
			loc13.Add(enemies[i]);
		}

		loc13.Append(allies);//loc13 = enemies.slice().concat(allies);

		UGameCharacter* loc14 = nullptr;
		int32 loc15 = 0;

		if (skill->targetsFriendlies || skill->friendlyFire)
		{
			for (int32 i = 0; i < allies.Num(); i++)
			{
				loc14 = allies[i];

				if (!loc14->avatar || !loc14->avatar->hex)
				{
					continue;
				}

				loc15 = rawDistances[GetCurrentHex()->index].arrInt32[loc14->avatar->hex->index];

				if (loc15 <= skill->GetRange())
				{
					loc4.Add(loc14);
				}
			}
		}

		if (!skill->targetsFriendlies)
		{
			for (int32 i = 0; i < enemies.Num(); i++)
			{
				loc14 = enemies[i];

				if (!loc14->avatar || !loc14->avatar->hex)
				{
					continue;
				}

				if (!loc14->health)
				{
					continue;
				}

				loc15 = rawDistances[GetCurrentHex()->index].arrInt32[loc14->avatar->hex->index];

				if (loc15 <= skill->GetRange())
				{
					loc4.Add(loc14);
				}
			}
		}
	}
	else if (skill->areaEffectRadius > 0)
	{
		for (int32 i = 0; i < hex->neighbours.Num(); i++)
		{
			AHex* loc16 = hex->neighbours[i];

			if (loc16 != nullptr && loc16->GetAnAvatar() != nullptr)
			{
				loc4.Add(loc16->GetAnAvatar()->GetCharRef());
			}
		}

		loc3 = hex;
	}

	if (skill->targetsTemplate)
	{
		TArray<AHex*> loc17 = GetHexesUnderTemplate(hex, skill);
		for (int32 i = 0; i < loc17.Num(); i++)
		{
			AHex* loc7 = loc17[i];

			if (loc7->avatar)
			{
				loc4.Add(loc7->avatar->GetCharRef());
				continue;
			}

			if (loc7->largeAvatar)
			{
				loc4.Add(loc7->largeAvatar->GetCharRef());
			}
		}
	}

	if (skill->arcRadius > 0)
	{
		for (int32 i = 0; i < hex->neighbours.Num(); i++)
		{
			AHex* loc18 = hex->neighbours[i];

			if (loc18 == nullptr || loc18 == GetCurrentHex())
			{
				continue;
			}

			for (int32 i = 0; i < GetCurrentHex()->neighbours.Num(); i++)
			{
				AHex* loc19 = GetCurrentHex()->neighbours[i];

				if (loc19 == nullptr || loc19 == hex)
				{
					continue;
				}

				if (loc19 != loc18)
				{
					continue;
				}

				if (loc19->GetAnAvatar() == nullptr)
				{
					continue;
				}

				loc4.Add(loc19->GetAnAvatar()->GetCharRef());
			}
		}
	}

	if (skill->hitsAllFriendlies)
	{
		loc4.Empty();

		for (int32 i = 0; i < allies.Num(); i++)
		{
			loc5 = allies[i];

			if (skill->doesNotEffectMages && loc5->IsMage())
			{
				continue;
			}

			loc4.Add(loc5);
		}
	}

	if (skill->hitsAllEnemies)
	{
		for (int32 i = 0; i < enemies.Num(); i++)
		{
			loc5 = enemies[i];

			loc4.Add(loc5);
		}
	}

	for (int32 i = 0; i < loc4.Num(); i++)
	{
		UGameCharacter* loc10 = loc4[i];
		int32 loc20 = 0;
		int32 loc21 = loc4.Num() - 1;

		while (loc21 >= 0)
		{
			if (loc4[loc21] == loc10)
			{
				++loc20;

				if (loc20 > 1)
				{
					loc4.RemoveAt(loc21, 1);
				}
			}

			--loc21;
		}
	}

	if (skill->selfEffectId)
	{
		UEffect* _effect = NewObject<UEffect>();
		_effect->InitializeEffect(skill->selfEffectId, loc1, loc1);

		if (skill->type != SKILL_TYPE_TOGGLED)
		{
			loc1->AddEffect(_effect);
		}
		else if (!skill->toggledOn)
		{
			loc1->AddEffect(_effect);
			//TODO animation toggle off
			/*AddAnimationToAvatar(new packages.battle.CasterAnim(new generic_toggle_on()), currentAvatar, skill);
			packages.audio.Sounds.play("toggle_on");*/
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Battlefield.doSkillEffect() - %s on %i targets."), *skill->skillname, loc4.Num());

	if (skill->skillId == SKILL_CONSUMABLE)
	{
		if (!skill->consumable->consumeSound.IsEmpty())
		{
			//packages.audio.Sounds.play(skill->consumable->consumeSound);
		}
		currentAction->gameCharacter->avatar->AddFloatingText("Item: " + skill->consumable->itemName);
		skill->consumable->stack--;

		if (skill->consumable->stack <= 0)
		{
			skill->shortcutSlot = -1;
			currentAction->gameCharacter->party->RemoveFromInventory(skill->consumable);
		}
	}

	UGameCharacter* loc11 = nullptr;

	for (int32 i = 0; i < loc4.Num(); i++)
	{
		loc11 = loc4[i];
		ADragonAgeJourneysCharacter* loc22 = loc11->avatar;
		//AHex* loc23 = loc22->hex;

		if (loc11->health <= 0 && !(skill->targetsDead || skill->resurrects))
		{
			continue;
		}

		if (!skill->friendlyFire && AreAllies(GetCurrentCharacter(), loc11))
		{
			continue;
		}

		bool loc24 = false;

		if (currentAction->skillMiss)
		{
			if (currentAction->gameCharacter->ai == nullptr && skill->GetDamage() > 0)
			{
				loc24 = true;
				loc22->AddFloatingText("Glancing Hit!");
				if (currentAction->useSkill->sound_miss == "")
				{
					//packages.audio.Sounds.play("impact_generic");
				}
				else
				{
					//packages.audio.Sounds.play(currentAction->useSkill->sound_miss);
				}
			}

			if (skill->GetDamage() <= 0 || !loc24)
			{
				loc22->AddFloatingText("Miss!");

				if (currentAction->useSkill->sound_miss == "")
				{
					//packages.audio.Sounds.play("miss_generic");
				}
				else
				{
					//packages.audio.Sounds.play(currentAction->useSkill->sound_miss);
				}

				continue;
			}
		}

		if (skill->targetEffectId && !loc24)
		{
			UEffect* _ef = NewObject<UEffect>();
			_ef->InitializeEffect(skill->targetEffectId, loc1, loc11);

			if (skill->type == SKILL_TYPE_TOGGLED)
			{
				if (skill->toggledOn)
				{
					loc11->RemoveEffectWithId(skill->targetEffectId);
				}
				else
				{
					loc11->AddEffect(_ef);
				}
			}
			else
			{
				loc11->AddEffect(_ef);
			}
		}

		if (skill->knockback && !loc24)
		{
			UE_LOG(LogTemp, Warning, TEXT("calculating knockback on attack %s used on %s"), *skill->skillname, *loc11->charName);

			TArray<AHex*> loc32;
			if (loc11->IsLargeCreature())
			{
				for (int32 i = 0; i < loc11->avatar->largeHexes.Num(); i++)
				{
					AHex* loc34 = loc11->avatar->largeHexes[i];
					loc32.Add(loc34);
				}
			}
			else
			{
				loc32.Add(loc22->hex);
			}

			bool loc29 = true;
			AHex* loc30 = nullptr;

			for (int32 j = 0; j < loc32.Num(); j++)
			{
				AHex* loc33 = loc32[j];
				AHex* loc31 = nullptr;

				if (GetCurrentHex()->GetActorLocation().X > loc22->hex->GetActorLocation().X)
				{
					loc31 = loc33->neighbours[HEX_LEFT];
					UE_LOG(LogTemp, Warning, TEXT("knockback left"));
				}
				else
				{
					loc31 = loc33->neighbours[HEX_RIGHT];
					UE_LOG(LogTemp, Warning, TEXT("knockback right"));
				}

				if (loc33 == loc22->hex)
				{
					loc30 = loc31;
				}

				if (loc31 == nullptr)
				{
					loc29 = false;
					break;
				}

				if (!loc31->CanMove(loc11))
				{
					loc29 = false;
					break;
				}
			}

			if (loc29)
			{
				UE_LOG(LogTemp, Warning, TEXT("activate knockback"));
				loc22->hex->DetachAvatar();
				loc22->ClearLargeHexes();
				loc22->SetActorLocation(loc30->GetActorLocation());
				loc30->AttachAvatar(loc22);

				if (loc11->IsLargeCreature())
				{
					SetLargeCreatureHexes(loc11);
				}

				//TODO Traps
				/*for(int32 i=0;i< traps.Num();i++)
				{
					UTrap* loc35 = traps[i];
					if (loc35 && loc35.hex == loc30 && loc35->active)
					{
						UGameCharacter* loc36 = loc22->GetCharRef();
						if (loc36 != nullptr)
						{
							TrySpringTrap(loc36, loc35);
						}
					}
				}*/
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("cannot trigger knockback!"));
				if (loc30 != nullptr)
				{
					if (!loc30->CanMove(loc11))
					{
						UE_LOG(LogTemp, Warning, TEXT("-- victim is not allowed to move into push hex"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("-- hex to push back into does not exist"));
				}
			}
		}

		//TODO GetTargetAnimation
		/*if (skill->hasTargetAnimation && !(skill->type == SKILL_TYPE_TOGGLED && skill->toggledOn))
		{
			loc37 = skill->GetTargetAnimation();
			if (loc37)
			{
				AddAnimationToAvatar(loc37, loc22, skill);
			}
		}*/

		int32 loc2 = CalculateDamage(skill, loc1, loc11, loc24);

		if (loc2 > 0)
		{
			//loc22->ShowBlood = skill->drawsBlood;
			loc11->TakeDamage(loc2, loc1/*, loc3*/);

			if (currentAction->backstab)
			{
				//packages.audio.Sounds.play("backstab");
			}
			else
			{
				//packages.audio.Sounds.play(skill->sound_impact);
			}

			if (loc22->hex && !(loc22->hex == loc3) && !skill->suppressTargetFacing)
			{
				loc22->FaceHex(loc3);
			}

			if (currentAction->backstab)
			{
				loc11->avatar->AddFloatingText("Backstab!", ANNOUNCE_TEXT_COLOR);
			}

			if (currentAction->flank)
			{
				loc11->avatar->AddFloatingText("Flanked!", ANNOUNCE_TEXT_COLOR);
			}

			if (currentAction->counterattack)
			{
				loc1->avatar->AddFloatingText("Counter!", ANNOUNCE_TEXT_COLOR);
			}

			if (loc11->health > 0)
			{
				UAction* loc38 = NewObject<UAction>();
				loc38->InitializeAction(ACTION_SKILL_WAIT_FOR_ANIMATION, loc11);
				loc38->skillMiss = false;
				loc38->useSkill = skill;
				actionQueue.Add(loc38);

				for (int32 k = 0; k < loc1->passiveSkills.Num(); k++)
				{
					USkill* loc8 = loc1->passiveSkills[k];
					if (loc8->IsUnlocked() && loc8->onTargetHitSelfEffectId)
					{
						UE_LOG(LogTemp, Warning, TEXT("target hit by %s, found passive skill %s which is triggered on hit"), *loc1->charName, *loc8->skillname);

						UEffect* _effect = NewObject<UEffect>();
						_effect->InitializeEffect(loc8->onTargetHitSelfEffectId, loc1, loc1);

						loc1->AddEffect(_effect);
					}
				}
			}
			else
			{
				/*if (loc11->IsMonster())
				{
					loc9.Add(loc11);
				}*/

				UE_LOG(LogTemp, Warning, TEXT("target %s killed"), *loc11->charName);

				for (int32 k = 0; k < loc1->passiveSkills.Num(); k++)
				{
					USkill* loc8 = loc1->passiveSkills[k];
					if (loc8->IsUnlocked() && loc8->onTargetKillSelfEffectId)
					{
						UE_LOG(LogTemp, Warning, TEXT("target killed by %s, found passive skill %s which is triggered on kill"), *loc1->charName, *loc8->skillname);

						UEffect* _effect = NewObject<UEffect>();
						_effect->InitializeEffect(loc8->onTargetKillSelfEffectId, loc1, loc1);

						loc1->AddEffect(_effect);
					}
				}
			}
		}
		else if (skill->GetDamage() > 0 && loc11->GetIncomingDamageMultiplier() == 0)
		{
			loc11->avatar->AddFloatingText("Immune to Damage!", ANNOUNCE_TEXT_COLOR);
		}

		if (skill->userConvertsDamageToHealthRatio)
		{
			int32 loc39 = FMath::RoundToInt(loc2 * skill->userConvertsDamageToHealthRatio);
			int32 loc40 = loc1->IncreaseHealth(loc39);
			loc1->avatar->AddFloatingHealing(loc40);
		}

		int32 loc25 = FMath::RoundToInt(skill->GetHealing() * loc11->GetIncomingHealingMultiplier());
		bool bIsDead = loc11->health <= 0;
		int32 loc27 = loc11->IncreaseHealth(loc25, skill->resurrects);

		FString sBool = (bIsDead == true) ? "TRUE" : "FALSE";//DHK

		UE_LOG(LogTemp, Warning, TEXT("%s was dead %s healed %i"), *loc11->charName, *sBool, loc27);

		if (loc27 && bIsDead && loc22->deathHex != nullptr)
		{
			AHex* loc41 = FindReviveHexFor(loc22);
			if (loc41 == nullptr)
			{
				loc11->TakeDamage(loc27);
			}
			else
			{
				loc41->AttachAvatar(loc22);
				loc41 = nullptr;

				//TODO portrait (search for portrait)
				/*if (loc22->GetCharRef() && loc22->GetCharRef().portrait)
				{
					loc22->GetCharRef().portrait.alive();
				}*/

				loc22->deathHex = nullptr;
				loc22->AnimateAvatar(ANIMATION_IDLE);
			}
		}

		if (loc27)
		{
			loc22->AddFloatingHealing(loc27);
		}

		loc11->IncreasePower(skill->GetPowerDelta());

		UE_LOG(LogTemp, Warning, TEXT("[%s] health += %i, power += %i"), *loc11->charName, loc27, skill->GetPowerDelta());

		//loc11->portrait.update();

		if (loc1 == loc11)
		{
			continue;
		}

		CheckForCounterAttack(loc1, loc11, skill);
	}

	if (skill->type == SKILL_TYPE_TOGGLED)
	{
		if (skill->toggledOn)
		{
			ToggleOffSkill(skill);
		}
		else
		{
			skill->toggledOn = true;
		}
	}

	TestAuras();

	if (skill->skillId != SKILL_STEALTH1)
	{
		if (loc1->IsHidden())
		{
			RemoveStealthEffects(loc1);
		}
		if (loc11 && loc11->IsHidden())
		{
			RemoveStealthEffects(loc11);
		}
	}

	//TODO Bars (check for Bars)
	int32 loc12 = GetQuickBar()->GetSelectedSkillInd();
	GetQuickBar()->SetGameCharacter(GetCurrentCharacter());
	/*bars_component.effectBar.updateEffectIcons();
	loc1->portrait.update();*/

	if (loc1->ai)
	{
		skill->aiCastCount++;
	}

	currentAction->complete = true;

	if (skill->type == SKILL_TYPE_TOGGLED && !skill->toggleEndsTurn && IsPlayerTurn())
	{
		AddBars(true);
		HighlightSkillTargets();
	}

	if (skill->type == SKILL_TYPE_TOGGLED && !(loc12 == GetQuickBar()->GetSelectedSkillInd()))
	{
		GetQuickBar()->SetSelectedSkill(loc12, false);
	}
}

void UBattlefield::CheckForCounterAttack(UGameCharacter* attacker, UGameCharacter* target, USkill* skill)
{
	UE_LOG(LogTemp, Warning, TEXT("Battlefield.checkForCounterAttack: %s was hit by %s"), *target->charName, *attacker->charName);

	if (skill->cannotBeCountered)
	{
		UE_LOG(LogTemp, Warning, TEXT("skill suppresses counter attacks against it"));
		return;
	}

	if (!target->GetCounterAttackReady())
	{
		UE_LOG(LogTemp, Warning, TEXT("  can\'t counter-attack-- our counter-attack is unavailable"));
		return;
	}

	if (target->health <= 0 || target->IsInactive())
	{
		UE_LOG(LogTemp, Warning, TEXT("  we can\'t counter-attack- we\'re dead/inactive"));
		return;
	}

	if (AreAllies(attacker, target))
	{
		UE_LOG(LogTemp, Warning, TEXT("  why would we counter-attack? we\'re allies"));
		return;
	}

	if (skill->GetDamage() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("  the skill they used on us inflicts no damage. don\'t counter-attack"));
		return;
	}

	if (target->IsStunned())
	{
		UE_LOG(LogTemp, Warning, TEXT("  we can\'t counter-attack. We\'re stunned!"));
		return;
	}

	if (FMath::RandRange(0, 99) < attacker->GetDecisiveStrikePercentage())
	{
		attacker->avatar->AddFloatingText("Decisive Strike!", ANNOUNCE_TEXT_COLOR);
		return;
	}

	AHex* loc1 = attacker->avatar->hex;
	int32 loc2 = loc1->index;
	AHex* loc3 = target->avatar->hex;
	USkill* loc4 = target->GetDefaultAttack();

	if (loc4 == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR! character %s has no valid counter skill"), *target->charName);
	}

	if (loc4->GetRange() > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("  victim doesn\'t have a melee attack to respond with"));
		return;
	}

	TArray<AHex*> loc5;
	bool loc6 = false;

	if (target->IsLargeCreature())
	{
		loc5 = HexNeighboursForLargeCreature(loc3, target);
	}
	else
	{
		loc5 = HexNeighbours(loc3, false);
	}

	for (int32 i = 0; i < loc5.Num(); i++)
	{
		AHex* loc7 = loc5[i];
		if (loc7 && loc7->GetAnAvatar() == attacker->avatar)
		{
			loc6 = true;
			break;
		}
	}

	if (!loc6)
	{
		UE_LOG(LogTemp, Warning, TEXT("counter-attack target too far away for default skill %s"), *loc4->skillname);
		return;
	}

	AddSkillActions(loc4, target, loc2, -1, nullptr, true);
	target->SetCounterAttackReady(false);

	UE_LOG(LogTemp, Warning, TEXT("counterAttack set: %s will attack %s with %s"), *target->charName, *attacker->charName, *loc4->skillname);
}

void UBattlefield::HexNeighboursHelper(int32 nGridX, int32 nGridY, TArray<AHex*>& tArray)
{
	AHex* hex = HexInGrid(nGridX, nGridY);

	if (helperIncludeNull)
	{
		tArray.Add(hex);
	}
	else if (hex != nullptr)
	{
		tArray.Add(hex);
	}
}

bool UBattlefield::HexesAdjacent(AHex* hexA, AHex* hexB)
{
	int32 aGridX = 0;
	int32 aGridY = 0;
	int32 bGridX = 0;
	int32 bGridY = 0;

	aGridX = hexA->gridX;
	bGridX = hexA->gridY;
	aGridY = hexB->gridX;
	bGridY = hexB->gridY;

	if (bGridY == bGridX)
	{
		if (aGridX - 1 == aGridY)
		{
			return true;
		}
		if (aGridX + 1 == aGridY)
		{
			return true;
		}
		return false;
	}

	if (bGridX % 2 == 1)
	{
		if (aGridY == aGridX)
		{
			if (bGridX - 1 == bGridY)
			{
				return true;
			}
			if (bGridX + 1 == bGridY)
			{
				return true;
			}
			return false;
		}
		if (aGridY == aGridX + 1)
		{
			if (bGridX - 1 == bGridY)
			{
				return true;
			}
			if (bGridX + 1 == bGridY)
			{
				return true;
			}
			return false;
		}
		return false;
	}

	if (aGridY == aGridX)
	{
		if (bGridX - 1 == bGridY)
		{
			return true;
		}
		if (bGridX + 1 == bGridY)
		{
			return true;
		}
		return false;
	}

	if (aGridY == aGridX - 1)
	{
		if (bGridX - 1 == bGridY)
		{
			return true;
		}
		if (bGridX + 1 == bGridY)
		{
			return true;
		}
		return false;
	}

	return false;
}

bool UBattlefield::IsBehindTargetBack(ADragonAgeJourneysCharacter* avatar1, ADragonAgeJourneysCharacter* avatar2)
{
	//TODO IsBehindTargetBack for backstab
	/*float av1x = avatar1->GetActorRotation().Roll;
	float av1y = avatar1->GetActorRotation().Pitch;
	float av1z = avatar1->GetActorRotation().Yaw;

	float av2x = avatar2->GetActorRotation().Roll;
	float av2y = avatar2->GetActorRotation().Pitch;
	float av2z = avatar2->GetActorRotation().Yaw;

	float abc = av1x + av2x + av1y + av2y + av1z + av2z;*/

	if (FMath::FloorToInt(avatar1->GetActorRotation().Yaw) == FMath::FloorToInt(avatar2->GetActorRotation().Yaw))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s behind %s"), *avatar1->GetCharRef()->charName, *avatar2->GetCharRef()->charName);
		return true;
	}
	return false;
}

bool UBattlefield::DoesSkillMiss(UGameCharacter* gameCharacter, USkill* skill, int32 nIndex)
{
	if (skill->targetsFriendlies)
	{
		return false;
	}

	if (skill->targetsSelf)
	{
		return false;
	}

	if (skill->GetAlwaysHits())
	{
		return false;
	}

	if (skill->type == SKILL_TYPE_TOGGLED)
	{
		return false;
	}

	if (skill->consumable != nullptr)
	{
		return false;
	}

	int32 loc1 = CalculateAccuracy(skill->char_ref, hexes[nIndex], skill);
	if (loc1 == -1)
	{
		return false;
	}

	int32 loc2 = FMath::RandRange(0, 99);
	FString sMiss = (loc2 >= loc1) ? "MISS" : "HIT";

	UE_LOG(LogTemp, Warning, TEXT("accuracy roll %i, needed < %i => %s"), loc2, loc1, *sMiss);
	if (loc2 >= loc1)
	{
		return true;
	}

	return false;
}

bool UBattlefield::TrySpringTrap(UGameCharacter* gameCharacter, ATrap* trap)
{
	if (AreAllies(trap->owner, gameCharacter))
	{
		return false;
	}

	gameCharacter->SetCounterAttackReady(false);
	//TODO blood searh for word
	//gameCharacter->avatar->ShowBlood = false;
	gameCharacter->TakeDamage(trap->GetDamage(), trap->owner/*, trap->hex*/);
	//AddAnimationToAvatar(new packages.battle.CasterAnim(new target_trapped()), gameCharacter->avatar, nullptr);
	//packages.audio.Sounds.play("skip_stunned", 1);

	if (gameCharacter->health > 0)
	{
		UAction* _trapAction = NewObject<UAction>();
		_trapAction->InitializeAction(ACTION_SKILL_WAIT_FOR_ANIMATION, gameCharacter);
		_trapAction->skillMiss = false;
		actionQueue.Add(_trapAction);
	}

	trap->active = false;

	//TODO remove trap actor
	/*if (trap->clip.parent)
	{
		trap->clip.parent.removeChild(trap->clip);
	}*/

	UE_LOG(LogTemp, Warning, TEXT("TRAP HIT!!"));

	return true;
}

bool UBattlefield::IsPlayingAnim()
{
	for (int32 i = 0; i < characters.Num(); i++)
	{
		if (characters[i]->avatar->AnimInstance->IsCustom())
			return true;
	}

	return false;
}

TArray<FInt32Array> UBattlefield::BuildAdjacencyMap()
{
	int32 nX = 0;
	int32 nY = 0;
	int32 nNodes = NUM_HEXES_X * NUM_HEXES_Y;
	FString sString;

	if (nNodes != hexes.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("!! - getAdjacencyMap - array size mismatch! (%i, %i)"), nNodes, hexes.Num());
	}

	TArray<AHex*> obstructedHexes;
	TArray<FInt32Array> arMap; arMap.SetNum(nNodes);

	while (nX < nNodes)
	{
		arMap[nX].arrInt32.SetNum(nNodes);

		if (hexes[nX]->CanShoot(GetCurrentCharacter()) == false)
		{
			obstructedHexes.Add(hexes[nX]);
		}

		nY = 0;
		while (nY < nNodes)
		{
			sString = "" + FString::FromInt(nX) + " (" + FString::FromInt(hexes[nX]->gridX) + ", " + FString::FromInt(hexes[nX]->gridY) + "), " + FString::FromInt(nY) + " (" + FString::FromInt(hexes[nY]->gridX) + ", " + FString::FromInt(hexes[nY]->gridY) + ") : ";
			if (hexes[nY]->CanMove(GetCurrentCharacter()) == false)
			{
				arMap[nX].arrInt32[nY] = NO_CONNECTION;
				sString = sString + "no connection -- canMove() false";
			}
			else if (nX == nY)
			{
				arMap[nX].arrInt32[nY] = 0;
				sString = sString + "distance 0 -- starting point";
			}
			else if (HexesAdjacent(hexes[nX], hexes[nY]))
			{
				if (hexes[nX]->gridX == hexes[nY]->gridX)
				{
					arMap[nX].arrInt32[nY] = 1;
				}
				else
				{
					arMap[nX].arrInt32[nY] = 1;
				}
				sString = sString + "adjacent";
			}
			else
			{
				arMap[nX].arrInt32[nY] = NO_CONNECTION;
				sString = sString + "no connection - misc";
			}


			/*#ifdef DEBUG
			UE_LOG(LogTemp, Warning, TEXT("%s"), *sString);
#endif // DEBUG*/

			nY++;
		}
		nX++;
	}

	return arMap;
}

TArray<AHex*> UBattlefield::HexNeighbours(AHex* hex, bool bHelperIncludeNull /*= false*/)
{
	helperIncludeNull = bHelperIncludeNull;
	TArray<AHex*> hexArray;

	if (hex == nullptr)
	{
		return hexArray;
	}

	int32 nGridX = hex->gridX;
	int32 nGridY = hex->gridY;

	if (nGridY % 2 != 1)
	{
		HexNeighboursHelper(nGridX, nGridY - 1, hexArray);
		HexNeighboursHelper(nGridX + 1, nGridY, hexArray);
		HexNeighboursHelper(nGridX, nGridY + 1, hexArray);
		HexNeighboursHelper(nGridX - 1, nGridY + 1, hexArray);
		HexNeighboursHelper(nGridX - 1, nGridY, hexArray);
		HexNeighboursHelper(nGridX - 1, nGridY - 1, hexArray);
	}
	else
	{
		HexNeighboursHelper(nGridX + 1, nGridY - 1, hexArray);
		HexNeighboursHelper(nGridX + 1, nGridY, hexArray);
		HexNeighboursHelper(nGridX + 1, nGridY + 1, hexArray);
		HexNeighboursHelper(nGridX, nGridY + 1, hexArray);
		HexNeighboursHelper(nGridX - 1, nGridY, hexArray);
		HexNeighboursHelper(nGridX, nGridY - 1, hexArray);
	}

	return hexArray;
}

TArray<AHex*> UBattlefield::HexNeighboursForLargeCreature(AHex* hex, UGameCharacter* gameCharacter)
{
	//TODO HexNeighboursForLargeCreature
	UE_LOG(LogTemp, Error, TEXT("//TODO HexNeighboursForLargeCreature"));

	TArray<AHex*> arResult;
	return arResult;
}

TArray<AHex*> UBattlefield::GetHexesBetween(AHex* hexStart, AHex* hexEnd)
{
	/*bool loc8 = false;*/
	TArray<AHex*> _hexesBetween;

	UE_LOG(LogTemp, Warning, TEXT("getHexesBetween(%s , %s)"), *hexStart->StringData(), *hexEnd->StringData());

	FVector loc2 = FVector(hexEnd->GetActorLocation().X - hexStart->GetActorLocation().X, hexEnd->GetActorLocation().Y - hexStart->GetActorLocation().Y, GetMain()->LevelMap->MapZ);
	loc2.Normalize(LOS_TEST_FREQ);
	FVector loc3 = FVector(hexStart->GetActorLocation().X, hexStart->GetActorLocation().Y, GetMain()->LevelMap->MapZ);
	bool loc4 = false;

	//TODO visual lines
	/*if (DragonMain.DEBUG_LOS)
	{
		UE_LOG(LogTemp, Warning, TEXT("  starting point: " + loc3.toString() + ", target point: " + hexEnd->GetActorLocation().X + ", " + hexEnd->GetActorLocation().Y + " direction vector = " + loc2.toString());
		stageGUI.graphics.lineStyle(3, 16711680);
		stageGUI.graphics.moveTo(hexStart->GetActorLocation().X, hexStart->GetActorLocation().Y);
		stageGUI.graphics.lineTo(hexEnd->GetActorLocation().X, hexEnd->GetActorLocation().Y);
	}*/

	AHex* loc6 = hexStart;

	float maxX = 0.f;
	float maxY = 0.f;

	for (int32 i = 0; i < hexes.Num(); i++)
	{
		maxX = FMath::Max(maxX, hexes[i]->GetActorLocation().X);
		maxY = FMath::Max(maxY, hexes[i]->GetActorLocation().Y);
	}

	//TODO doublecheck

	//while (!loc4 && loc3.X < DragonMain.APP_WIDTH - 90 && loc3.X > 90 && loc3.Y > 90 && loc3.Y < DragonMain.APP_HEIGHT - 90) 
	while (!loc4 && loc3.X < maxX && loc3.X > 0 && loc3.Y > 0 && loc3.Y < maxY)
	{
		loc3 = FVector(loc3.X + loc2.X, loc3.Y + loc2.Y, GetMain()->LevelMap->MapZ);
		// 		loc3.X = loc3.X + loc2.X;
		// 		loc3.Y = loc3.Y + loc2.Y;

				/*if (DragonMain.DEBUG_LOS)
				{
					if (loc5)
					{
						stageGUI.graphics.lineStyle(1, 0);
					}
					else
					{
						stageGUI.graphics.lineStyle(1, 16777215);
					}
					stageGUI.graphics.moveTo(loc3.X - loc2.X, loc3.Y - loc2.Y);
					stageGUI.graphics.lineTo(loc3.X, loc3.Y);
					loc5 = !loc5;
				}*/

		AHex* loc7 = GetHexAtPoint(loc3);
		if (loc7 != nullptr && loc7 != loc6)
		{
			loc6 = loc7;
			if (loc7 == hexEnd)
			{
				loc4 = true;
				UE_LOG(LogTemp, Warning, TEXT("  hit target hex"));
			}
			else
			{
				/*loc8 = false;*/
				_hexesBetween.Add(loc7);
			}
		}
	}

	/*#ifdef DEBUG
		FString loc9 = "get hexesbetween - found " + FString::FromInt(loc1.Num()) + "\n";

		for (int32 i = 0; i < hexes.Num(); i++)
		{
			AHex* loc10 = hexes[i];
			FString sBool = loc10->CanShoot(GetCurrentCharacter()) ? "TRUE" : "FALSE";
			loc9 = loc9 + (" [" + loc10->StringData() + "] - Shoot: " + sBool);
			loc9 = loc9 + "\n";
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *loc9);
	#endif // DEBUG*/

	return _hexesBetween;
}

TArray<AHex*> UBattlefield::GetRouteTo(AHex* hex, UDijkstra* dijkstra /*= nullptr*/)
{
	TArray<AHex*> loc3;

	if (dijkstra == nullptr)
	{
		dijkstra = dSearch;
	}

	if (HexesAdjacent(GetCurrentHex(), hex))
	{
		loc3.Add(GetCurrentHex());
		loc3.Add(hex);
		return loc3;
	}
	if (dijkstra == nullptr)
	{
		TArray<FInt32Array> loc7 = BuildAdjacencyMap();
		dijkstra = NewObject<UDijkstra>();
		dijkstra->InitializeDijkstra(loc7, GetCurrentHex()->index);
		dijkstra->FindShortestPath();
	}

	int32 loc4 = 1000;
	TArray<int32> loc5 = dijkstra->FindShortestPathTo(hex->index, loc4);

	for (int32 i = 0; i < loc5.Num(); i++)
	{
		int32 loc6 = loc5[i];
		loc3.Add(hexes[loc6]);
	}

	return loc3;
}

TArray<AHex*> UBattlefield::GetHexesUnderTemplate(AHex* targetHex, USkill* skill)
{
	TArray<AHex*> arResult;
	AHex* tempHex = nullptr;
	int32 nTempX = 0;

	UE_LOG(LogTemp, Warning, TEXT("getHexesUnderTemplate - targetHex = %s"), *targetHex->GetName());

	if (!targetHex)
	{
		return arResult;
	}

	if (skill->targetsTemplate == TEMPLATE_HORIZONTAL4)
	{
		if (targetHex->GetActorLocation().X < GetCurrentHex()->GetActorLocation().X)
		{
			arResult = GetHexesInLine(GetCurrentHex(), HEX_LEFT, 4);
		}
		else
		{
			arResult = GetHexesInLine(GetCurrentHex(), HEX_RIGHT, 4);
		}
	}
	if (skill->targetsTemplate == TEMPLATE_CONE)
	{
		if (targetHex->GetActorLocation().X < GetCurrentHex()->GetActorLocation().X)
		{
			targetHex = grid[GetCurrentHex()->gridX].arrHex[GetCurrentHex()->gridY];
			arResult = GetHexesInLine(targetHex, HEX_LEFT, 3);
			if (targetHex->gridX < grid.Num())
			{
				if (GetCurrentHex()->gridY % 2 != 1)
				{
					nTempX = targetHex->gridX - 1;
				}
				else
				{
					nTempX = targetHex->gridX;
				}
				if (targetHex->gridY - 1 >= 0)
				{
					arResult.Append(GetHexesInLine(grid[nTempX].arrHex[targetHex->gridY - 1], HEX_LEFT, 3));
				}
				if (targetHex->gridY + 1 < grid[0].arrHex.Num())
				{
					arResult.Append(GetHexesInLine(grid[nTempX].arrHex[targetHex->gridY + 1], HEX_LEFT, 3));
				}
			}

			tempHex = grid[targetHex->gridX - 3].arrHex[targetHex->gridY - 2];
			if (tempHex != nullptr)
			{
				arResult.Add(tempHex);
			}
			tempHex = grid[targetHex->gridX - 3].arrHex[targetHex->gridY + 2];
			if (tempHex != nullptr)
			{
				arResult.Add(tempHex);
			}

		}
		else
		{
			targetHex = grid[GetCurrentHex()->gridX].arrHex[GetCurrentHex()->gridY];
			arResult = GetHexesInLine(targetHex, HEX_RIGHT, 3);
			if (targetHex->gridX < grid.Num())
			{
				if (GetCurrentHex()->gridY % 2 != 1)
				{
					nTempX = targetHex->gridX;
				}
				else
				{
					nTempX = targetHex->gridX + 1;
				}
				if (targetHex->gridY - 1 >= 0)
				{
					arResult.Append(GetHexesInLine(grid[nTempX].arrHex[targetHex->gridY - 1], HEX_RIGHT, 3));
				}
				if (targetHex->gridY + 1 < grid[0].arrHex.Num())
				{
					arResult.Append(GetHexesInLine(grid[nTempX].arrHex[targetHex->gridY + 1], HEX_RIGHT, 3));
				}

			}

			tempHex = grid[targetHex->gridX + 3].arrHex[targetHex->gridY - 2];
			if (tempHex != nullptr)
			{
				arResult.Add(tempHex);
			}

			tempHex = grid[targetHex->gridX + 3].arrHex[targetHex->gridY + 2];
			if (tempHex != nullptr)
			{
				arResult.Add(tempHex);
			}

		}
	}
	return arResult;
}

TArray<AHex*> UBattlefield::GetHexesInLine(AHex* hex, int32 nDirection, int32 nAmount)
{
	TArray<AHex*> arResult;
	while (hex != nullptr && arResult.Num() < nAmount)
	{
		hex = hex->neighbours[nDirection];
		if (hex)
		{
			arResult.Add(hex);
		}
	}

	return arResult;
}

AHex* UBattlefield::HexInGrid(int32 nGridX, int32 nGridY)
{
	if (nGridX < 0)
	{
		return nullptr;
	}
	if (nGridX >= NUM_HEXES_X)
	{
		return nullptr;
	}
	if (nGridY < 0)
	{
		return nullptr;
	}
	if (nGridY >= NUM_HEXES_Y)
	{
		return nullptr;
	}
	return grid[nGridX].arrHex[nGridY];
}

AHex* UBattlefield::GetCurrentHex()
{
	if (currentAvatar->hex)
		return currentAvatar->hex;
	else if (grid[currentAvatar->battlefieldX].arrHex[currentAvatar->battlefieldY])
		return grid[currentAvatar->battlefieldX].arrHex[currentAvatar->battlefieldY];
	else
	{
		return nullptr;
	}
}

AHex* UBattlefield::GetHexAtPoint(FVector vPoint)
{
	int32 fX = vPoint.X;
	int32 fY = vPoint.Y;

	int32 posX;
	int32 posY;

	bool bFoundX = false;
	bool bFoundY = false;

	if (fX > hexes[0]->GetActorLocation().X && fX < hexes[1]->GetActorLocation().X)
	{
		posX = hexes[0]->gridX;
		bFoundX = true;
	}

	if (fY > hexes[0]->GetActorLocation().Y && fY < hexes[1]->GetActorLocation().Y)
	{
		posY = hexes[0]->gridY;
		bFoundY = true;
	}

	if (!bFoundX)
	{
		for (int32 i = 1; i < hexes.Num(); i++)
		{
			if (fX < hexes[i]->GetActorLocation().X)
			{
				posX = hexes[i - 1]->gridX;
				bFoundX = true;
				break;
			}
		}
	}

	if (!bFoundY)
	{
		for (int32 i = 1; i < hexes.Num(); i++)
		{
			if (fY < hexes[i]->GetActorLocation().Y)
			{
				posY = hexes[i - 1]->gridY;
				bFoundY = true;
				break;
			}
		}
	}

	if (bFoundX && bFoundY)
	{
		return grid[bFoundX].arrHex[bFoundY];
	}

#ifdef DEBUG
	UE_LOG(LogTemp, Error, TEXT("GetHexAtPoint no hex!!"));
#endif // DEBUG

	return nullptr;
}

AHex* UBattlefield::FindReviveHexFor(ADragonAgeJourneysCharacter* avatar)
{
	AHex* loc1 = avatar->deathHex;

	if (loc1->GetAnAvatar() == nullptr)
	{
		return loc1;
	}

	if (loc1->GetAnAvatar() != nullptr)
	{
		for (int32 i = 0; i < loc1->neighbours.Num(); i++)
		{
			AHex* loc2 = loc1->neighbours[i];

			if (loc2->GetAnAvatar() != nullptr)
			{
				return loc2;
			}
		}
	}

	return nullptr;
}

UGameCharacter* UBattlefield::GetCurrentCharacter()
{
	if (currentAvatar)
	{
		return currentAvatar->GetCharRef();
	}

	if (currentAction)
	{
		return currentAction->gameCharacter;
	}

	return nullptr;
}

float UBattlefield::HexDistance(AHex* hex, UDijkstra* dijkstra /*= nullptr*/)
{
	if (dijkstra == nullptr)
	{
		dijkstra = dSearch;
	}

	if (dijkstra == nullptr)
	{
		return NO_CONNECTION;
	}

	return dijkstra->distance[hex->index];
}

int32 UBattlefield::SubtractPowerForSkill(USkill* skill)
{
	auto loc1 = 0;
	if (GetCurrentCharacter()->IsUsingHealthForPower())
	{
		if (skill->costsRemainingPower)
		{
			loc1 = GetCurrentCharacter()->ReduceHealth(GetCurrentCharacter()->health);
		}
		else
		{
			loc1 = GetCurrentCharacter()->ReduceHealth(skill->powerCost);
		}
	}
	else if (skill->costsRemainingPower)
	{
		loc1 = GetCurrentCharacter()->ReducePower(GetCurrentCharacter()->power);
	}
	else
	{
		loc1 = GetCurrentCharacter()->ReducePower(skill->powerCost);
	}

	//TODO GetCurrentCharacter()->portrait.update();

	return loc1;
}

int32 UBattlefield::CalculateAccuracy(UGameCharacter* gameCharacter, AHex* hex, USkill* skill)
{
	if (skill->GetAlwaysHits())
	{
		return -1;
	}

	if (skill->moveOnly)
	{
		return -1;
	}

	if (skill->IsSpell())
	{
		return -1;
	}

	if (skill->targetsSelf)
	{
		return -1;
	}

	if (skill->consumable)
	{
		return -1;
	}

	if (hex->GetAnAvatar() == nullptr)
	{
		return 100;
	}

	if (gameCharacter->IsHidden() && skill->skillId == SKILL_STRIKE)
	{
		return 100;
	}

	UGameCharacter* loc1 = hex->GetAnAvatar()->GetCharRef();
	if (loc1 && loc1->isRunt)
	{
		return 100;
	}

	int32 nAccuracy = 80;
	nAccuracy = nAccuracy + skill->attacksWithAccuracyMod;

	int32 loc4 = loc1->GetDefense();
	if (skill->rangedAttack)
	{
		loc4 = loc4 + loc1->GetDefenseMissile();
	}

	int32 loc5 = gameCharacter->GetAttack() - loc4;
	nAccuracy = nAccuracy + loc5;
	nAccuracy = FMath::Min(nAccuracy, 100);
	nAccuracy = FMath::Max(nAccuracy, 10);

	return nAccuracy;
}
