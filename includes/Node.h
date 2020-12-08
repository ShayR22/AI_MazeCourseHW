#ifndef __NODE__
#define __NODE__
#include <vector>
#include "Drawable.h"
#include "Edge.h"

class Node : public Drawable {
private:
	int x;
	int y;
	bool isVisiting;
	bool visited;
	bool isPath;
	Node* parent;
	std::vector<Edge*> edges;

	double heuristic;

public:
	Node(int x, int y);
	Node(const Node& node);
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline bool getIsVisiting() { return isVisiting; }
	inline void setIsVisiting(bool v) { isVisiting = v; }
	inline bool getVisited() { return visited; }
	inline void setVisited(bool v) { visited = v; }
	inline bool getIsPath() { return isPath; }
	inline void setIsPath(bool p) { isPath = p; }
	inline Node* getParent() { return parent; }
	inline void setParent(Node* p) { parent = p; }
	void addEdge(Edge& edge);
	void setHeuristic(double h);
	inline double getHeuristic() { return heuristic; }
	std::vector<Node*> getNieghbors();
	inline std::vector<Edge*>& getEdges() { return edges; }
	virtual void draw() override;
	
	static void drawPoints(std::vector<Node*>& nodes);
};

#endif