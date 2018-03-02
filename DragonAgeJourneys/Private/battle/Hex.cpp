#include "Hex.h"
#include "Kismet/KismetMathLibrary.h"
#include "DragonAgeJourneys.h"
#include "DragonAgeJourneysCharacter.h"
#include "GameCharacter.h"
#include "DragonAgeJourneysPlayerController.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "Battlefield.h"
#include "UserWidgetQuickBar.h"
#include "Skill.h"
#include "HexDirection.h"
#include "GameFramework/Actor.h"
#include <TimerManager.h>
#include "Map.h"

AHex::AHex(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AHex::BattleHexClicked()
{
	if (nTouch == 0)
	{
		nTouch++;
		GetWorldTimerManager().SetTimer(tTouch, this, &AHex::CheckTouch, 0.25, false);
	}
	else nTouch++;
}

void AHex::AttachAvatar(ADragonAgeJourneysCharacter* gameAvatar)
{
	if (avatar != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("*ERROR* attempt to attach second avatar"));
		return;
	}
	gameAvatar->battlefieldX = gridX;
	gameAvatar->battlefieldY = gridY;
	gameAvatar->hex = this;
	avatar = gameAvatar;
}

void AHex::DetachAvatar()
{
	if (avatar == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("*ERROR* attempt to detach non-existant avatar"));
		return;
	}
	HexOut();
	avatar->hex = nullptr;
	avatar = nullptr;
}

//actually tapped twice = execute
void AHex::HexClicked()
{
	//hexOver(nullptr);
	USkill* _skill = GetQuickBar()->GetSelectedSkill();
	if (!ModeSkillUpToDate())
	{
		GetMain()->Battlefield->HighlightSkillTargets(_skill);
		return;
	}

	switch (GetMode())
	{
	case HEX_MODE_MOVE:
	case HEX_MODE_MOVEATTACK:
	{
		if (GetMain()->Battlefield->PlayerActionAllowed())
		{
			GetMain()->Battlefield->SetMoveActionForHex(this);
		}
		//hexOut(nullptr);
		break;
	}
	case HEX_MODE_TARGET:
	{
		if (GetMain()->Battlefield->PlayerActionAllowed())
		{
			GetMain()->Battlefield->SetPlayerActionForHex(this);
			// 			GetMain()->Battlefield->RemoveActionIndicator();
			// 			GetMain()->Battlefield->RemoveTemplate();
			SetMode(HEX_MODE_NORMAL, -1);
		}
		//hexOut(nullptr);
		break;
	}
	default:
	{
		break;
	}
	}
}

