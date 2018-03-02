// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetMainMenu.h"
#include <TimerManager.h>
#include "DragonAgeJourneys.h"
#include "DragonAgeJourneysPlayerController.h"
#include "Scripting.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UUserWidgetMainMenu::SelectDefaultButton()
{
	nTouch = 1;
	if (Text0->GetVisibility() == ESlateVisibility::Visible)
		nButton = 0;
	else nButton = 1;
	GetStage()->GetFirstPlayerController()->GetWorldTimerManager().SetTimer(tTouch, this, &UUserWidgetMainMenu::CheckTap, 0.25, false);
}

void UUserWidgetMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("------ - Main Menu Construct---- - "));

#endif // DEBUG

	Border0 = (UBorder*)GetWidgetFromName(TEXT("Border0"));
	Border0->SetVisibility(ESlateVisibility::Hidden);

	Border1 = (UBorder*)GetWidgetFromName(TEXT("Border1"));
	Border1->SetVisibility(ESlateVisibility::Hidden);

	Border2 = (UBorder*)GetWidgetFromName(TEXT("Border2"));
	Border2->SetVisibility(ESlateVisibility::Hidden);

	Border3 = (UBorder*)GetWidgetFromName(TEXT("Border3"));
	Border3->SetVisibility(ESlateVisibility::Hidden);

	Border4 = (UBorder*)GetWidgetFromName(TEXT("Border4"));
	Border4->SetVisibility(ESlateVisibility::Hidden);

	Border5 = (UBorder*)GetWidgetFromName(TEXT("Border5"));
	Border5->SetVisibility(ESlateVisibility::Hidden);

	//Continue
	Text0 = (UTextBlock*)GetWidgetFromName(TEXT("Text0"));
	Text0->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));
	Text0->SetVisibility(ESlateVisibility::Hidden);

	//NewGame
	Text1 = (UTextBlock*)GetWidgetFromName(TEXT("Text1"));
	Text1->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));

	//LoadGame
	Text2 = (UTextBlock*)GetWidgetFromName(TEXT("Text2"));
	Text2->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));

	//Options
	Text3 = (UTextBlock*)GetWidgetFromName(TEXT("Text3"));
	Text3->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));

	//Credits
	Text4 = (UTextBlock*)GetWidgetFromName(TEXT("Text4"));
	Text4->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));

	//Quit
	Text5 = (UTextBlock*)GetWidgetFromName(TEXT("Text5"));
	Text5->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));

	//Continue
	Button0 = (UButton*)GetWidgetFromName(TEXT("Button0"));
	Button0->OnClicked.AddDynamic(this, &UUserWidgetMainMenu::OnTap0);

	//NewGame
	Button1 = (UButton*)GetWidgetFromName(TEXT("Button1"));
	Button1->OnClicked.AddDynamic(this, &UUserWidgetMainMenu::OnTap1);

	//LoadGame
	Button2 = (UButton*)GetWidgetFromName(TEXT("Button2"));
	Button2->OnClicked.AddDynamic(this, &UUserWidgetMainMenu::OnTap2);

	//Options
	Button3 = (UButton*)GetWidgetFromName(TEXT("Button3"));
	Button3->OnClicked.AddDynamic(this, &UUserWidgetMainMenu::OnTap3);

	//Credits
	Button4 = (UButton*)GetWidgetFromName(TEXT("Button4"));
	Button4->OnClicked.AddDynamic(this, &UUserWidgetMainMenu::OnTap4);

	//Quit
	Button5 = (UButton*)GetWidgetFromName(TEXT("Button5"));
	Button5->OnClicked.AddDynamic(this, &UUserWidgetMainMenu::OnTap5);
}

