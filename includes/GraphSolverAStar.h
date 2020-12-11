#ifndef __GRAPH_SOLVER_A_STAR__
#define __GRAPH_SOLVER_A_STAR__

#include "Graph.h"
#include "Solvable.h"
#include <map>
#include <set>

class GraphSolverAStar : public Solveable {
	std::vector<Node*>& nodes;
	std::vector<Edge*>& edges;
	std::vector<Node*>& starts;
	Node* target;

	// this map will show for each node its cost from the start
	std::map<Node*, double> moveCost; // g

	// this map will show the heuristic estimation for each node to the end
	std::map<Node*, double> heuristicCost; // h

	// heuristic + moveCost
	std::map<Node*, double> funcValues; // f = g + h

	std::set<Node*> openSet;
	std::set<Node*> closedSet;
	Node* findNeighborWithLowestFuncValue();
	void calculateStepInPathFromCurrentNode(Node* lowestF);

	void restorePath();
public:
	GraphSolverAStar(Graph& g);
	virtual void solveIteration() override;

};


#endif
