// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EmbedList.h"
#include "EmbeddedAssets.generated.h"

UCLASS()
class UEmbeddedAssets : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
		UEmbedList* embed_list = nullptr;

private:
	virtual ~UEmbeddedAssets();

};



