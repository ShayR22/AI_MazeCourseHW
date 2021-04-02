#ifndef __GAME_POINT_NODE__
#define __GAME_POINT_NODE__

#include <vector>

#include "vec2.h"
#include "GamePoint.hpp"

class GamePointEdge;

class GamePointNode {
private:
	GamePoint gameLocation;
	vec2f xy;
	bool isVisiting;
	bool visited;
	bool isPath;
	GamePointNode* parent;
	std::vector<GamePointEdge*> edges;

public:
	GamePointNode(GamePoint& gameLocation);
	GamePointNode(const GamePointNode& node);
	GamePoint& getGameLocation() { return gameLocation; }
	void setGameLocation(GamePoint& gameLocation) { this->gameLocation = gameLocation; }
	inline vec2f& getXY() { return xy; }
	inline void setXY(vec2f& xy) { this->xy = xy; }
	inline float getX() { return xy.x; }
	inline void setX(float x) { xy.x = x; }
	inline float getY() { return xy.y; }
	inline void setY(float y) { xy.y = y; }
	inline bool getIsVisiting() { return isVisiting; }
	inline void setIsVisiting(bool v) { isVisiting = v; }
	inline bool getVisited() { return visited; }
	inline void setVisited(bool v) { visited = v; }
	inline bool getIsPath() { return isPath; }
	inline void setIsPath(bool p) { isPath = p; }
	inline GamePointNode* getParent() { return parent; }
	inline void setParent(GamePointNode* p) { parent = p; }
	void addEdge(GamePointEdge& edge);

	std::vector<GamePointNode*> getNieghbors();
	inline std::vector<GamePointEdge*>& getEdges() { return edges; }

	bool operator==(const GamePointNode& other) { return (xy.x == other.xy.x) && (xy.y == other.xy.y); }
};


#endif
