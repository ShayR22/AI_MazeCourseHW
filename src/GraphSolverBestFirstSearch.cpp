#include "GraphSolverBestFirstSearch.h"


using namespace std;


GraphSolverBestFirstSearch::GraphSolverBestFirstSearch(Graph& g) : nodes(g.getNodes()), edges(g.getEdges()),
	starts(g.getStarts()), target(g.getTarget())
{
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

	if (cur == target)
	{
		solved = true;
		return;
	}




}
