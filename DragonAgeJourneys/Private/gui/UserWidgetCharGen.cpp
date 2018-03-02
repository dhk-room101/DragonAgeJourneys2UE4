// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetCharGen.h"
#include "DragonAgeJourneys.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameCharacter.h"
#include "Gender.h"
#include "Race.h"
#include "Classes.h"
#include "Scripting.h"
#include "DragonAgeJourneysCharacter.h"
#include "DragonAgeJourneysPlayerController.h"
#include "Slot.h"
#include "DragonAgeJourneysAnimInstance.h"

void UUserWidgetCharGen::NativeConstruct()
{
	Super::NativeConstruct();

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("------ - CharGen Construct---- - "));

#endif // DEBUG

	TextGender = (UTextBlock*)GetWidgetFromName(TEXT("TextGender"));
	ButtonFemale = (UButton*)GetWidgetFromName(TEXT("ButtonFemale"));
	ButtonFemale->OnClicked.AddDynamic(this, &UUserWidgetCharGen::OnTapFemale);
	ButtonMale = (UButton*)GetWidgetFromName(TEXT("ButtonMale"));
	ButtonMale->OnClicked.AddDynamic(this, &UUserWidgetCharGen::OnTapMale);
	SelectFemale = (UBorder*)GetWidgetFromName(TEXT("SelectFemale"));
	SelectMale = (UBorder*)GetWidgetFromName(TEXT("SelectMale"));

	TextRace = (UTextBlock*)GetWidgetFromName(TEXT("TextRace"));
	ButtonDwarf = (UButton*)GetWidgetFromName(TEXT("ButtonDwarf"));
	ButtonDwarf->OnClicked.AddDynamic(this, &UUserWidgetCharGen::OnTapDwarf);
	ButtonElf = (UButton*)GetWidgetFromName(TEXT("ButtonElf"));
	ButtonElf->OnClicked.AddDynamic(this, &UUserWidgetCharGen::OnTapElf);
	ButtonHuman = (UButton*)GetWidgetFromName(TEXT("ButtonHuman"));
	ButtonHuman->OnClicked.AddDynamic(this, &UUserWidgetCharGen::OnTapHuman);
	SelectDwarf = (UBorder*)GetWidgetFromName(TEXT("SelectDwarf"));
	SelectElf = (UBorder*)GetWidgetFromName(TEXT("SelectElf"));
	SelectHuman = (UBorder*)GetWidgetFromName(TEXT("SelectHuman"));
	IconDwarfFemale = (UBorder*)GetWidgetFromName(TEXT("IconDwarfFemale"));
	IconDwarfMale = (UBorder*)GetWidgetFromName(TEXT("IconDwarfMale"));
	IconElfFemale = (UBorder*)GetWidgetFromName(TEXT("IconElfFemale"));
	IconElfMale = (UBorder*)GetWidgetFromName(TEXT("IconElfMale"));
	IconHumanFemale = (UBorder*)GetWidgetFromName(TEXT("IconHumanFemale"));
	IconHumanMale = (UBorder*)GetWidgetFromName(TEXT("IconHumanMale"));

	TextClass = (UTextBlock*)GetWidgetFromName(TEXT("TextClass"));
	ButtonMage = (UButton*)GetWidgetFromName(TEXT("ButtonMage"));
	ButtonMage->OnClicked.AddDynamic(this, &UUserWidgetCharGen::OnTapMage);
	ButtonRogue = (UButton*)GetWidgetFromName(TEXT("ButtonRogue"));
	ButtonRogue->OnClicked.AddDynamic(this, &UUserWidgetCharGen::OnTapRogue);
	ButtonWarrior = (UButton*)GetWidgetFromName(TEXT("ButtonWarrior"));
	ButtonWarrior->OnClicked.AddDynamic(this, &UUserWidgetCharGen::OnTapWarrior);
	SelectMage = (UBorder*)GetWidgetFromName(TEXT("SelectMage"));
	SelectRogue = (UBorder*)GetWidgetFromName(TEXT("SelectRogue"));
	SelectWarrior = (UBorder*)GetWidgetFromName(TEXT("SelectWarrior"));
	IconMage = (UBorder*)GetWidgetFromName(TEXT("IconMage"));

	ButtonStart = (UButton*)GetWidgetFromName(TEXT("ButtonStart"));
	ButtonStart->OnClicked.AddDynamic(this, &UUserWidgetCharGen::OnTapStart);
}

void UUserWidgetCharGen::OnTapFemale()
{
	ADragonAgeJourneysCharacter* avatar =
		(Cast<ADragonAgeJourneysCharacter>(GetCurrentPlayerController()->GetPawn()));

	if (avatar->GetCharRef()->gender != GENDER_FEMALE)
	{
		avatar->GetCharRef()->gender = GENDER_FEMALE;
		avatar->InitDefaultSkeletalMeshes();
		avatar->GetCharRef()->DefaultEquipment();
		CharGenUpdateSelection();
	}
}

