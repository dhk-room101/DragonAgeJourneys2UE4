// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetFloatyText.generated.h"

class ADragonAgeJourneysCharacter;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DRAGONAGEJOURNEYS_API UUserWidgetFloatyText : public UUserWidget
{
	GENERATED_BODY()

public:
	UTextBlock* Floaty;
	ADragonAgeJourneysCharacter* avatar = nullptr;
	int32 nStyle = 0;
	int32 nColor = 0xFFFFFF;//white
	float fDuration = 1.f;
	float fTimestamp;
	float PosX = -1;
	FVector vPosition = FVector(0.f);
	
protected:
	virtual void NativeConstruct() override;
	
};
