#include "MapHex.h"
#include "Group.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "STypes.h"
#include "Scripting.h"
#include "DragonMain.h"
#include "EnemyPosition.h"
#include "NeutralPosition.h"
#include "Party.h"
#include "Map.h"

AMapHex::AMapHex(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AMapHex::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> Components;
	this->GetComponents<UStaticMeshComponent>(Components);

	UStaticMeshComponent* StaticMeshComponent = Components[0];
}

void AMapHex::AddNeighbour(AMapHex* aNeighbourHex)
{
	if (!neighbours.Contains(aNeighbourHex))
	{
		//UE_LOG(LogTemp, Warning, TEXT("adding %s as neighbour of %s"), *aNeighbourHex->GetName(), *this->GetName());
		neighbours.Add(aNeighbourHex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s already added as neighbour of %s"), *aNeighbourHex->GetName(), *this->GetName());
	}
}

void AMapHex::SetHexVisible(int32 nFlag)
{
	if (nFlag != 0)
	{
		TArray<UStaticMeshComponent*> Components;
		this->GetComponents<UStaticMeshComponent>(Components);

		UStaticMeshComponent* StaticMeshComponent = Components[0];
		UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
		UMaterialInterface* inst = StaticMesh->GetMaterial(0);

		//TODO SetDiffuseBoost, currently no difference
		if (nFlag == SEEN_FLAG || nFlag == VISITED_FLAG)
		{
			this->SetActorHiddenInGame(false);
			inst->SetDiffuseBoost(0.35f);
		}
		else if (nFlag == HIGHLIGHT_FLAG)
		{
			this->SetActorHiddenInGame(false);
			inst->SetDiffuseBoost(1.f);
		}
	}
	else
	{
		this->SetActorHiddenInGame(true);
	}
}

void AMapHex::MapHexClicked()
{
	//TODO return if in dialog or open interface
	/*if (packages.dialogue.Conversation.current || dm.main.hasAnyNavPagesOpen())
	{
		return;
	}*/
	if (npc && npc->active)
	{
		GetMain()->LevelMap->conversationPendingHex = this;
		GetMain()->LevelMap->RequestTravelAdjacentTo(this, 10);
		UE_LOG(LogTemp, Warning, TEXT("set conversation pending hex to %s"), *locationName);
		GetMain()->LevelMap->battlePendingHex = nullptr;
		return;
	}

	GetMain()->LevelMap->conversationPendingHex = nullptr;

	if (enemyGroup)
	{
		//if (enemyGroup && enemyGroup->id.Contains("tut2") >= 0)	return;

		GetMain()->LevelMap->battlePendingHex = this;
		GetMain()->LevelMap->RequestTravelAdjacentTo(this, 10);
		UE_LOG(LogTemp, Warning, TEXT("set battle pending hex to %s"), *this->GetName() /**locationName*/);
		GetMain()->LevelMap->conversationPendingHex = nullptr;
		return;
	}

	GetMain()->LevelMap->battlePendingHex = nullptr;

	if (obstructed)
	{
		return;
	}

	if (GetMain()->LevelMap->suspendInput)
	{
		return;
	}

	if (GetMain()->LevelMap)
	{
		if (this == GetMain()->LevelMap->currentLocation)
		{
			return;
		}
		GetMain()->LevelMap->RequestTravelTo(this, 10);
	}

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("clicked on %s at pos %s"), *this->GetName(), *this->GetActorLocation().ToString());
#endif // DEBUG

}

bool AMapHex::AddEnemies(FMapHexStruct mapHexStruct, FMapEncounterStruct encounterStruct)
{
	enemyGroupId = encounterStruct.ID;

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("MapHex: Adding enemies with id %s"), *enemyGroupId);
#endif // DEBUG

	if (enemyGroupId != ""
		&& (!GetParty()->enemiesDefeated.Contains(enemyGroupId)
			|| GetParty()->enemiesDefeated[enemyGroupId] == false))
	{
		enemyGroup = GetParty()->GetGroup()->GenerateGroup(enemyGroupId);

#ifdef DEBUG
		UE_LOG(LogTemp, Warning, TEXT("adding enemy group!"));
#endif // DEBUG

		if (enemyGroup != nullptr)
		{
			obstructed = true;
			enemyGroup->encounter_data = encounterStruct;
			enemyGroup->mapHex = this;
			enemyGroup->pursuitHex = this;
			enemyGroup->groupPosition = NewObject<UEnemyPosition>();
			enemyGroup->groupPosition->X = mapHexStruct.xc;
			enemyGroup->groupPosition->Y = mapHexStruct.yc;
			GetMain()->LevelMap->AddEnemyGroup(enemyGroup);
			return true;
		}

#ifdef DEBUG
		UE_LOG(LogTemp, Warning, TEXT("!!WARNING!! - MapHex.addEnemies() - failed to create enemy group: "), *enemyGroupId);
#endif // DEBUG

	}

	return false;
}

bool AMapHex::AdjacentTo(AMapHex* adjHex)
{
	for (int32 i = 0; i < neighbours.Num(); i++)
	{
		AMapHex* hex = neighbours[i];
		if (hex == adjHex)
		{
			return true;
		}
	}
	
	return false;
}

FString AMapHex::Visit(bool bMoving)
{
	FString sHexType = "none";
	visited = true;
	seen = true;
	if (enemyGroup && !enemyGroup->GetDefeated() &&
		enemyGroup->pursuitHex == enemyGroup->mapHex)
	{
		sHexType = "battle";
	}
	else if (type == "conversation")
	{
		sHexType = "conversation";
	}
	if (!bMoving)
	{
		if (type == "transition")
			sHexType = "transition";
		else if (type.Contains("loot") &&
			GetParty()->hexesLooted[locationName] == false)
			sHexType = "loot";
		else if (type.Contains("recovery") &&
			GetParty()->recoveryUsed[locationName] == false)
			sHexType = "recovery";
		else if (type == "shop")
			sHexType = "shop";
	}
	return sHexType;
}
