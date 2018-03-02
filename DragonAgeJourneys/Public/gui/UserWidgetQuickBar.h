// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UserWidgetQuickBar.generated.h"

class UTextBlock;
class UBorder;
class UButton;
class UUserWidgetSlotIcon;
class UGameCharacter;
class USkill;

const int32 MAX_SKILLS = 12;

/**
 *
 */
UCLASS()
class DRAGONAGEJOURNEYS_API UUserWidgetQuickBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void HandleIconPosition();
	void ResetSlots(int32 nSlot = -1);
	void SetItemSlotText(int32 nSlot, int32 bRemove = 0);
	void SetSelectedSkill(int32 nIndex, bool bAnimate = true);
	void SetSelectedNone();
	void SetGameCharacter(UGameCharacter* gameCharacter);
	void UpdateDefaultAttack();
	int32 GetSelectedSkillInd();
	bool AddIcon(UUserWidgetSlotIcon* slotIcon, int32 nIndex);
	FVector2D GetRelativeSlotPosition(int32 nSlot);
	USkill* GetSelectedSkill();

	int32 nIndex = -1;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnTapEndTurn();

private:
	void ClearSkills();

	UPROPERTY()
		TArray<UUserWidgetSlotIcon*> icons;
	UPROPERTY()
		TArray<bool> emptySlots;

	int32 selected_id = -1;
	UPROPERTY()
		UGameCharacter* character = nullptr;
	UPROPERTY()
		UUserWidgetSlotIcon* defaultSkillIcon = nullptr;

	UUserWidgetSlotIcon* qkbSkill0;
	UUserWidgetSlotIcon* qkbSkill1;
	UUserWidgetSlotIcon* qkbSkill2;
	UUserWidgetSlotIcon* qkbSkill3;
	UUserWidgetSlotIcon* qkbSkill4;
	UUserWidgetSlotIcon* qkbSkill5;
	UUserWidgetSlotIcon* qkbSkill6;
	UUserWidgetSlotIcon* qkbSkill7;
	UUserWidgetSlotIcon* qkbSkill8;
	UUserWidgetSlotIcon* qkbSkill9;
	UUserWidgetSlotIcon* qkbSkill10;
	UUserWidgetSlotIcon* qkbSkill11;

	/*UTextBlock* txtSkill0;
	UTextBlock* txtSkill1;
	UTextBlock* txtSkill2;
	UTextBlock* txtSkill3;
	UTextBlock* txtSkill4;
	UTextBlock* txtSkill5;
	UTextBlock* txtSkill6;
	UTextBlock* txtSkill7;*/

	/*//item
	UTextBlock* txtSkill8;
	UTextBlock* txtCurrentA;
	UTextBlock* txtMaxA;
	UTextBlock* txtSlashA;

	UTextBlock* txtSkill9;
	UTextBlock* txtCurrentB;
	UTextBlock* txtMaxB;
	UTextBlock* txtSlashB;*/

	/*//hourglass
	UBorder* qkbHourglass;*/

	UTextBlock* txtEndTurn;
	UButton* qkbEndTurnButton;
	UBorder* qkbEndTurn;
	UBorder* qkbFrame;
	UBorder* qkbHighlight;
};
