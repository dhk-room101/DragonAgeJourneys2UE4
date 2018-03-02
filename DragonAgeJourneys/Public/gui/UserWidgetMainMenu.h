// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetMainMenu.generated.h"

class UButton;
class UTextBlock;
class UBorder;

/**
 * 
 */
UCLASS()
class DRAGONAGEJOURNEYS_API UUserWidgetMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SelectDefaultButton();

	UBorder* Border0; //Continue
	UBorder* Border1; //New Game
	UBorder* Border2; //Load
	UBorder* Border3; //Options
	UBorder* Border4; //Credits
	UBorder* Border5; //Quit

	UButton* Button0;// Continue;
	UButton* Button1;// New Game;
	UButton* Button2;// Load;
	UButton* Button3;// Options;
	UButton* Button4;// Credits;
	UButton* Button5;// Quit;

	UTextBlock* Text0;// Continue;
	UTextBlock* Text1;// New Game;
	UTextBlock* Text2;// Load;
	UTextBlock* Text3;// Options;
	UTextBlock* Text4;// Credits;
	UTextBlock* Text5;// Quit;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void CheckTap();

	UFUNCTION()
		void OnTap0();
	UFUNCTION()
		void OnTap1();
	UFUNCTION()
		void OnTap2();
	UFUNCTION()
		void OnTap3();
	UFUNCTION()
		void OnTap4();
	UFUNCTION()
		void OnTap5();

private:
	void ResetButtons();

	UPROPERTY()
		FTimerHandle tTouch;
	int32 nTouch = 0;
	int32 nButton = -1;
	
	
};
