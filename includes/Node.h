#ifndef __NODE__
#define __NODE__
#include <vector>
#include "Drawable.h"
#include "Edge.h"

class Node : public Drawable {
private:
	int x;
	int y;
	bool visited;
	std::vector<Edge*> edges;

	double heuristic;

public:
	Node(int x, int y);
	Node(const Node& node);
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline bool getVisited() { return visited; }
	inline void setVisited(bool v) { visited = v; }
	void addEdge(Edge& edge);
	void setHeuristic(double h);
	inline double getHeuristic() { return heuristic; }
	std::vector<Node*> getNieghbors();
	inline std::vector<Edge*>& getEdges() { return edges; }
	virtual void draw() override;
	
	static void drawPoints(std::vector<Node*>& nodes);

	bool isOpen;
	bool isClosed;

};

#endif