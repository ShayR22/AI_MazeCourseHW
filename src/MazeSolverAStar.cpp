#include <limits.h>
#include <vector>
#include "MazeSolverAStar.h"
#include "heruistics.h"
#include "Cell.h"

using namespace std;

MazeSolverAStar::MazeSolverAStar(Maze& m) : cells(m.getCells()), starts(m.getStarts()), 
	target(&m.getTarget())
{
	solved = false;
	setStartTarget(*m.getStarts()[0], m.getTarget());
}

Cell* MazeSolverAStar::findNeighborWithLowestFuncValue()
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

std::vector<Cell*> MazeSolverAStar::getCellNeighbors(Cell *cell)
{
	vector<Cell*> neighbors;

	int row = cell->getX();
	int col = cell->getY();

	/* left top right down */
	int xys[4][2] = {
		{row, col - 1},
		{row - 1, col},
		{row, col + 1},
		{row + 1, col},
	};
	bool* walls = cell->getWalls();

	for (int i = 0; i < 4; i++) {
		if (walls[i])
			continue;

		int r = xys[i][0];
		int c = xys[i][1];
		neighbors.push_back(&cells[r][c]);
	}

	return neighbors;
}

void MazeSolverAStar::calculateStepInPathFromCurrentCell(Cell* lowestF)
{
	vector<Cell*> neighbors = getCellNeighbors(lowestF);
	for (auto& n : neighbors) {
		if (closedSet.find(n) != closedSet.end())
			continue;

		double currentNeighborCost = moveCost[n];
		/* assume in maze all weights are 1 */
		double costFromCellToNeighbor = moveCost[lowestF] + 1;

		if (openSet.find(n) == openSet.end()) {
			openSet.insert(n);
			n->setVisiting(true);
		}
		else if (currentNeighborCost < costFromCellToNeighbor) {
			continue;
		}

		/* if not continued meaning need to update maps costs */
		moveCost[n] = costFromCellToNeighbor;
		funcValues[n] = moveCost[n] + heuristicCost[n];
		n->setParent(lowestF);
		nextInPath[lowestF] = n;
	}
}

void MazeSolverAStar::restorePath()
{
	Cell* temp = target->getParent();
	while (temp != nullptr && temp != starts[0])
	{
		temp->setIsPath(true);
		temp = temp->getParent();
	}
}

void MazeSolverAStar::solveIteration()
{
	if (solved || openSet.empty())
		return;

	Cell* lowestF = findNeighborWithLowestFuncValue();
	openSet.erase(lowestF);
	lowestF->setVisiting(false);
	closedSet.insert(lowestF);
	lowestF->setVisited(true);

	if (lowestF == target) {
		std::cout << "solved" << std::endl;
		solved = true;
		restorePath();
		return;
	}

	calculateStepInPathFromCurrentCell(lowestF);
}

void MazeSolverAStar::clear()
{

	openSet.clear();
	closedSet.clear();

	for (auto& cellRow : cells) {
		for (auto& cell : cellRow) {
			cell.setVisited(false);
			cell.setVisiting(false);
			cell.setIsPath(false);

			double h = manhattan_distance(&cell, target);
			heuristicCost[&cell] = h;

			moveCost[&cell] = INT_MAX;
			funcValues[&cell] = INT_MAX;

		}
	}
}


void MazeSolverAStar::setStartTarget(Cell& s, Cell& t)
{
	starts[0] = &s;
	target = &t;
	clear();

	Cell* start = starts[0];
	moveCost[start] = 0;
	funcValues[start] = heuristicCost[start] + moveCost[start];
	openSet.insert(start);
}