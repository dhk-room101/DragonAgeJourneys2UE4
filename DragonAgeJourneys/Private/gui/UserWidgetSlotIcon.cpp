// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetSlotIcon.h"
#include "UserWidgetQuickBar.h"

#include "WidgetBlueprintLibrary.h"
#include "WidgetLayoutLibrary.h"
#include "UMG.h"
#include "DragonAgeJourneysPlayerController.h"
#include "Scripting.h"
#include "Skill.h"
#include "Skills.h"
#include "Consumable.h"
#include "GameCharacter.h"
#include <UnrealString.h>
#include <UObjectBaseUtility.h>

void UUserWidgetSlotIcon::SetSkill(USkill* skill)
{
	if (skill == nullptr)
	{
		return;
	}

	skill_ref = skill;
	tt_text = skill_ref->GetSkillDescription();
	tt_title = skill_ref->skillname;

	//clip.gotoAndStop(skill->iconFrame);
	UTexture2D* _texture = GetCurrentPlayerController()->skillTextures[skill->iconFrame];
	FSlateBrush brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(_texture);
	SlotIcon->SetBrush(brush);
	bEmpty = false;

	if (skill->skillId == SKILL_CONSUMABLE)
	{
		int32 loc1 = 0;
		if (skill->consumable)
		{
			loc1 = skill->consumable->stack;
		}

		//TODO skill consumable
		UE_LOG(LogTemp, Warning, TEXT("//TODO skill consumable %s "), *tt_title);
		/*loc2 = clip;
		clip = new InventoryIconSWF();
		clip.width = 42;
		clip.height = 42;
		if (loc2.parent)
		{
			loc2.parent.addChild(clip);
			loc2.parent.removeChild(loc2);
		}
		loc2.removeEventListener(flash.events.MouseEvent.MOUSE_OVER, mouseOverTarget);
		loc2.removeEventListener(flash.events.MouseEvent.MOUSE_OUT, mouseOutTarget);
		clip.gotoAndStop(skill->consumable->iconFrame);
		clip.mouseChildren = false;
		loc3 = new HotkeyText();
		clip.addChild(loc3);
		clip.hotkeyText = loc3;
		clip.hotkeyText.scaleX = clip.hotkeyText.scaleX / clip.scaleX;
		clip.hotkeyText.scaleY = clip.hotkeyText.scaleY / clip.scaleY;
		loc4 = clip.stack_size;
		loc4.selectable = false;
		loc4.tabEnabled = false;
		loc4.text = "x" + loc1;
		if (loc1 > 0)
		{
			loc4.textColor = 16777215;
		}
		else
		{
			loc4.textColor = 16711680;
		}
		dmf.embolden(loc4);
		clip.quantityText = loc4;
		clip.addEventListener(flash.events.MouseEvent.MOUSE_OVER, mouseOverTarget, false, 0, true);
		clip.addEventListener(flash.events.MouseEvent.MOUSE_OUT, mouseOutTarget, false, 0, true);
		clip.hotkeyText.mouseEnabled = false;
		clip.quantityText.mouseEnabled = false;
		clip.buttonMode = true;
		clip.class_pointer = this;*/
	}

	SetUsable(skill_ref->IsUsable());

	if (skill_ref->char_ref != nullptr && skill_ref->char_ref)
	{
		SetCharacterOwner(skill_ref->char_ref);//char_ref = skill_ref->char_ref;
	}

	if (skill_ref->type == SKILL_TYPE_TOGGLED)
	{
		//TODO skill toggleable
		UE_LOG(LogTemp, Warning, TEXT("//TODO skill toggleable %s "), *tt_title);

		/*loc5 = new SkillIconTogglableGFX();
		loc5.mouseEnabled = false;
		loc5.x = 1;
		loc5.y = 1;
		clip.addChild(loc5);
		showToggled();*/
	}
}

