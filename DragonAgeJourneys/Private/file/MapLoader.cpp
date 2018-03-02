#include "MapLoader.h"
#include "DragonAgeJourneys.h"
#include "Scripting.h"
#include "Map.h"
#include <Paths.h>
#include <XmlFile.h>
#include "Group.h"

UMapLoader::UMapLoader(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#ifdef DEBUG
	//GetAssets();
	UE_LOG(LogTemp, Warning, TEXT("%s: New MapLoader created!"), *GetClass()->GetName());
#endif // DEBUG
}

void UMapLoader::LoadMap(FString areaString)
{
	ParseMapDataXML(FName(*areaString));

	MapDataLoad.Broadcast();
}

UGroup* UMapLoader::GetEncounter(FString sEncounter)
{
	FMapEncounterStruct encounter;
	TArray<int32> tChar;
	UGroup* group = NewObject<UGroup>();

	if (last_map.map_ID == "")
	{
		return group;
	}

	for (int32 i = 0; i < last_map.encounter_list.Num(); i++)
	{
		encounter = last_map.encounter_list[i];
		if (encounter.ID == sEncounter)
		{
			if (encounter.m1 != 0)
			{
				tChar.Add(encounter.m1);
			}
			if (encounter.m2 != 0)
			{
				tChar.Add(encounter.m2);
			}
			if (encounter.m3 != 0)
			{
				tChar.Add(encounter.m3);
			}
			if (encounter.m4 != 0)
			{
				tChar.Add(encounter.m4);
			}
			
			group->InitializeGroup(tChar);
			group->aggroRadius = encounter.aggro_radius;
			group->id = sEncounter; //DHK
			
			return group;
		}
	}
	
	return group;
}

UMapLoader::~UMapLoader()
{
}

void UMapLoader::ParseMapDataXML(FName fMapDataXML)
{
	FString sMapDataXML = FPaths::GameDevelopersDir() + "Source/Maps/" + fMapDataXML.ToString() + ".map";

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("Parsing XML %s"), *sMapDataXML);
#endif  DEBUG

	const FXmlFile fileMap(sMapDataXML);

	const FXmlNode* mapDataXmlNode = fileMap.GetRootNode();

	const TArray<FXmlNode*> mapLists = mapDataXmlNode->GetChildrenNodes();

	FXmlNode* mapHexListNode = mapLists[0];
	const TArray<FXmlNode*> mapHexListChildren = mapHexListNode->GetChildrenNodes();

	for (int32 i = 0; i < mapHexListChildren.Num(); i++)
	{
		FXmlNode* mapHex = mapHexListChildren[i];

		FMapHexStruct mapHexStruct;
		mapHexStruct.status = FCString::Atoi(*mapHex->GetAttribute("status"));
		mapHexStruct.xc = FCString::Atof(*mapHex->GetAttribute("xc"));
		mapHexStruct.yc = FCString::Atof(*mapHex->GetAttribute("yc"));
		mapHexStruct.ground_tile = mapHex->GetAttribute("ground_tile");
		mapHexStruct.detail_tile = mapHex->GetAttribute("detail_tile");
		mapHexStruct.detail_tile2 = mapHex->GetAttribute("detail_tile2");
		mapHexStruct.detail_tile3 = mapHex->GetAttribute("detail_tile3");
		mapHexStruct.special_tile = mapHex->GetAttribute("special_tile");
		mapHexStruct.special_type = FCString::Atoi(*mapHex->GetAttribute("special_type"));
		mapHexStruct.special_id = mapHex->GetAttribute("special_id");
		mapHexStruct.encounter_id = FCString::Atoi(*mapHex->GetAttribute("encounter_id"));
		FString sBool = mapHex->GetAttribute("obstructed");
		mapHexStruct.obstructed = (sBool == "true") ? true : false;

		GetMain()->LevelMap->map_data.hex_list.Add(mapHexStruct);
	}

	FXmlNode* mapEncounterListNode = mapLists[1];
	const TArray<FXmlNode*> mapEncounterListChildren = mapEncounterListNode->GetChildrenNodes();

	for (int32 i = 0; i < mapEncounterListChildren.Num(); i++)
	{
		FXmlNode* mapEncounter = mapEncounterListChildren[i];

		FMapEncounterStruct mapEncounterStruct;
		mapEncounterStruct.ID = mapEncounter->GetAttribute("ID");
		mapEncounterStruct.m1 = FCString::Atoi(*mapEncounter->GetAttribute("m1"));
		mapEncounterStruct.m2 = FCString::Atoi(*mapEncounter->GetAttribute("m2"));
		mapEncounterStruct.m3 = FCString::Atoi(*mapEncounter->GetAttribute("m3"));
		mapEncounterStruct.m4 = FCString::Atoi(*mapEncounter->GetAttribute("m4"));
		mapEncounterStruct.aggro_radius = FCString::Atoi(*mapEncounter->GetAttribute("aggro_radius"));
		mapEncounterStruct.hex_id = FCString::Atoi(*mapEncounter->GetAttribute("hex_id"));

		GetMain()->LevelMap->map_data.encounter_list.Add(mapEncounterStruct);
	}

	GetMain()->LevelMap->map_data.checksum = mapDataXmlNode->GetAttribute("checksum");
	GetMain()->LevelMap->map_data.version = FCString::Atoi(*mapDataXmlNode->GetAttribute("version"));
	GetMain()->LevelMap->map_data.map_ID = mapDataXmlNode->GetAttribute("map_ID");
	GetMain()->LevelMap->map_data.tileset = mapDataXmlNode->GetAttribute("tileset");
}