void UUserWidgetMainMenu::CheckTap()
{
	if (nTouch == 1)
		UE_LOG(LogTemp, Warning, TEXT("tap on %i"), nButton);

	ResetButtons();

	switch (nButton)
	{
	case 0:
	{
		Text0->SetColorAndOpacity(FLinearColor(0.99, 0.99, 0.99, 1));
		if (Text0->GetVisibility() != ESlateVisibility::Hidden)
			Border0->SetVisibility(ESlateVisibility::Visible);

		if (nTouch > 1)
			UE_LOG(LogTemp, Warning, TEXT("double tap on Continue"));

		break;
	}
	case 1:
	{
		Text1->SetColorAndOpacity(FLinearColor(0.99, 0.99, 0.99, 1));
		if (Text1->GetVisibility() != ESlateVisibility::Hidden)
			Border1->SetVisibility(ESlateVisibility::Visible);

		if (nTouch > 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("double tap on New Game"));
			GetCurrentPlayerController()->SetCurrentGameMode(EUI::CHARGEN);
		}

		break;
	}
	case 2:
	{
		Text2->SetColorAndOpacity(FLinearColor(0.99, 0.99, 0.99, 1));
		if (Text2->GetVisibility() != ESlateVisibility::Hidden)
			Border2->SetVisibility(ESlateVisibility::Visible);

		if (nTouch > 1)
			UE_LOG(LogTemp, Warning, TEXT("double tap on Load"));

		break;
	}
	case 3:
	{
		Text3->SetColorAndOpacity(FLinearColor(0.99, 0.99, 0.99, 1));
		if (Text3->GetVisibility() != ESlateVisibility::Hidden)
			Border3->SetVisibility(ESlateVisibility::Visible);

		if (nTouch > 1)
			UE_LOG(LogTemp, Warning, TEXT("double tap on Options"));

		break;
	}
	case 4:
	{
		Text4->SetColorAndOpacity(FLinearColor(0.99, 0.99, 0.99, 1));
		if (Text4->GetVisibility() != ESlateVisibility::Hidden)
			Border4->SetVisibility(ESlateVisibility::Visible);

		if (nTouch > 1)
			UE_LOG(LogTemp, Warning, TEXT("double tap on Credits"));

		break;
	}
	case 5:
	{
		Text5->SetColorAndOpacity(FLinearColor(0.99, 0.99, 0.99, 1));
		if (Text5->GetVisibility() != ESlateVisibility::Hidden)
			Border5->SetVisibility(ESlateVisibility::Visible);

		if (nTouch > 1)
			UE_LOG(LogTemp, Warning, TEXT("double tap on Quit"));

		break;
	}
	default:
		break;
	}


	nTouch = 0;
	nButton = -1;
}

void UUserWidgetMainMenu::OnTap0()
{
	if (nTouch == 0)
	{
		nTouch++;
		nButton = 0;
		GetStage()->GetFirstPlayerController()->GetWorldTimerManager().SetTimer(tTouch, this, &UUserWidgetMainMenu::CheckTap, 0.25, false);
	}
	else nTouch++;
}

void UUserWidgetMainMenu::OnTap1()
{
	if (nTouch == 0)
	{
		nTouch++;
		nButton = 1;
		GetStage()->GetFirstPlayerController()->GetWorldTimerManager().SetTimer(tTouch, this, &UUserWidgetMainMenu::CheckTap, 0.25, false);
	}
	else nTouch++;
}

void UUserWidgetMainMenu::OnTap2()
{
	if (nTouch == 0)
	{
		nTouch++;
		nButton = 2;
		GetStage()->GetFirstPlayerController()->GetWorldTimerManager().SetTimer(tTouch, this, &UUserWidgetMainMenu::CheckTap, 0.25, false);
	}
	else nTouch++;
}

void UUserWidgetMainMenu::OnTap3()
{
	if (nTouch == 0)
	{
		nTouch++;
		nButton = 3;
		GetStage()->GetFirstPlayerController()->GetWorldTimerManager().SetTimer(tTouch, this, &UUserWidgetMainMenu::CheckTap, 0.25, false);
	}
	else nTouch++;
}

void UUserWidgetMainMenu::OnTap4()
{
	if (nTouch == 0)
	{
		nTouch++;
		nButton = 4;
		GetStage()->GetFirstPlayerController()->GetWorldTimerManager().SetTimer(tTouch, this, &UUserWidgetMainMenu::CheckTap, 0.25, false);
	}
	else nTouch++;
}

void UUserWidgetMainMenu::OnTap5()
{
	if (nTouch == 0)
	{
		nTouch++;
		nButton = 5;
		GetStage()->GetFirstPlayerController()->GetWorldTimerManager().SetTimer(tTouch, this, &UUserWidgetMainMenu::CheckTap, 0.25, false);
	}
	else nTouch++;
}

void UUserWidgetMainMenu::ResetButtons()
{
	Border0->SetVisibility(ESlateVisibility::Hidden);
	Text0->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));
	Border1->SetVisibility(ESlateVisibility::Hidden);
	Text1->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));
	Border2->SetVisibility(ESlateVisibility::Hidden);
	Text2->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));
	Border3->SetVisibility(ESlateVisibility::Hidden);
	Text3->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));
	Border4->SetVisibility(ESlateVisibility::Hidden);
	Text4->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));
	Border5->SetVisibility(ESlateVisibility::Hidden);
	Text5->SetColorAndOpacity(FLinearColor(0.58, 0.58, 0.58, 1));
}
