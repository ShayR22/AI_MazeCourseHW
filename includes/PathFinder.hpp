#ifndef __PATHFINDER__
#define __PATHFINDER__

#include <vector>
#include <stack>
#include <map>
#include <set>

#include "GamePoint.hpp"
#include "Cell.hpp"
#include "CellMovingObject.hpp"

class Room;
class Corridor;
class Team;
class Bot;
class BoardCells;
class RoomPathFinder;
class ConnectorPathFinder;
class GamePointEdge;

class PathFinder {
private:
	std::vector<Team*>& teams;
	std::vector<Room*>& rooms;
	std::vector<Corridor*>& corridors;
	CellMovingObject* cellMovingObject;
	RoomPathFinder* roomFinder;
	ConnectorPathFinder* connectorFinder;

	std::vector<Cell*> getCellNeighbors(Cell& cell);
	GamePoint findClosetConsumable(ConsumableType type);
	std::stack<GamePoint> searchClosetConsumable(ConsumableType type);
	Bot* getClosestEnemy(Team& enemyTeam, bool& closeEnemy, float& minFireRange);
	std::stack<GamePoint> roamWithDistancing(Bot& closetEnemy);

	Team* getEnemyTeam();
public:
	PathFinder();
	PathFinder(CellMovingObject& cellMovingObject);
	std::map<Cell*, Cell*> getRoomPath(Cell* target);
	std::stack<GamePoint> searchTeammate(Bot& teammate);
	std::stack<GamePoint> searchClosetEnemy();
	std::stack<GamePoint> searchClosetAmmo();
	std::stack<GamePoint> searchClosetHealth();
	std::stack<GamePoint> roam();

	void setSource(CellMovingObject& cellMovingObject);
	inline CellMovingObject& getSource() { return *cellMovingObject; }

	void drawGraph();
	void drawRoomPath();

};

#endif