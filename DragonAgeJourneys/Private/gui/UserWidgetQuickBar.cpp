// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetQuickBar.h"

#include "WidgetBlueprintLibrary.h"
#include "WidgetLayoutLibrary.h"
#include "UMG.h"

#include "UserWidgetSlotIcon.h"

#include "UMGDragDropOp.h"
#include "GameCharacter.h"
#include "Skills.h"
#include "Skill.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "Consumable.h"
#include "Group.h"
#include "Battlefield.h"

void UUserWidgetQuickBar::NativeConstruct()
{
	Super::NativeConstruct();

	icons.SetNum(MAX_SKILLS);

	emptySlots.SetNum(MAX_SKILLS);
	for (int32 i = 0; i < emptySlots.Num(); i++)
	{
		emptySlots[i] = true;
	}

	qkbSkill0 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "0")));
	icons[0] = qkbSkill0;

	qkbSkill1 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "1")));
	icons[1] = qkbSkill1;

	qkbSkill2 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "2")));
	icons[2] = qkbSkill2;

	qkbSkill3 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "3")));
	icons[3] = qkbSkill3;

	qkbSkill4 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "4")));
	icons[4] = qkbSkill4;

	qkbSkill5 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "5")));
	icons[5] = qkbSkill5;

	qkbSkill6 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "6")));
	icons[6] = qkbSkill6;

	qkbSkill7 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "7")));
	icons[7] = qkbSkill7;

	qkbSkill8 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "8")));
	icons[8] = qkbSkill8;

	qkbSkill9 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "9")));
	icons[9] = qkbSkill9;

	qkbSkill10 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "10")));
	icons[10] = qkbSkill10;

	qkbSkill11 = (UUserWidgetSlotIcon*)GetWidgetFromName(FName(*(qkbSkill + "11")));
	icons[11] = qkbSkill11;

	/*txtCurrentA = (UTextBlock*)GetWidgetFromName(TEXT("txt_currentA"));
	txtCurrentA->SetVisibility(ESlateVisibility::Hidden);
	txtMaxA = (UTextBlock*)GetWidgetFromName(TEXT("txt_maxA"));
	txtMaxA->SetVisibility(ESlateVisibility::Hidden);
	txtSlashA = (UTextBlock*)GetWidgetFromName(TEXT("txt_slashA"));
	txtSlashA->SetVisibility(ESlateVisibility::Hidden);*/

	qkbEndTurnButton = (UButton*)GetWidgetFromName(TEXT("qkb_endturnbutton"));
	qkbEndTurnButton->OnClicked.AddDynamic(this, &UUserWidgetQuickBar::OnTapEndTurn);
	txtEndTurn = (UTextBlock*)GetWidgetFromName(TEXT("txt_endturn"));
	qkbEndTurn = (UBorder*)GetWidgetFromName(TEXT("qkb_endturn"));
	qkbFrame = (UBorder*)GetWidgetFromName(TEXT("qkb_frame"));
}

void UUserWidgetQuickBar::OnTapEndTurn()
{
	if (GetMain()->Battlefield->IsPlayerTurn())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTapEndTurn"));
		//TODO play sound also in chargen and main menu?
		//Sounds.play("choose_talent");
		GetMain()->Battlefield->PassTurn();
	}
}

void UUserWidgetQuickBar::ClearSkills()
{
	for (int32 i = 0; i < icons.Num(); i++)
	{
		UUserWidgetSlotIcon* loc1 = icons[i];
		if (loc1)
		{
			loc1->SetSkillEmpty();

			/*loc1.clip.removeEventListener(flash.events.MouseEvent.CLICK, skillClick);
			if (clip.contains(loc1.clip))
			{
				clip.removeChild(loc1.clip);
			}*/
		}

		//loc1 = nullptr;//DHK

		/*if (!loc1)
		{
			continue;
		}
		loc1.cleanup();*/
	}

	//icons.Empty(); icons.SetNum(MAX_SKILLS);
	defaultSkillIcon = nullptr;
	selected_id = -1;
}

void UUserWidgetQuickBar::HandleIconPosition()
{
	/*#ifdef DEBUG
	LogWarning("hover over slot " + IntToString(nIndex));
#endif // DEBUG

	ADA2UE4Creature* oCreature = Cast<ADA2UE4Creature>(GetCurrentPlayerController()->GetPawn());
	if (nIndex != -1)
	{
		if (GetCurrentPlayerController()->bDragging || oCreature->QuickBar[nIndex].nAbilityID > 0)
		{
			ResetSlots(nIndex);//all but
		}
		else ResetSlots();//all
	}
	else
	{
		ResetSlots();//all
	}*/
}

