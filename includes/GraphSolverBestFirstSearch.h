#ifndef __GRAPH_SOLVER_BEST_FIRST_SEARCH__
#define __GRAPH_SOLVER_BEST_FIRST_SEARCH__

#include "Graph.h"
#include "Solvable.h"
#include <queue>
#include <vector>

class GraphSolverBestFirstSearch : public Solveable {

private:
	class MinHeuristicComperator {
	public:
		bool operator()(Node* n1, Node* n2)
		{
			return n1->getHeuristic() < n2->getHeuristic();
		}
	};

	std::vector<Node*>& nodes;
	std::vector<Edge*>& edges;
	std::vector<Node*>& starts;
	Node* target;

	std::priority_queue<Node*, std::vector<Node*>, MinHeuristicComperator > priorityQueue;

public:
	GraphSolverBestFirstSearch(Graph& g);

	virtual void solveIteration() override;
	
};


#endif