//actually tapped once = select
void AHex::HexOver()
{
	if (!GetMain()->Battlefield->IsPlayerTurn())
	{
		return;
	}
	
	GetMain()->Battlefield->directionArrow->SetActorHiddenInGame(true);

	//TODO HexOver
	USkill* _skill = GetQuickBar()->GetSelectedSkill();

	ADragonAgeJourneysCharacter* _avatar = GetAnAvatar();
	if (_avatar != nullptr)
	{
		_avatar->GetCharRef()->MouseOver(GetBattleTraceImpact());
	}

	if (obstacle != OBSTACLE_NONE)
	{
		FString loc3 = "Obstacle";
		FString loc4 = "Obstacle of unknown type.";

		if (obstacle == OBSTACLE_SMALL)
		{
			loc3 = "Low Obstacle";
			loc4 = "Blocks movement. Ranged attacks are not blocked.";
		}
		if (obstacle == OBSTACLE_LARGE)
		{
			loc3 = "High Obstacle";
			loc4 = "Completely blocks movement and line-of-sight for ranged attacks.";
		}

		//TODO visual obstacle tooltip
		/*if (visualObstacle)
		{
			visualObstacle.filters = [Filters.cache.avatarMouseOver];
		}
		loc5 = new packages.tooltips.MiscTooltip(loc3, loc4);
		loc5.name = "obstacle_tooltip";
		packages.tooltips.TooltipController.showTooltip(loc5, arg1);*/
	}

	if (!ModeSkillUpToDate())
	{
		GetMain()->Battlefield->HighlightSkillTargets(_skill);
		return;
	}

	GetMain()->Battlefield->SetActionTextForHex(this);
	GetMain()->Battlefield->SetActionIndicatorForHex(this);

	if (GetMode() == HEX_MODE_MOVE)
	{
		GetMain()->Battlefield->ShowMovementPath(this);
		//GetMain()->Battlefield->FadeEnemiesInFrontOfHex(this);
	}

	if (GetMode() == HEX_MODE_TARGET)
	{

		//GetMain()->Battlefield->FadeEnemiesInFrontOfHex(this);

		bool loc6 = false;
		bool bAllies = false;

		//TODO highlight/outline NPCs
		/*if (_avatar != nullptr)
		{
			loc6 = false;
			if (_skill != nullptr && _skill->targetsFriendlies)
			{
				loc6 = true;
			}

			_avatar->HighlightAsTarget(loc6);

			if (_skill != nullptr)
			{
				bAllies = GetMain()->Battlefield->AreAllies(GetMain()->Battlefield->GetCurrentCharacter, _avatar->GetCharRef());

				if (!bAllies && _skill->hitsAllEnemies)
				{
					GetMain()->Battlefield->HighlightAllEnemies(_avatar->charRef);
				}
				else if (bAllies && _skill->hitsAllFriendlies)
				{
					GetMain()->Battlefield->HighlightAllAllies(_avatar->charRef);
				}
			}
		}*/

		if (_skill == nullptr)
		{
			return;
		}

		/*if (_skill->areaEffectRadius > 0)
		{
			GetMain()->Battlefield->MoveAreaIndicatorTo(this);
		}

		if (_skill->targetsTemplate)
		{
			GetMain()->Battlefield->AdjustTemplateForTargetHex(this);
		}*/

		if (_skill->ShowDirection())
		{
			ShowDirection();

			if (directionHex)
			{
				GetMain()->Battlefield->ShowMovementPath(directionHex);
				//GetMain()->Battlefield->FadeEnemiesInFrontOfHex(directionHex);
			}
		}
	}
}

void AHex::HexOut()
{
	//TODO HexOut = Remove all visual indicators
}

void AHex::UpdateHex()
{
	//update hex material based on _mode;	//gotoAndStop(_mode);
	GetCurrentPlayerController()->SetBattleHexMaterialInstance(static_cast<uint32>(_mode), this);

	//TODO not needed?
	/*switch (static_cast<uint32>(_mode))
	{
	case HEX_MODE_MOVE:
	case HEX_MODE_TARGET:
	case HEX_MODE_MOVEATTACK:
	{
		//buttonMode = true;
		break;
	}
	case HEX_MODE_RANGED:
	{
		break;
	}
	default:
	{
		//buttonMode = false;
		break;
	}
	}
	RemoveDirection();*/
}

void AHex::SetMode(int32 nMode, int32 nSkill)
{
	_mode = static_cast<EHexMode>(nMode);
	_modeSkillId = nSkill;
}