void UUserWidgetSlotIcon::SetSkillEmpty()
{
	FSlateBrush brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(GetCurrentPlayerController()->SkillEmpty);
	SlotIcon->SetBrush(brush);
	SlotToggleable->SetVisibility(ESlateVisibility::Hidden);
	SlotToggled->SetVisibility(ESlateVisibility::Hidden);
	SlotSelected->SetVisibility(ESlateVisibility::Hidden);
	bEmpty = true;
}

void UUserWidgetSlotIcon::SetUsable(bool bUsable /*= true*/)
{
	//TODO SetUsable filters
	usable = bUsable;
}

void UUserWidgetSlotIcon::SetCharacterOwner(UGameCharacter* gameCharacter)
{
	char_ref = gameCharacter;
}

void UUserWidgetSlotIcon::NativeConstruct()
{
	Super::NativeConstruct();

	SlotIcon = (UBorder*)GetWidgetFromName(TEXT("SlotIcon"));
	//SetSkillEmpty();

	SlotToggleable = (UBorder*)GetWidgetFromName(TEXT("SlotToggleable"));
	SlotToggled = (UBorder*)GetWidgetFromName(TEXT("SlotToggled"));
	SlotSelected = (UBorder*)GetWidgetFromName(TEXT("SlotSelected"));

	SlotToggleable->SetVisibility(ESlateVisibility::Hidden);
	SlotToggled->SetVisibility(ESlateVisibility::Hidden);
	SlotSelected->SetVisibility(ESlateVisibility::Hidden);
}

FReply UUserWidgetSlotIcon::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

FReply UUserWidgetSlotIcon::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (nTouch == 0)
	{
		nTouch++;
		GetStage()->GetFirstPlayerController()->GetPawn()->GetWorldTimerManager().SetTimer(tTouch, this, &UUserWidgetSlotIcon::CheckTouch, 0.25, false);
	}
	else nTouch++;

	return FReply::Handled();
}

void UUserWidgetSlotIcon::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& InOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, InOperation);

	if (!InOperation)
		InOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());

	InOperation->DefaultDragVisual = this;
	InOperation->Pivot = EDragPivot::CenterCenter;

	//LogWarning("Drag From slot " + IntToString(GetCurrentPlayerController()->UIQuickBar->nIndex));

	/*GetCurrentPlayerController()->bDragging = true;
	GetCurrentPlayerController()->nDragFrom = GetCurrentPlayerController()->UIQuickBar->nIndex;
	GetCurrentPlayerController()->UIQuickBar->nIndex = -1;

	if (GetCurrentPlayerController()->nDragFrom > MAX_SLOTS_SKILL - 1)
		GetCurrentPlayerController()->bDragItem = TRUE_;

	GetCurrentPlayerController()->GameCursor->SetVisibility(ESlateVisibility::Hidden);*/
}

bool UUserWidgetSlotIcon::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	/*GetCurrentPlayerController()->vDragPos = InDragDropEvent.GetScreenSpacePosition();

	return GetCurrentPlayerController()->bDragging;*/

	return false;
}

bool UUserWidgetSlotIcon::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	//LogWarning("Dropped on slot " + IntToString(GetCurrentPlayerController()->nDragTo));

	//GetCurrentPlayerController()->HandleQuickBarIconSwap();

	return true;
}

void UUserWidgetSlotIcon::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	//LogWarning("Leaving!");

	/*GetCurrentPlayerController()->UIQuickBar->nIndex = -1;
	GetCurrentPlayerController()->UIQuickBar->ResetSlots();
	GetCurrentPlayerController()->nDragTo = -1;*/
}

void UUserWidgetSlotIcon::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	//LogWarning("Cancelled!");

	/*GetCurrentPlayerController()->nDragTo = -1;

	GetCurrentPlayerController()->HandleQuickBarIconSwap();*/
}

void UUserWidgetSlotIcon::CheckTouch()
{
	FString sIndex = GetName();
	sIndex.RemoveFromStart(qkbSkill);
	GetCurrentPlayerController()->UIQuickBar->SetSelectedSkill(FCString::Atoi(*sIndex));
	UE_LOG(LogTemp, Warning, TEXT("%i was clicked!"), FCString::Atoi(*sIndex));
	nTouch = 0;
}