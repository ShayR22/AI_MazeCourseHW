#include "RoomPathFinder.hpp"
#include "BoardCells.hpp"
#include "Cell.hpp"
#include "Bot.hpp"
#include "heruistics.h"
#include "Drawer.hpp"

#include <iostream>

using namespace std;

void RoomPathFinder::clear(Cell *target)
{
	openSet.clear();
	closedSet.clear();
	visiting.clear();
	visited.clear();
	path.clear();
	parent.clear();

	/* extract room offset and cells */
	BoardCells& board = cellMovingObject->getBoardCells();
	vec2f& xyOffset = board.getXYOffset();
	CellMat& cells = board.getCells();

	float tgtX = xyOffset.x + target->getX();
	float tgtY = xyOffset.y + target->getY();

	for (auto& cellRow : cells) {
		for (auto& cell : cellRow) {
			float srcX = xyOffset.x + cell.getX();
			float srcY = xyOffset.y + cell.getY();

			double heuristicVal = manhattan_distance(srcX, srcY, tgtX, tgtY);

			heuristicCost[&cell] = heuristicVal;
			moveCost[&cell] = INT_MAX;
			funcValues[&cell] = INT_MAX;
			nextInPath[&cell] = nullptr;
			parent[&cell] = nullptr;
		}
	}

	Cell* start = &cellMovingObject->getCellLocation();
	moveCost[start] = 0;
	funcValues[start] = heuristicCost[start] + moveCost[start];
	openSet.insert(start);

	this->target = target;

	/* update for draw function */
	lastSearchBoard = &board;
}

Cell* RoomPathFinder::findNeighborWithLowestFuncValue()
{
	/* add + 1 for edge case in start where all values are INT_MAX */
	double minVal = (double)INT_MAX + 1;
	Cell* lowestNeighborF = nullptr;

	for (auto& n : openSet) {
		double curfVal = funcValues[n];
		if (curfVal < minVal) {
			minVal = curfVal;
			lowestNeighborF = n;
		}
	}

	return lowestNeighborF;
}

vector<Cell*> RoomPathFinder::getCellNeighbors(Cell& cell)
{
	vector<Cell*> neighbors;
	CellMat& cells = cellMovingObject->getBoardCells().getCells();
	/* add -1 to accomidate the fact that array start from index 0 */
	int width = cells[0].size() - 1;
	int height = cells.size() - 1;

	int x = cell.getX();
	int y = cell.getY();
	int offsets[][2] = {
		{x + 1, y},
		{x - 1, y},
		{x, y + 1},
		{x, y - 1}
	};

	for (auto& offset : offsets) {
		int nx = offset[0];
		int ny = offset[1];

		if (nx < 0 || nx > width) {
			continue;
		}
		else if (ny < 0 || ny > height) {
			continue;
		}

		Cell* neighbor = &cells[ny][nx];

		if (!neighbor->getIsOccupy()) {
			neighbors.push_back(neighbor);
		}
	}

	return neighbors;
}


void RoomPathFinder::calculateStepInPathFromCurrentCell(Cell* lowestF)
{
	vector<Cell*> neighbors = getCellNeighbors(*lowestF);
	for (auto& n : neighbors) {
		if (closedSet.find(n) != closedSet.end())
			continue;

		double currentNeighborCost = moveCost[n];
		/* assume in maze all weights are 1 */
		double costFromCellToNeighbor = moveCost[lowestF] + 1;

		if (openSet.find(n) == openSet.end()) {
			openSet.insert(n);
			visiting.insert(n);
		}
		else if (currentNeighborCost < costFromCellToNeighbor) {
			continue;
		}

		/* if not continued meaning need to update maps costs */
		moveCost[n] = costFromCellToNeighbor;
		funcValues[n] = moveCost[n] + heuristicCost[n];
		parent[n] = lowestF;
	}
}

map<Cell*, Cell*> RoomPathFinder::setNextInPath()
{
	map<Cell*, Cell*> path;
	Cell* curTarget = target;
	Cell* curParent = parent[curTarget];

	while (curParent != nullptr) {
		path[curParent] = curTarget;
		curTarget = curParent;
		curParent = parent[curParent];
	}

	return path;
}

void RoomPathFinder::restorePath()
{
	Cell* start = &cellMovingObject->getCellLocation();

	Cell* temp = target;
	while (temp != nullptr)
	{
		path.insert(temp);
		temp = parent[temp];
	}
}

std::map<Cell*, Cell*> RoomPathFinder::getPath(Cell* target)
{
	map<Cell*, Cell*> path;
	clear(target);

	if (cellMovingObject == nullptr) {
		cout << __func__ << ": bot is nullptr" << endl;
		return nextInPath;
	}

	while (!openSet.empty()) {
		Cell* lowestF = findNeighborWithLowestFuncValue();
		openSet.erase(lowestF);
		visiting.erase(lowestF);
		closedSet.insert(lowestF);
		visited.insert(lowestF);

		if (lowestF == target) {
			restorePath();
			path = setNextInPath();
			break;
		}

		calculateStepInPathFromCurrentCell(lowestF);
	}

	return path;
}

RoomPathFinder::RoomPathFinder() : cellMovingObject(nullptr), target(nullptr), lastSearchBoard(nullptr)
{

}

RoomPathFinder::RoomPathFinder(CellMovingObject& cellMovingObject) : cellMovingObject(&cellMovingObject),
target(nullptr), lastSearchBoard(nullptr)
{
	/* DO NOTHING */
}


void RoomPathFinder::draw()
{
	vec2f& xy = lastSearchBoard->getXYOffset();

	for (auto& c : path) {
		float x = xy.x + c->getX();
		float y = xy.y + c->getY();

		Drawer::rect(x, y, 1, 1, DrawerColor::PURPULE);
	}
}