void UUserWidgetQuickBar::ResetSlots(int32 nSlot)
{
	/*UTexture2D* t;

	if (nSlot == -1)
	{
		t = *GetCurrentPlayerController()->quickBarIconTextures.Find(FString::FromInt(ABILITY_TRANSPARENT));
		FSlateBrush b = UWidgetBlueprintLibrary::MakeBrushFromTexture(t);
		qkbFrame->SetBrush(b);
	}

	for (int32 k = 0; k < MAX_SLOTS; k++)
	{
		if (k != nSlot)
		{
			UCanvasPanel* Buttons = (UCanvasPanel*)GetCurrentPlayerController()->UIQuickBar->GetWidgetFromName(TEXT("Buttons"));
			if (Buttons)
			{
				for (int32 i = 0; i < Buttons->GetChildrenCount(); i++)
				{
					if (Buttons->GetChildAt(i))
					{
						if (Buttons->GetChildAt(i)->GetName().Contains(FString::FromInt(k)))
						{
							UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(Buttons->GetChildAt(i)->Slot);
							if (slot->GetPosition().Y == 0)
								slot->SetPosition(FVector2D(slot->GetPosition().X, 4));
						}
					}
				}
			}
		}
		else
		{
			if (k >= MAX_SLOTS_SKILL)
				t = *GetCurrentPlayerController()->quickBarIconTextures.Find(FString::FromInt(ABILITY_ITEM_SELECTED));
			else t = *GetCurrentPlayerController()->quickBarIconTextures.Find(FString::FromInt(ABILITY_SKILL_SELECTED));
			FSlateBrush b = UWidgetBlueprintLibrary::MakeBrushFromTexture(t);
			qkbFrame->SetBrush(b);

			UCanvasPanel* Buttons = (UCanvasPanel*)GetCurrentPlayerController()->UIQuickBar->GetWidgetFromName(TEXT("Buttons"));
			if (Buttons)
			{
				for (int32 i = 0; i < Buttons->GetChildrenCount(); i++)
				{
					if (Buttons->GetChildAt(i))
					{
						if (Buttons->GetChildAt(i)->GetName().Contains(FString::FromInt(k)))
						{
							UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Buttons->GetChildAt(i));
							if (slot->GetPosition().Y != 0)
							{
								slot->SetPosition(FVector2D(slot->GetPosition().X, 0));
								if (Buttons->GetChildAt(i)->GetName().Contains(qkbSkill))
									UWidgetLayoutLibrary::SlotAsCanvasSlot(qkbFrame)->SetPosition(slot->GetPosition());

								UWidgetLayoutLibrary::SlotAsCanvasSlot(slot->Parent)->GetPosition();
							}
						}
					}
				}
			}
		}
	}*/
}

void UUserWidgetQuickBar::SetItemSlotText(int32 nSlot, int32 bRemove)
{
	/*if (nSlot < MAX_SLOTS_SKILL) return;

	int32 nMaxItemQty = 0;

	ADA2UE4Creature* oPC = Cast<ADA2UE4Creature>(GetCurrentPlayerController()->GetPawn());

	if (oPC->QuickBar[nSlot].nAbilityID > 0)
	{
		nMaxItemQty = GetSlotItemMaxQty(nSlot);

		if (nMaxItemQty > 0)
		{
			switch (nSlot)
			{
			case MAX_SLOTS_SKILL:
			{
				if (!bRemove)
				{
					txtMaxA->SetText(FText::FromString(FString::FromInt(nMaxItemQty)));
					txtMaxA->SetVisibility(ESlateVisibility::HitTestInvisible);

					txtSlashA->SetVisibility(ESlateVisibility::HitTestInvisible);

					FItem* item = oPC->Inventory.FindByPredicate(
						[&](const FItem& _item)
					{
						return (Cast<ADA2UE4Item>(_item.ItemActor))->Ability == oPC->QuickBar[nSlot].nAbilityID;
					});

					if (item)
					{
						txtCurrentA->SetText(FText::FromString(FString::FromInt(item->StackSize)));
						txtCurrentA->SetVisibility(ESlateVisibility::HitTestInvisible);
					}
				}
				else
				{
					txtCurrentA->SetVisibility(ESlateVisibility::Hidden);
					txtMaxA->SetVisibility(ESlateVisibility::Hidden);
					txtSlashA->SetVisibility(ESlateVisibility::Hidden);
				}
				break;
			}
			case MAX_SLOTS_SKILL + 1:
			{
				if (!bRemove)
				{
					txtMaxB->SetText(FText::FromString(FString::FromInt(nMaxItemQty)));
					txtMaxB->SetVisibility(ESlateVisibility::HitTestInvisible);

					txtSlashB->SetVisibility(ESlateVisibility::HitTestInvisible);

					FItem* item = oPC->Inventory.FindByPredicate(
						[&](const FItem& _item)
					{
						return (Cast<ADA2UE4Item>(_item.ItemActor))->Ability == oPC->QuickBar[nSlot].nAbilityID;
					});

					if (item)
					{
						txtCurrentB->SetText(FText::FromString(FString::FromInt(item->StackSize)));
						txtCurrentB->SetVisibility(ESlateVisibility::HitTestInvisible);
					}
				}
				else
				{
					txtCurrentB->SetVisibility(ESlateVisibility::Hidden);
					txtMaxB->SetVisibility(ESlateVisibility::Hidden);
					txtSlashB->SetVisibility(ESlateVisibility::Hidden);
				}

				break;
			}
			default:
				break;
			}
		}
	}*/
}

