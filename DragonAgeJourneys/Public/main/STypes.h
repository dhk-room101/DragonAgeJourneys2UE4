#pragma once

#include <UnrealString.h>
#include <Array.h>
#include "Engine/DataTable.h"
#include "STypes.generated.h"

class AHex;
class UGameCharacter;

//creature skeletal meshes ID
const int32 MESHES_SKELETAL = 4;
const int32 MESH_HEAD = 0;
const int32 MESH_HAIR = 1;
const int32 MESH_EYES = 2;
const int32 MESH_ARM = 3;

//creature static meshes
const int32 MESHES_STATIC = 3;
const int32 MESH_RIGHT = 0;
const int32 MESH_LEFT = 1;
const int32 MESH_AMMO = 2;

UENUM()
enum class EUI : uint8
{
	INVALID,
	QUICKBAR,
	CONVERSATION,
	MAINMENU,
	CHARGEN,
	EXPLORE,
	COMBAT
};

UENUM()
enum class EWeaponStyle : uint8
{
	INVALID,
	SINGLE,
	DUAL,
	BOW,
	STAFF,
	TWOHANDS
};

UENUM()
enum class EHexMode : uint8
{
	INVALID, //0
	NORMAL,
	MOVE,
	SELECTED,
	TARGET,
	MOVEATTACK,
	RANGED,
	SHADOWED,
	SHOWPATH //DHK
};

UENUM(BlueprintType)
enum class ETable : uint8
{
	INVALID,
	MESH
};

USTRUCT()
struct FGameDataStruct
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<int32> ExperienceLevels;
	UPROPERTY()
		TMap<FName, int32> ExperienceClassAndXP;
	//TODO Quest
};

USTRUCT()
struct FEmbedListMember
{
	GENERATED_BODY()

		UPROPERTY()
		FName file = "";
	UPROPERTY()
		FName classname = "";
};

USTRUCT()
struct FMapEncounterStruct
{
	GENERATED_BODY()

		UPROPERTY()
		FString ID;
	UPROPERTY()
		int32 m1;
	UPROPERTY()
		int32 m2;
	UPROPERTY()
		int32 m3;
	UPROPERTY()
		int32 m4;
	UPROPERTY()
		int32 aggro_radius;
	UPROPERTY()
		int32 hex_id = -1;
};

USTRUCT()
struct FMapHexStruct
{
	GENERATED_BODY()

		UPROPERTY()
		int32 status;
	UPROPERTY()
		float xc;
	UPROPERTY()
		float yc;
	UPROPERTY()
		FString ground_tile;
	UPROPERTY()
		FString detail_tile;
	UPROPERTY()
		FString detail_tile2;
	UPROPERTY()
		FString detail_tile3;
	UPROPERTY()
		FString special_tile;
	UPROPERTY()
		int32 special_type;
	UPROPERTY()
		FString special_id;
	UPROPERTY()
		int32 encounter_id = -1;
	UPROPERTY()
		bool obstructed;
};

USTRUCT()
struct FMapData
{
	GENERATED_BODY()

		UPROPERTY()
		FString checksum;
	UPROPERTY()
		int32 version;
	UPROPERTY()
		FString map_ID;
	UPROPERTY()
		FString tileset;
	UPROPERTY()
		TArray<FMapHexStruct> hex_list;
	UPROPERTY()
		TArray<FMapEncounterStruct> encounter_list;
};

USTRUCT()
struct FHexArray
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<AHex*> arrHex;
};

USTRUCT()
struct FInt32Array
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<int32> arrInt32;
};

USTRUCT()
struct FDamageSource
{
	GENERATED_BODY()

		UPROPERTY()
		UGameCharacter* charAttacker = nullptr;
	int32 nAmount;
};

USTRUCT(BlueprintType)
struct FMeshTable : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table")
		FString MeshString;
};