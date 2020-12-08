#include <limits.h>
#include <vector>
#include "GraphSolverAStar.h"
#include "heruistics.h"
#include "Edge.h"
#include "Node.h"

using namespace std;

GraphSolverAStar::GraphSolverAStar(Graph& g) : nodes(g.getNodes()), edges(g.getEdges()),
	starts(g.getStarts()), target(g.getTarget())
{
	solved = false;
	for (auto& n : nodes) {
		double h = manhattan_distance(n, target);
		heuristicCost[n] = h;

		moveCost[n] = INT_MAX;
		funcValues[n] = INT_MAX;
	}

	Node* start = starts[0];
	// set cost to get to start from start to 0
	moveCost[start] = 0;
	funcValues[start] = heuristicCost[start] + moveCost[start];

	openSet.insert(start);
}

Node* GraphSolverAStar::findNeighborWithLowestFuncValue()
{
	/* add + 1 for edge case in start where all values are INT_MAX */
	double minVal = (double)INT_MAX + 1;
	Node* lowestNeighborF = nullptr;

	for (auto& n : openSet) {
		double curfVal = funcValues[n];
		if (curfVal < minVal) {
			minVal = curfVal;
			lowestNeighborF = n;
		}
	}

	return lowestNeighborF;
}

void GraphSolverAStar::calculateStepInPathFromCurrentNode(Node* lowestF)
{
	vector<Edge*>& edges = lowestF->getEdges();
	for (auto& e : edges) {
		Node* neighbor = e->getTo();
		if (closedSet.find(neighbor) != closedSet.end())
			continue;

		double currentNeighborCost = moveCost[neighbor];
		double costFromNodeToNeighbor = moveCost[lowestF] + e->getWeight();

		if (openSet.find(neighbor) == openSet.end()) {
			openSet.insert(neighbor);
			neighbor->setIsVisiting(true);
		}
		else if (currentNeighborCost < costFromNodeToNeighbor) {
			continue;
		}

		/* if not continued meaning need to update maps costs */
		moveCost[neighbor] = costFromNodeToNeighbor;
		funcValues[neighbor] = moveCost[neighbor] + heuristicCost[neighbor];
		neighbor->setParent(lowestF);
	}
}

void GraphSolverAStar::restorePath()
{
	Node* temp = target->getParent();
	while (temp != nullptr && temp != starts[0])
	{
		temp->setIsPath(true);
		temp = temp->getParent();
	}
}

void GraphSolverAStar::solveIteration()
{

	if (solved || openSet.empty())
		return;
	
	Node* lowestF = findNeighborWithLowestFuncValue();
	openSet.erase(lowestF);
	lowestF->setIsVisiting(false);
	closedSet.insert(lowestF);
	lowestF->setVisited(true);

	if (lowestF == target) {
		std::cout << "solved" << std::endl;
		solved = true;
		restorePath();
		return;
	}

	calculateStepInPathFromCurrentNode(lowestF);
}


