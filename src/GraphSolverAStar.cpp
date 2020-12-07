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

		if (openSet.find(neighbor) == openSet.end())
			openSet.insert(neighbor);
		else if (currentNeighborCost >= costFromNodeToNeighbor)
			continue;

		/* if not continued meaning need to update maps costs */
		moveCost[neighbor] = costFromNodeToNeighbor;
		funcValues[neighbor] = moveCost[neighbor] + heuristicCost[neighbor];
		cameFrom[neighbor] = lowestF;
	}
}

static void printSet(string nodesName, set<Node*> nodes, map<Node*, double> mmap)
{
	cout << "=====================================" << endl;
	cout << nodesName << endl;
	for (auto& n : nodes)
		cout << n->getX() << ", " << n->getY() << ": " << mmap[n] << endl;
	cout << "=====================================" << endl;
}

void GraphSolverAStar::solveIteration()
{
	if (solved || openSet.empty())
		return;

	for (auto& n : nodes) {
		n->isClosed = false;
		n->isOpen = false;
	}

	for (auto& n : openSet)
		n->isOpen = true;

	for (auto& n : closedSet)
		n->isClosed = true;

	cout << "***************************************" << endl;
	printSet("moveCost on Open", openSet, moveCost);
	printSet("moveCost on Close", closedSet, moveCost);
	cout << "***************************************\n" << endl;



	Node* lowestF = findNeighborWithLowestFuncValue();
	openSet.erase(lowestF);
	closedSet.insert(lowestF);

	if (lowestF == target) {
		std::cout << "solved" << std::endl;
		solved = true;
		return;
	}

	calculateStepInPathFromCurrentNode(lowestF);
}

