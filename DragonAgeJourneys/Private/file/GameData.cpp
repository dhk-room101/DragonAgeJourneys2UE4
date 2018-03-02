#include "GameData.h"
#include "Scripting.h"
#include <Paths.h>
#include "EmbedList.h"
#include <XmlFile.h>

UGameData::UGameData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGameData::LoadData()
{
	ParseGameDataXML(GetAssets()->embed_list->EMBED_GAMEDATA_XML);

	GameDataLoad.Broadcast();
}

UGameData::~UGameData()
{
}

void UGameData::ParseGameDataXML(FName fGameDataXML)
{
	//TODO Quests
	FString sFile = GetFileFromClassName(fGameDataXML).ToString();
	FString sGameDataXML = FPaths::GameDevelopersDir() + "Source/" + sFile;

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("Parsing XML %s"), *sGameDataXML);
#endif // DEBUG

	const FXmlFile fileGameData(sGameDataXML);
	const FXmlNode* fileGameDataXmlNode = fileGameData.GetRootNode();
	const TArray<FXmlNode*> gameDataLists = fileGameDataXmlNode->GetChildrenNodes();

	FXmlNode* CharacterNode = gameDataLists[0];
	const TArray<FXmlNode*> CharacterList = CharacterNode->GetChildrenNodes();

	FXmlNode* experienceLevelsNode = CharacterList[0];
	const TArray<FXmlNode*> experienceLevelsNodeChildren = experienceLevelsNode->GetChildrenNodes();

	for (int32 i = 0; i < experienceLevelsNodeChildren.Num(); i++)
	{
		FXmlNode* child = experienceLevelsNodeChildren[i];
		GameDataXML.ExperienceLevels.Add(FCString::Atoi(*child->GetContent()));
	}

	FXmlNode* ClassesNode = gameDataLists[1];
	const TArray<FXmlNode*> ClassesList = ClassesNode->GetChildrenNodes();

	for (int32 i = 0; i < ClassesList.Num(); i++)
	{
		FXmlNode* child = ClassesList[i];
		FName fName = FName(*child->GetAttribute("name"));
		FXmlNode* xpNode = child->GetChildrenNodes()[0];
		int32 nXP = FCString::Atoi(*xpNode->GetContent());
		GameDataXML.ExperienceClassAndXP.Add(fName, nXP);
	}

	//TODO initialize first character
}