void UUserWidgetCharGen::OnTapMale()
{
	ADragonAgeJourneysCharacter* avatar =
		(Cast<ADragonAgeJourneysCharacter>(GetCurrentPlayerController()->GetPawn()));

	if (avatar->GetCharRef()->gender != GENDER_MALE)
	{
		avatar->GetCharRef()->gender = GENDER_MALE;
		avatar->InitDefaultSkeletalMeshes();
		avatar->GetCharRef()->DefaultEquipment();
		CharGenUpdateSelection();
	}
}

void UUserWidgetCharGen::OnTapDwarf()
{
	ADragonAgeJourneysCharacter* avatar =
		(Cast<ADragonAgeJourneysCharacter>(GetCurrentPlayerController()->GetPawn()));

	if (avatar->GetCharRef()->classId == CLASS_MAGE) return;

	if (avatar->GetCharRef()->GetRace() != RACE_DWARF)
	{
		avatar->GetCharRef()->SetRace(RACE_DWARF);
		avatar->GetCharRef()->SetDefaultEquipmentTemplate();
		avatar->InitDefaultSkeletalMeshes();
		avatar->GetCharRef()->DefaultEquipment();
		CharGenUpdateSelection();
	}
}

void UUserWidgetCharGen::OnTapElf()
{
	ADragonAgeJourneysCharacter* avatar =
		(Cast<ADragonAgeJourneysCharacter>(GetCurrentPlayerController()->GetPawn()));

	if (avatar->GetCharRef()->GetRace() != RACE_ELF)
	{
		avatar->GetCharRef()->SetRace(RACE_ELF);
		avatar->GetCharRef()->SetDefaultEquipmentTemplate();
		avatar->InitDefaultSkeletalMeshes();
		avatar->GetCharRef()->DefaultEquipment();
		CharGenUpdateSelection();
	}
}

void UUserWidgetCharGen::OnTapHuman()
{
	ADragonAgeJourneysCharacter* avatar =
		(Cast<ADragonAgeJourneysCharacter>(GetCurrentPlayerController()->GetPawn()));

	if (avatar->GetCharRef()->GetRace() != RACE_HUMAN)
	{
		avatar->GetCharRef()->SetRace(RACE_HUMAN);
		avatar->GetCharRef()->SetDefaultEquipmentTemplate();
		avatar->InitDefaultSkeletalMeshes();
		avatar->GetCharRef()->DefaultEquipment();
		CharGenUpdateSelection();
	}
}

void UUserWidgetCharGen::OnTapMage()
{
	ADragonAgeJourneysCharacter* avatar =
		(Cast<ADragonAgeJourneysCharacter>(GetCurrentPlayerController()->GetPawn()));

	if (avatar->GetCharRef()->GetRace() == RACE_DWARF) return;

	if (avatar->GetCharRef()->classId != CLASS_MAGE)
	{
		avatar->GetCharRef()->classId = CLASS_MAGE;
		avatar->GetCharRef()->InitializeGameCharacter(avatar->GetCharRef()->classId, false);
		avatar->GetCharRef()->SetDefaultEquipmentTemplate();
		avatar->InitDefaultSkeletalMeshes();
		avatar->GetCharRef()->DefaultEquipment();
		CharGenUpdateSelection();
	}
}

void UUserWidgetCharGen::OnTapRogue()
{
	ADragonAgeJourneysCharacter* avatar =
		(Cast<ADragonAgeJourneysCharacter>(GetCurrentPlayerController()->GetPawn()));

	if (avatar->GetCharRef()->classId != CLASS_ROGUE)
	{
		avatar->GetCharRef()->classId = CLASS_ROGUE;
		avatar->GetCharRef()->InitializeGameCharacter(avatar->GetCharRef()->classId, false);
		avatar->GetCharRef()->SetDefaultEquipmentTemplate();
		avatar->InitDefaultSkeletalMeshes();
		avatar->GetCharRef()->DefaultEquipment();
		CharGenUpdateSelection();
	}
}

void UUserWidgetCharGen::OnTapWarrior()
{
	ADragonAgeJourneysCharacter* avatar =
		(Cast<ADragonAgeJourneysCharacter>(GetCurrentPlayerController()->GetPawn()));

	if (avatar->GetCharRef()->classId != CLASS_WARRIOR)
	{
		avatar->GetCharRef()->classId = CLASS_WARRIOR;
		avatar->GetCharRef()->InitializeGameCharacter(avatar->GetCharRef()->classId, false);
		avatar->GetCharRef()->SetDefaultEquipmentTemplate();
		avatar->InitDefaultSkeletalMeshes();
		avatar->GetCharRef()->DefaultEquipment();
		CharGenUpdateSelection();
	}
}