void UUserWidgetQuickBar::SetSelectedSkill(int32 nIndex, bool bAnimate /*= true*/)
{
	if (nIndex >= icons.Num())
	{
		return;
	}

	if (icons[nIndex] == nullptr)
	{
		return;
	}

	if (selected_id == nIndex)
	{
		return;
	}

	if (!icons[nIndex]->usable)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("SetSelectedSkill %i"), nIndex);
	for (int32 i = 0; i < icons.Num(); i++)
	{
		icons[i]->SlotSelected->SetVisibility(ESlateVisibility::Hidden);
	}
	
	UUserWidgetSlotIcon* _selectedSkillIcon = icons[nIndex];
	if (GetMain()->Battlefield != nullptr)
	{
		_selectedSkillIcon->SlotSelected->SetVisibility(ESlateVisibility::Visible);
		if (_selectedSkillIcon->skill_ref->type != SKILL_TYPE_TOGGLED)
		{
			/*clip.highlight.x = loc1->clip.x;
			clip.highlight.y = loc1->clip.y;*/
			selected_id = nIndex;
			//clip.setChildIndex(clip.highlight, clip.numChildren - 1);
			GetMain()->Battlefield->HighlightSkillTargets(_selectedSkillIcon->skill_ref);

			//TODO portraits
			//GetMain()->Battlefield->setPortraitsAsSkillTargets(loc1->skill_ref);
		}
		else
		{
			if (character->GetDefaultAttack()->skillId != SKILL_STRIKE)
			{
				SetSelectedSkill(0);
			}
			else
			{
				SetSelectedSkill(1);
			}
			GetMain()->Battlefield->UseInstantSkill(_selectedSkillIcon->skill_ref);
		}

		//TODO AnimateSelection
		/*if (bAnimate)
		{
			AnimateSelection(loc1);
		}*/
		
		//TODO RemoveTemplate
		//GetMain()->Battlefield->RemoveTemplate();
	}

	//TODO no quickbar on map, outside combat?
	/*else
{
	clip.highlight.x = loc1->clip.x;
	clip.highlight.y = loc1->clip.y;
	selected_id = nIndex;
	clip.setChildIndex(clip.highlight, clip.numChildren - 1);
	if (bAnimate)
	{
		animateSelection(loc1);
	}
	packages.map.Map.current.setPortraitsAsSkillTargets(loc1->skill_ref);
}*/
}

void UUserWidgetQuickBar::SetSelectedNone()
{
	qkbHighlight->SetVisibility(ESlateVisibility::Hidden);
}

