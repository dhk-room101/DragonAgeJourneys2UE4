#include "Dijkstra.h"
#include "Algo/Reverse.h"

UDijkstra::UDijkstra(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UDijkstra::InitializeDijkstra(TArray<FInt32Array> mapGrid, int32 nNode)
{
	infiniteDistance = NO_CONNECTION;
	startNode = nNode;
	map = mapGrid;
	numberOfNodes = map[0].arrInt32.Num();
	previousNode.SetNum(numberOfNodes);
	distance.SetNum(numberOfNodes);
	visited.SetNum(numberOfNodes);
	bestPath = 0;
	nodesLeft.Empty();
}

void UDijkstra::GetResults()
{
	int32 nIndex = 0;
	int32 nInt1 = 0;
	int32 nInt2 = 0;
	
	TArray<FInt32Array> int32Map;
	
	while (nIndex < numberOfNodes)
	{
		int32Map.SetNum(numberOfNodes);
		int32Map[nIndex].arrInt32.Empty();
		nInt1 = -1;
		nInt2 = nIndex;
		int32Map[nIndex].arrInt32.Add(nIndex);
		while (nInt1 != startNode)
		{
			int32Map[nIndex].arrInt32.Add(previousNode[nInt2]);
			nInt1 = previousNode[nInt2];
			nInt2 = previousNode[nInt2];
		}
		
		Algo::Reverse(int32Map[nIndex].arrInt32);
		
		UE_LOG(LogTemp, Warning, TEXT("---------------------------------------"));
		UE_LOG(LogTemp, Warning, TEXT("The shortest distance from the startNode: %i, to node %i : is -> %i"), startNode, nIndex, distance[nIndex]);
		UE_LOG(LogTemp, Warning, TEXT("The shortest path from the startNode: %i, to node %i is -> %i"), startNode, nIndex, int32Map[nIndex].arrInt32.Num());
		UE_LOG(LogTemp, Warning, TEXT("---------------------------------------"));
		
		++nIndex;
	}
}

void UDijkstra::FindShortestPath()
{
	int32 nIndex = 0;
	while (nIndex < numberOfNodes)
	{
		if (nIndex != startNode)
		{
			visited[nIndex] = 0;
			distance[nIndex] = map[startNode].arrInt32[nIndex];
		}
		else
		{
			visited[nIndex] = 1;
			distance[nIndex] = 0;
		}
		previousNode[nIndex] = startNode;
		++nIndex;
	}
	int32 nMax = 1000;
	int32 nCounter = 0;
	while (SomethingLeft(visited) && nCounter < nMax)
	{
		nodesLeft = NodesNotVisited(visited);
		bestPath = FindBestPath(distance, nodesLeft);
		UpdateDistanceAndPrevious(bestPath);
		visited[bestPath] = 1;
		++nCounter;
	}
	pathsFound = true;
}

TArray<int32> UDijkstra::FindShortestPathTo(int32 nStart, int32 nEnd)
{
	if (!pathsFound)
	{
		FindShortestPath();
	}
	
	TArray<int32> path;
	int32 nPreviousNode = -1;
	int32 nCurrentNode = nStart;
	
	path.Add(nStart);
	
	int32 nIndex = 1;
	
	while (nPreviousNode != startNode && nIndex < nEnd)
	{
		path.Add(previousNode[nCurrentNode]);
		nPreviousNode = previousNode[nCurrentNode];
		nCurrentNode = previousNode[nCurrentNode];
		++nIndex;
	}
	
	if (nIndex >= nEnd)
	{
		path.Empty();
		UE_LOG(LogTemp, Warning, TEXT("Dijkstra: no valid path found"));
	}
	else
	{
		Algo::Reverse(path);
	}
	
	return path;
}

UDijkstra::~UDijkstra()
{
}

void UDijkstra::UpdateDistanceAndPrevious(int32 nUpdate)
{
	int32 nIndex = 0;
	while (nIndex < numberOfNodes)
	{
		if (map[nUpdate].arrInt32[nIndex] != infiniteDistance || map[nUpdate].arrInt32[nIndex] == 0)
		{
			if (distance[nUpdate] + map[nUpdate].arrInt32[nIndex] < distance[nIndex])
			{
				distance[nIndex] = distance[nUpdate] + map[nUpdate].arrInt32[nIndex];
				previousNode[nIndex] = nUpdate;
			}
		}
		++nIndex;
	}
}

bool UDijkstra::SomethingLeft(TArray<int32> arNodes)
{
	int32 nIndex = 0;
	while (nIndex < numberOfNodes)
	{
		if (!arNodes[nIndex])
		{
			return true;
		}
		++nIndex;
	}
	return false;

}

TArray<int32> UDijkstra::NodesNotVisited(TArray<int32> arNodes)
{
	TArray<int32> arResult;
	int32 nIndex = 0;
	while (nIndex < numberOfNodes)
	{
		if (arNodes[nIndex] == 0)
		{
			arResult.Add(nIndex);
		}
		++nIndex;
	}
	return arResult;

}

int32 UDijkstra::FindBestPath(TArray<int32> arStart, TArray<int32> arEnd)
{
	int32 nNode = infiniteDistance;
	int32 nResult = 0;
	int32 nIndex = 0;
	
	while (nIndex < arEnd.Num())
	{
		if (arStart[arEnd[nIndex]] < nNode)
		{
			nNode = arStart[arEnd[nIndex]];
			nResult = arEnd[nIndex];
		}
		++nIndex;
	}
	
	return nResult;
}
