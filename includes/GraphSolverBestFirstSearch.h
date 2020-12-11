#ifndef __GRAPH_SOLVER_BEST_FIRST_SEARCH__
#define __GRAPH_SOLVER_BEST_FIRST_SEARCH__

#include "Graph.h"
#include "Solvable.h"
#include <queue>
#include <vector>
#include <map>
#include <iostream>

class GraphSolverBestFirstSearch : public Solveable {

private:
	// this map will show the heuristic estimation for each node to the end
	std::map<Node*, double> heuristicCost; // h

	class MinHeuristicComperator {
	private:
		std::map<Node*, double>& heuristicCost; // h
	public:

		MinHeuristicComperator(std::map<Node*, double>& heuristicCost): heuristicCost(heuristicCost) {}

		bool operator()(Node* n1, Node* n2)
		{
			/*priority que works with max element, thus, return max insted min
			, for further explaintions https://en.cppreference.com/w/cpp/container/priority_queue */
			return heuristicCost[n1] > heuristicCost[n2];
		}
	};

	std::vector<Node*>& nodes;
	std::vector<Edge*>& edges;
	std::vector<Node*>& starts;
	Node* target;

	std::priority_queue<Node*, std::vector<Node*>, MinHeuristicComperator> priorityQueue;

	void retorePath();

public:
	GraphSolverBestFirstSearch(Graph& g);
	virtual void solveIteration() override;
	
};


#endif