void UUserWidgetQuickBar::SetGameCharacter(UGameCharacter* gameCharacter)
{
	character = gameCharacter;

	ClearSkills();

	if (gameCharacter == nullptr || gameCharacter->IsMonster())
	{
		if (GetVisibility() != ESlateVisibility::Hidden)
			SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	gameCharacter->RefreshSkills();

	if (GetVisibility() != ESlateVisibility::Visible)
		SetVisibility(ESlateVisibility::Visible);

	if (gameCharacter->group != nullptr)
	{
		if (!gameCharacter->group->player)
		{
			if (GetVisibility() != ESlateVisibility::Hidden)
				SetVisibility(ESlateVisibility::Hidden);
			return;
		}
	}

	/*Cast<UUserWidgetSlotIcon>(
	NewWidgetObject(UUserWidgetSlotIcon::StaticClass()->GetOuter(),
	UUserWidgetSlotIcon::StaticClass(),
	NAME_None, RF_NoFlags)); */ //NewObject<UUserWidgetSlotIcon>();

	UUserWidgetSlotIcon* _slotIcon = icons[0];
	_slotIcon->SlotIcon = (UBorder*)_slotIcon->GetWidgetFromName(TEXT("SlotIcon"));
	USkill* _move = NewObject<USkill>();
	_move->InitializeSkill(SKILL_MOVE, gameCharacter);
	_slotIcon->SetSkill(_move);
	_slotIcon->SetCharacterOwner(gameCharacter);
	AddIcon(_slotIcon, 0);

	//loc1->clip.hotkeyText.text.text = "1";

	UpdateDefaultAttack();

	int32 nIndex = 0;
	while (nIndex < gameCharacter->GetSkills().Num())
	{
		USkill* _skill = gameCharacter->GetSkills()[nIndex];
		if (_skill != nullptr)
		{
			if (_skill->shortcutSlot != -1)
			{
				if (_skill->shortcutSlot == 1 || _skill->shortcutSlot == 2)
				{
					return;
				}

				if (_skill->consumable != nullptr && _skill->consumable->stack <= 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("consumable has empty stack!"));
					_skill->shortcutSlot = -1;
				}
				else
				{
					_slotIcon = icons[_skill->shortcutSlot - 1];// NewObject<UUserWidgetSlotIcon>();
					_slotIcon->SetSkill(_skill);
					_slotIcon->SetCharacterOwner(gameCharacter);
					AddIcon(_slotIcon, _skill->shortcutSlot - 1);

					//TODO disabled, no text index on mobile
					/*if (loc3->shortcutSlot != 10)
					{
						loc1->clip.hotkeyText.text.text = String(loc3->shortcutSlot);
					}
					else
					{
						loc1->clip.hotkeyText.text.text = "0";
					}*/
				}
			}
		}
		++nIndex;
	}

	if (/*packages.map.Map.current*/ GetMain()->Battlefield != nullptr)
	{
		if (gameCharacter->GetDefaultAttack()->skillId == SKILL_STRIKE
			|| gameCharacter->GetDefaultAttack()->skillId == SKILL_BOW_FIRE
			|| gameCharacter->GetDefaultAttack()->skillId == SKILL_MAGE_BOLT)
		{
			SetSelectedSkill(1, false);
		}
		else
		{
			SetSelectedSkill(0, false);
		}
	}
	else
	{
		SetSelectedNone();
	}
}

void UUserWidgetQuickBar::UpdateDefaultAttack()
{
	if (character == nullptr)
	{
		return;
	}

	defaultSkillIcon = icons[1];// NewObject<UUserWidgetSlotIcon>();
	defaultSkillIcon->SetSkill(character->GetDefaultAttack());
	defaultSkillIcon->SetCharacterOwner(character);
	AddIcon(defaultSkillIcon, 1);

	//defaultSkillIcon.clip.hotkeyText.text.text = "2";
}

int32 UUserWidgetQuickBar::GetSelectedSkillInd()
{
	return selected_id;
}

bool UUserWidgetQuickBar::AddIcon(UUserWidgetSlotIcon* slotIcon, int32 nIndex)
{
	bool loc3 = emptySlots[nIndex];
	if (loc3 == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("can\'t find target for icon index %i"), nIndex);
		return false;
	}

	/*slotIcon->clip.x = loc3.clip.x;
	slotIcon->clip.y = loc3.clip.y;*/

	icons[nIndex] = slotIcon;
	slotIcon->SetUsable(slotIcon->skill_ref->IsUsable());

	//clip.addChild(slotIcon->clip);

	//TODO Tutorials
	/*if (slotIcon->skill_ref && slotIcon->skill_ref->type == Skill.TYPE_TOGGLED)
	{
		Scripting.triggerTutorial(packages.gui.Tutorial.TUT_TOGGLED_TALENTS);
	}*/

	//TODO click and drag listeners
	/*slotIcon->clip.addEventListener(flash.events.MouseEvent.CLICK, skillClick);
	slotIcon->clip.addEventListener(flash.events.MouseEvent.MOUSE_DOWN, startItemDragging);
	slotIcon->clip.addEventListener(flash.events.MouseEvent.MOUSE_UP, stopItemDragging);*/

	return true;
}

FVector2D UUserWidgetQuickBar::GetRelativeSlotPosition(int32 nSlot)
{
	/*FName fn = *(qkbSkill + FString::FromInt(nSlot));
	UUserWidgetSlotIcon* slot = (UUserWidgetSlotIcon*)GetCurrentPlayerController()->UIQuickBar->GetWidgetFromName(fn);

	FVector2D v;
	if (slot)
	v = UWidgetLayoutLibrary::SlotAsCanvasSlot(slot)->GetPosition();

	FVector2D vp = GetCurrentPlayerController()->UIGetPosition(EUI::QUICKBAR);

	return FVector2D(v.X + vp.X, v.Y + vp.Y);*/

	FVector2D v2d;
	return v2d;
}

USkill* UUserWidgetQuickBar::GetSelectedSkill()
{
	UUserWidgetSlotIcon* _slotIcon = icons[selected_id];
	
	if (_slotIcon != nullptr)
	{
		return _slotIcon->skill_ref;
	}

	return nullptr;
}
