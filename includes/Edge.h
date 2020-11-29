#ifndef __EDGE__
#define __EDGE__
#include <vector>
#include "Drawable.h"

class Node;

class Edge : public Drawable {
private:
	Node* from;
	Node* to;
	double weight;
	double calcWeight();

public:
	Edge(Node* from, Node* to);
	inline Node* getFrom() { return from; }
	inline Node* getTo() { return to; }
	virtual void draw() override;

	static void drawEdges(std::vector<Edge>& edges);
};

#endif 
