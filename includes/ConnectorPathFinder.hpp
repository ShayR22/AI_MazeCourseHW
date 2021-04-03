
#ifndef __CONNECTOR_PATH_FINDER__
#define __CONNECTOR_PATH_FINDER__

#include <vector>
#include <stack>
#include <map>
#include <set>
#include "GamePoint.hpp"
#include "GamePointEdge.hpp"
#include "CellMovingObject.hpp"

class Room;
class Corridor;
class Cell;
class Team;
class BoardCells;
class Bot;

class ConnectorPathFinder {
private:
	std::vector<Team*>& teams;
	std::vector<Room*>& rooms;
	std::vector<Corridor*>& corridors;
	
	CellMovingObject* cellMovingObject;
	bool isStartNodeExistingNode;
	GamePointNode *start;
	std::vector<GamePointEdge*> startEdges;
	bool isTargetNodeExistingNode;
	GamePointNode *target;
	std::vector<GamePointEdge*> targetEdges;

	std::vector<GamePointNode*> nodes;
	std::vector<GamePointEdge*> edges;

	// this map will show for each node its cost from the start
	std::map<GamePointNode*, double> moveCost; // g

	// this map will show the heuristic estimation for each node to the end
	std::map<GamePointNode*, double> heuristicCost; // h

	// heuristic + moveCost
	std::map<GamePointNode*, double> funcValues; // f = g + h

	std::set<GamePointNode*> openSet;
	std::set<GamePointNode*> closedSet;

	void addStartTarget(GamePoint& gamePointTarget);
	void clear();
	GamePointNode* findNeighborWithLowestFuncValue();
	void calculateStepInPathFromCurrentNode(GamePointNode* lowestF);
	std::stack<GamePoint> generatePath();
	void removeStartTarget();

	GamePointNode* isInNodes(GamePoint &point);
	void generateNodesAndEdges();
public:
	ConnectorPathFinder();
	ConnectorPathFinder(CellMovingObject& cellMovingObject);
	
	std::stack<GamePoint> getPath(GamePoint &gamePoint);

	inline void setSource(CellMovingObject& cellMovingObject) { this->cellMovingObject = &cellMovingObject; }
	inline CellMovingObject& getSource() { return *cellMovingObject; }

	void draw();
};

#endif