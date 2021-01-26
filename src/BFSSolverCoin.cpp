#include "BFSSolverCoin.h"

using namespace std;

BFSSolverCoin::BFSSolverCoin(Maze& maze) : maze(maze), cells(maze.getCells())
{
	start = maze.getStarts()[0];
	start->setVisiting(true);
	visiting.push_back(start);
	solved = false;
}

void BFSSolverCoin::clear()
{
	solved = false;
	visiting.clear();

	for (auto& cellRow : cells) {
		for (auto& cell : cellRow) {
			cell.setVisited(false);
			cell.setVisiting(false);
			cell.setIsPath(false);
			cell.setParent(nullptr);
		}
	}
}

void BFSSolverCoin::setStart(Cell& s)
{
	start = &s;
	clear();
	start->setVisiting(true);
	visiting.push_back(start);
}


void BFSSolverCoin::checkCellNeighbors(Cell& cell)
{
	int row = cell.getX();
	int col = cell.getY();

	/* left top right down */
	int xys[4][2] = {
		{row, col - 1},
		{row - 1, col},
		{row, col + 1},
		{row + 1, col},
	};
	bool* walls = cell.getWalls();

	for (int i = 0; i < 4; i++) {
		if (walls[i])
			continue;

		int r = xys[i][0];
		int c = xys[i][1];
		Cell* neighbor = &cells[r][c];
		if (neighbor->getVisited())
			continue;

		neighbor->setVisiting(true);
		neighbor->setParent(&cell);

		if (neighbor->getHasCoin()) {
			restorePath(*neighbor);
			solved = true;
			return;
		}

		visiting.push_back(neighbor);
	}
}

void BFSSolverCoin::restorePath(Cell& currentCell)
{
	Cell* temp = &currentCell;
	while (temp != nullptr)
	{
		temp->setIsPath(true);
		temp = temp->getParent();

	}
}

void BFSSolverCoin::solveIteration()
{
	if (solved) {
		return;
	}

	if (visiting.empty()) {
		return;
	}

	Cell* first = visiting.front();

	first->setVisited(true);
	first->setVisiting(false);
	checkCellNeighbors(*first);

	visiting.erase(visiting.begin());
}
