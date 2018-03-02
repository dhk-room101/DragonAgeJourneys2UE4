// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UserWidgetSlotIcon.generated.h"

class UBorder;
class USkill;
class UGameCharacter;

/**
 *
 */
UCLASS()
class DRAGONAGEJOURNEYS_API UUserWidgetSlotIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSkill(USkill* skill);
	void SetSkillEmpty();
	void SetUsable(bool bUsable = true);
	void SetCharacterOwner(UGameCharacter* gameCharacter);

	UPROPERTY()
		UBorder* SlotIcon;
	UPROPERTY()
		UBorder* SlotToggleable;
	UPROPERTY()
		UBorder* SlotToggled;
	UPROPERTY()
		UBorder* SlotSelected;
	UPROPERTY()
		USkill* skill_ref = nullptr;
	UPROPERTY()
		UGameCharacter* char_ref = nullptr;
	FString tt_text;
	FString tt_title;

	bool usable = true;
	bool bEmpty = true;

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	void CheckTouch();

	UPROPERTY()
		FTimerHandle tTouch; 
	int32 nTouch = 0;
};
