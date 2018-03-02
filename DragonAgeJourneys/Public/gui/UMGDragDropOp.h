// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/DragDropOperation.h"
#include "UMGDragDropOp.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class DRAGONAGEJOURNEYS_API UUMGDragDropOp : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UUserWidget* WidgetToDrag = nullptr;
	FVector2D MouseOffset;
	
	
};
