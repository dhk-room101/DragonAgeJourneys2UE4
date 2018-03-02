// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "DragonAgeJourneysMeshDataAsset.generated.h"

class USkeletalMesh;
class UStaticMesh;

//Holds the information about a single character "part"
USTRUCT()
struct FSkeletalMeshInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "DATA", meta = (ToolTip = "The Mesh ID"))
		int32 MeshID;

	UPROPERTY(EditAnywhere, Category = "DATA", meta = (ToolTip = "Related Asset"))
		TAssetPtr<USkeletalMesh> MeshResource;

	FSkeletalMeshInfo()
	{
		MeshID = 0;
		MeshResource = FStringAssetReference("");
	}
};

USTRUCT()
struct FStaticMeshInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "DATA", meta = (ToolTip = "The Mesh ID"))
		int32 MeshID;

	UPROPERTY(EditAnywhere, Category = "DATA", meta = (ToolTip = "Related Asset"))
		TAssetPtr<UStaticMesh> MeshResource;

	FStaticMeshInfo()
	{
		MeshID = 0;
		MeshResource = FStringAssetReference("");
	}
};

/**
 *
 */
UCLASS(Blueprintable)
class DRAGONAGEJOURNEYS_API UDragonAgeJourneysMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UDragonAgeJourneysMeshDataAsset();

	//Exposes the array as editable on editor
	UPROPERTY(EditAnywhere, Category = "Model List", meta = (ToolTip = "Asset Info"))
		TArray<FSkeletalMeshInfo> SkeletalMeshList;

	UPROPERTY(EditAnywhere, Category = "Model List", meta = (ToolTip = "Asset Info"))
		TArray<FStaticMeshInfo> StaticMeshList;
};
