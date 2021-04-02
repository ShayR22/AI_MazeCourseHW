#ifndef __ROOM_PATH_FINDER__
#define __ROOM_PATH_FINDER__

#include <vector>
#include <stack>
#include <map>
#include <set>

#include "Cell.hpp"
#include "CellMovingObject.hpp"

class Bot;

class RoomPathFinder {
private:
	CellMovingObject* cellMovingObject;
	Cell* target;

	std::map<Cell*, Cell*> nextInPath;
	std::map<Cell*, double> moveCost; // g
	std::map<Cell*, double> heuristicCost; // h
	std::map<Cell*, double> funcValues; // f = g + h

	std::set<Cell*> openSet;
	std::set<Cell*> closedSet;

	/* used for draw */
	BoardCells* lastSearchBoard;

	std::set<Cell*> visiting;
	std::set<Cell*> visited;
	std::map<Cell*, Cell*> parent;
	std::set<Cell*> path;

	/* target would be use to recalculate statistics*/
	void clear(Cell *target);

	Cell* findNeighborWithLowestFuncValue();
	void calculateStepInPathFromCurrentCell(Cell* lowestF);
	std::vector<Cell*> getCellNeighbors(Cell& cell);
	std::map<Cell*, Cell*> setNextInPath();
	void restorePath();

public:
	RoomPathFinder();
	RoomPathFinder(CellMovingObject& cellMovingObject);
	/* a map containing from each cell what is the next cell 
	 * if path is not defined nullptr would be the value used
	 */
	std::map<Cell*, Cell*> getPath(Cell* target);
	
	inline void setSource(CellMovingObject& cellMovingObject) { this->cellMovingObject = &cellMovingObject; }
	inline CellMovingObject& getSource() { return *cellMovingObject; }

	void draw();
};

#endif