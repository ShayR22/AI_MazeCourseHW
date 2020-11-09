#ifndef __NODE__
#define __NODE__
#include <vector>
#include "Edge.h"

class Node {
private:
	int x;
	int y;
	std::vector<Edge*> edges;

public:
	Node(int x, int y);
	Node(const Node& node);
	inline int getX() { return x; }
	inline int getY() { return y; }
	void addEdge(Edge& edge);
	std::vector<Node*> getNieghbors();
	void draw();
	
	static void drawPoints(std::vector<Node*>& nodes);
};

#endif