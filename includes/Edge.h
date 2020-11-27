#ifndef __EDGE__
#define __EDGE__
#include <vector>

class Node;

class Edge {
private:
	Node* from;
	Node* to;
	double weight;
	double calcWeight();

public:
	Edge(Node* from, Node* to);
	inline Node* getFrom() { return from; }
	inline Node* getTo() { return to; }
	void draw();

	static void drawEdges(std::vector<Edge>& edges);
};

#endif 
