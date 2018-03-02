// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetFloatyText.h"
#include "Components/TextBlock.h"
#include "DragonAgeJourneys.h"

void UUserWidgetFloatyText::NativeConstruct()
{
	Super::NativeConstruct();

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("------ - Floaty Construct---- - "));

#endif // DEBUG

	Floaty = (UTextBlock*)GetWidgetFromName(TEXT("Floaty"));
}

