#include "GraphSolverBestFirstSearch.h"
#include "heruistics.h"


using namespace std;


GraphSolverBestFirstSearch::GraphSolverBestFirstSearch(Graph& g) : nodes(g.getNodes()), edges(g.getEdges()),
	starts(g.getStarts()), target(g.getTarget()), priorityQueue(MinHeuristicComperator(heuristicCost))
{
	for (auto& n : nodes)
	{
		heuristicCost[n] = manhattan_distance(n, target);
	}

	solved = false;

	priorityQueue.push(starts[0]);
	starts[0]->setVisited(true);
}

void GraphSolverBestFirstSearch::solveIteration()
{
	if (solved || priorityQueue.empty())
		return;

	Node* cur = priorityQueue.top();
	priorityQueue.pop();
	cur->setIsVisiting(false);
	cur->setVisited(true);

	vector<Edge*>& edges = cur->getEdges();

	for (auto& e : edges)
	{
		Node* neighbor = e->getTo();
		
		if (neighbor->getVisited() || neighbor->getIsVisiting())
			continue;

		priorityQueue.push(neighbor);
		neighbor->setIsVisiting(true);
		neighbor->setParent(cur);

		if (neighbor == target)
		{
			solved = true;
			retorePath();
			return;
		}
	}
}


void GraphSolverBestFirstSearch::retorePath()
{
	Node* temp = target->getParent();
	while (temp != nullptr && temp != starts[0])
	{
		temp->setIsPath(true);
		temp = temp->getParent();
	}
}