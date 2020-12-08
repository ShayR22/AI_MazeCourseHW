#ifndef __GRAPH__
#define __GRAPH__

#include <vector>
#include "Drawable.h"
#include "Maze.h"
#include "Node.h"
#include "Edge.h"

class Graph : public Drawable{
private:
	typedef std::vector<std::vector<Node*>> node_mat;
	std::vector<Node*> nodes;
	std::vector<Edge*> edges;
	std::vector<Node*> starts;
	Node* target;


	void initalizeNodeMat(node_mat& nm, cell_mat& cm);
	void setStartsAndTarget(node_mat& nm, Maze& maze);
	bool isAbove(int i, int j, node_mat& nm, cell_mat &cm);
	bool isBelow(int i, int j, cell_mat& mat);
	bool isDeadEnd(int i, int j, cell_mat& mat);
	bool isSideways(int i, int j, cell_mat& mat);
	bool toCreateNode(int i, int j, node_mat& nm, cell_mat& cm);
	void findCreateEdge(int i, int j, node_mat& nm, cell_mat& cm);
	void removeNoWhereToGo() noexcept(false);
	void setHeuristics();
public:
	Graph(Maze& maze);
	virtual void draw() override;
	~Graph();

	inline std::vector<Node*>& getNodes() { return nodes; }
	inline std::vector<Edge*>& getEdges() { return edges; }
	inline std::vector<Node*>& getStarts() { return starts; }
	inline Node* getTarget() { return target; }
};

#endif /* __GRAPH__ */