void AHex::ShowDirection()
{
	directionHex = nullptr;

	FVector vImpact = GetCurrentPlayerController()->BattleTraceImpact;
	FVector vTarget = GetMain()->Battlefield->targetHex->GetActorLocation();

	TArray<float> arDist;

	for (int32 i = 0; i < attackerHexes.Num(); i++)
	{
		arDist.Add((vImpact - attackerHexes[i]->GetActorLocation()).Size());
	}

	arDist.Sort();

	for (int32 i = 0; i < attackerHexes.Num(); i++)
	{
		if ((vImpact - attackerHexes[i]->GetActorLocation()).Size() == arDist[0])
		{
			directionHex = attackerHexes[i];
			break;
		}
	}

	if (directionHex)
	{
		UE_LOG(LogTemp, Warning, TEXT("directionHex is %s"), *directionHex->GetName());

		GetMain()->Battlefield->directionArrow->SetActorHiddenInGame(false);

		FVector vArrow = FVector(directionHex->GetActorLocation().X, directionHex->GetActorLocation().Y, directionHex->GetActorLocation().Z + 5);
		GetMain()->Battlefield->directionArrow->SetActorLocation(vArrow);

		//TEMP rotation?
		FRotator _rotation = UKismetMathLibrary::FindLookAtRotation(GetMain()->Battlefield->directionArrow->GetActorLocation(), GetMain()->Battlefield->targetHex->GetActorLocation());
		_rotation.Pitch = 0;
		GetMain()->Battlefield->directionArrow->SetActorRotation(_rotation);

	}

	/*directionArrow = new HexDirection();
	addChildAt(directionArrow, 1);
	directionArrow.mouseEnabled = false;
	directionArrow.mouseChildren = false;
	if (dm.stage->GetChildByName("stageDirection") != nullptr)
	{
		dm.stage.removeChild(dm.stage->GetChildByName("stageDirection"));
	}
	stageDirection = new HexDirection();
	stageDirection.name = "stageDirection";
	stageDirection.mouseEnabled = false;
	stageDirection.mouseChildren = false;
	stageDirection.x = x;
	stageDirection.y = y;
	stageDirection.alpha = 0.2;
	dm.stage.addChild(stageDirection);*/

	//loc1 = DVector.vectors2angle(mouseX, mouseY, 0, -1);

	/*if (mouseX <= 0)
	{
		loc1 = 360 - loc1;
	}*/

	/*loc2 = GetNeighbourDirection(loc1);

	if (loc2 == -1)
	{
		RemoveDirection();
		return;
	}*/

	/*directionArrow.gotoAndStop(loc2 + 1);
	stageDirection.gotoAndStop(loc2 + 1);*/

	//directionHex = neighbours[loc2];
}

void AHex::RemoveDirection()
{
	//TODO not needed? hidden in battlefield class...
	//GetMain()->Battlefield->directionArrow->SetActorHiddenInGame(true);
}

int32 AHex::GetMode()
{
	return static_cast<uint32>(_mode);
}

TArray<ADragonAgeJourneysCharacter*> AHex::GetNeighbourAvatars(bool bLarge /*= false*/)
{
	TArray<ADragonAgeJourneysCharacter*> arChars;

	for (int32 i = 0; i < neighbours.Num(); i++)
	{
		AHex* hex = neighbours[i];

		if (hex == nullptr)
		{
			continue;
		}

		if (hex->avatar)
		{
			if (hex->avatar != avatar)
			{
				arChars.Add(hex->avatar);
			}
			continue;
		}

		if (!bLarge)
		{
			continue;
		}

		if (hex->GetAnAvatar() != GetAnAvatar())
		{
			arChars.Add(hex->GetAnAvatar());
		}
	}

	for (int32 i = 0; i < arChars.Num(); i++)
	{
		ADragonAgeJourneysCharacter* gameChar = arChars[i];
		int32 nFirstIndex = 0;
		int32 nLastIndex = arChars.Num() - 1;

		while (nLastIndex >= 0)
		{
			if (arChars[nLastIndex] == gameChar)
			{
				++nFirstIndex;
				if (nFirstIndex > 1)
				{
					arChars.RemoveAt(nLastIndex, 1);
				}
			}
			--nLastIndex;
		}
	}

	return arChars;
}

ADragonAgeJourneysCharacter* AHex::GetAnAvatar()
{
	if (avatar != nullptr)
	{
		return avatar;
	}

	if (largeAvatar != nullptr)
	{
		return largeAvatar;
	}

	return nullptr;
}

