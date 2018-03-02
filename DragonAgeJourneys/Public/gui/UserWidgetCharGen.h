// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetCharGen.generated.h"

class UBorder;
class UButton;
class UTextBlock;

/**
 *
 */
UCLASS()
class DRAGONAGEJOURNEYS_API UUserWidgetCharGen : public UUserWidget
{
	GENERATED_BODY()

public:
	void CharGenUpdateSelection();

	UTextBlock* TextGender;
	UButton* ButtonFemale;
	UButton* ButtonMale;
	UBorder* SelectFemale;
	UBorder* SelectMale;

	UTextBlock* TextRace;
	UButton* ButtonDwarf;
	UButton* ButtonElf;
	UButton* ButtonHuman;
	UBorder* SelectDwarf;
	UBorder* SelectElf;
	UBorder* SelectHuman;
	UBorder* IconDwarfFemale;
	UBorder* IconDwarfMale;
	UBorder* IconElfFemale;
	UBorder* IconElfMale;
	UBorder* IconHumanFemale;
	UBorder* IconHumanMale;

	UTextBlock* TextClass;
	UButton* ButtonMage;
	UButton* ButtonRogue;
	UButton* ButtonWarrior;
	UBorder* SelectMage;
	UBorder* SelectRogue;
	UBorder* SelectWarrior;
	UBorder* IconMage;
	
	UButton* ButtonStart;


protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnTapFemale(); 
	UFUNCTION()
		void OnTapMale();

	UFUNCTION()
		void OnTapDwarf();
	UFUNCTION()
		void OnTapElf();
	UFUNCTION()
		void OnTapHuman();

	UFUNCTION()
		void OnTapMage();
	UFUNCTION()
		void OnTapRogue();
	UFUNCTION()
		void OnTapWarrior();

	UFUNCTION()
		void OnTapStart();

};