void UUserWidgetCharGen::OnTapStart()
{
	//reset char gen anim just in case
	ADragonAgeJourneysCharacter* avatar =
		(Cast<ADragonAgeJourneysCharacter>(GetCurrentPlayerController()->GetPawn()));
	avatar->AnimInstance->SetCustom(false);

	GetMain()->CreationFinished();
}

void UUserWidgetCharGen::CharGenUpdateSelection()
{
	ADragonAgeJourneysCharacter* avatar =
		(Cast<ADragonAgeJourneysCharacter>(GetCurrentPlayerController()->GetPawn()));

	int32 nGender = avatar->GetCharRef()->gender;
	int32 nRace = avatar->GetCharRef()->GetRace();
	int32 nClass = avatar->GetCharRef()->classId;
	IconMage->SetBrushColor(FLinearColor(1, 1, 1, 1));

	//gender
	SelectFemale->SetVisibility(ESlateVisibility::Hidden);
	SelectMale->SetVisibility(ESlateVisibility::Hidden);
	if (nGender == GENDER_FEMALE)
	{
		SelectFemale->SetVisibility(ESlateVisibility::Visible);
		TextGender->SetText(FText::FromString("Female"));
	}
	else
	{
		SelectMale->SetVisibility(ESlateVisibility::Visible);
		TextGender->SetText(FText::FromString("Male"));
	}

	//race
	SelectDwarf->SetVisibility(ESlateVisibility::Hidden);
	SelectElf->SetVisibility(ESlateVisibility::Hidden);
	SelectHuman->SetVisibility(ESlateVisibility::Hidden);
	IconDwarfFemale->SetVisibility(ESlateVisibility::Hidden);
	IconDwarfFemale->SetBrushColor(FLinearColor(1, 1, 1, 1));
	IconDwarfMale->SetVisibility(ESlateVisibility::Hidden);
	IconDwarfMale->SetBrushColor(FLinearColor(1, 1, 1, 1));
	IconElfFemale->SetVisibility(ESlateVisibility::Hidden);
	IconElfMale->SetVisibility(ESlateVisibility::Hidden);
	IconHumanFemale->SetVisibility(ESlateVisibility::Hidden);
	IconHumanMale->SetVisibility(ESlateVisibility::Hidden);
	if (nGender == GENDER_FEMALE)
	{
		IconDwarfFemale->SetVisibility(ESlateVisibility::Visible);
		IconElfFemale->SetVisibility(ESlateVisibility::Visible);
		IconHumanFemale->SetVisibility(ESlateVisibility::Visible);
		if (nClass == CLASS_MAGE)
		{
			IconDwarfFemale->SetBrushColor(FLinearColor(1, 1, 1, 0.5));
		}
	}
	else
	{
		IconDwarfMale->SetVisibility(ESlateVisibility::Visible);
		IconElfMale->SetVisibility(ESlateVisibility::Visible);
		IconHumanMale->SetVisibility(ESlateVisibility::Visible);
		if (nClass == CLASS_MAGE)
		{
			IconDwarfMale->SetBrushColor(FLinearColor(1, 1, 1, 0.5));
		}
	}
	if (nRace == RACE_DWARF)
	{
		SelectDwarf->SetVisibility(ESlateVisibility::Visible);
		TextRace->SetText(FText::FromString("Dwarf"));
	}
	else if (nRace == RACE_ELF)
	{
		SelectElf->SetVisibility(ESlateVisibility::Visible);
		TextRace->SetText(FText::FromString("Elf"));
	}
	else if (nRace == RACE_HUMAN)
	{
		SelectHuman->SetVisibility(ESlateVisibility::Visible);
		TextRace->SetText(FText::FromString("Human"));
	}

	//class
	SelectMage->SetVisibility(ESlateVisibility::Hidden);
	SelectRogue->SetVisibility(ESlateVisibility::Hidden);
	SelectWarrior->SetVisibility(ESlateVisibility::Hidden);
	if (nRace == RACE_DWARF)
	{
		IconMage->SetBrushColor(FLinearColor(1, 1, 1, 0.5));
	}
	if (nClass == CLASS_MAGE)
	{
		SelectMage->SetVisibility(ESlateVisibility::Visible);
		TextClass->SetText(FText::FromString("Mage"));
	}
	else if (nClass == CLASS_ROGUE)
	{
		SelectRogue->SetVisibility(ESlateVisibility::Visible);
		TextClass->SetText(FText::FromString("Rogue"));
	}
	else if (nClass == CLASS_WARRIOR)
	{
		SelectWarrior->SetVisibility(ESlateVisibility::Visible);
		TextClass->SetText(FText::FromString("Warrior"));
	}
}