FString AHex::StringData()
{
	FString sString = FString::FromInt(index) + " (" + FString::FromInt(gridX) + ", " + FString::FromInt(gridY) + ")";

	if (GetAnAvatar())
	{
		sString += (" - " + GetAnAvatar()->GetCharRef()->charName);
	}

	return sString;
}

bool AHex::CanShoot(UGameCharacter* gameCharacter)
{
	if (obstacle == HEX_OBSTACLE_LARGE)
	{
		return false;
	}

	else if (avatar != nullptr)
	{
		return false;
	}

	else if (largeAvatar != nullptr && largeAvatar->GetCharRef() != gameCharacter)
	{
		return false;
	}

	return true;
}

bool AHex::CanMove(UGameCharacter* gameCharacter)
{
	if (obstacle != HEX_OBSTACLE_NONE)
	{
		return false;
	}
	else if (avatar != nullptr && avatar->GetCharRef() != gameCharacter)
	{
		return false;
	}
	else if (largeAvatar != nullptr && largeAvatar->GetCharRef() != gameCharacter)
	{
		return false;
	}

	return true;
}

bool AHex::CanEndMoveHere(UGameCharacter* gameCharacter)
{
	int32 nSize = 0;

	if (!CanMove(gameCharacter))
	{
		return false;
	}

	if (gameCharacter->IsLargeCreature())
	{
		nSize = FMath::FloorToInt((gameCharacter->creatureWidth - 1) / 2);

		AHex* hexHigh = neighbours[4];
		AHex* hexLow = neighbours[1];

		bool bHigh = false;
		bool bLow = false;

		int32 nIndex = 0;
		while (nIndex < nSize)
		{
			if (hexHigh != nullptr)
			{
				if (hexHigh->CanMove(gameCharacter) != false)
				{
					hexHigh = hexHigh->neighbours[4];
				}
				else
				{
					bHigh = true;
				}
			}

			if (hexLow != nullptr)
			{
				if (hexLow->CanMove(gameCharacter) != false)
				{
					hexLow = hexLow->neighbours[1];
				}
				else
				{
					bLow = true;
				}
			}

			++nIndex;
		}

		if (bHigh || bLow)
		{
			return false;
		}
	}

	return true;
}

bool AHex::ModeSkillUpToDate()
{
	if (_modeSkillId == -1)
	{
		return true;
	}

	USkill* _skill = GetQuickBar()->GetSelectedSkill();
	if (_skill && _skill->skillId == _modeSkillId)
	{
		return true;
	}

	return false;
}

//TODO click vs doubleclick 
void AHex::CheckTouch()
{
	//TODO finish CheckTouch
	if (nTouch == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("TODO!!! %s was clicked!"), *this->GetName());
		GetMain()->Battlefield->targetHex = this;
		HexOver();
	}

	if (nTouch > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("TODO!!! %s was double clicked!"), *this->GetName());
		GetMain()->Battlefield->targetHex = this;
		GetMain()->Battlefield->directionArrow->SetActorHiddenInGame(true);
		HexOver();
		HexClicked();
	}

	nTouch = 0;
}

bool AHex::SameSide(FVector2D p1, FVector2D p2, FVector2D a, FVector2D b)
{
	FVector pp1 = FVector(p1.X, p1.Y, 0);
	FVector pp2 = FVector(p2.X, p2.Y, 0);
	FVector aa = FVector(a.X, a.Y, 0);
	FVector bb = FVector(b.X, b.Y, 0);

	FVector cp1 = FVector::CrossProduct(bb - aa, pp1 - aa);
	FVector cp2 = FVector::CrossProduct(bb - aa, pp2 - aa);
	//cp1 = CrossProduct(b - a, p1 - a);
	//cp2 = CrossProduct(b - a, p2 - a);
	//if (DotProduct(cp1, cp2) >= 0) return true;
	if (FVector::DotProduct(cp1, cp2) >= 0) return true;
	else return false;
}
