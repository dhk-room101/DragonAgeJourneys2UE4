// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "STypes.h"
#include "Dijkstra.generated.h"

const int32 NO_CONNECTION = 99999999;

UCLASS()
class UDijkstra : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void InitializeDijkstra(TArray<FInt32Array> mapGrid, int32 nNode);
	void GetResults();
	void FindShortestPath();
	TArray<int32> FindShortestPathTo(int32 nStart, int32 nEnd);

	TArray<int32> distance;
	bool pathsFound = false;

private:
	virtual ~UDijkstra();

	void UpdateDistanceAndPrevious(int32 nUpdate);
	bool SomethingLeft(TArray<int32> arNodes);
	TArray<int32> NodesNotVisited(TArray<int32> arNodes);
	int32 FindBestPath(TArray<int32> arStart, TArray<int32> arEnd);

	TArray<int32> visited;
	TArray<int32> previousNode;
	TArray<int32> nodesLeft;
	TArray<FInt32Array> map;
	int32 startNode;
	int32 infiniteDistance;
	int32 numberOfNodes;
	int32 bestPath;
};



