#include "BFSBiDirectionalSolver.h"

using namespace std;

BFSBiDirectionalSolver::BFSBiDirectionalSolver(Maze& maze) : maze(maze), cells(maze.getCells())
{
	solved = false;

	// first stack with start as starting point
	Cell* start = maze.getStarts()[0];
	start->setVisiting(true);
	vistingStart.insert(start);

	// second stack with target as starting point
	Cell* target = &maze.getTarget();
	target->setVisiting(true);
	vistingTarget.insert(target);
}

void BFSBiDirectionalSolver::checkCellNeighbors(Cell& cell, set<Cell*>& visiting, set<Cell*>& targets)
{
	int row = cell.getRow();
	int col = cell.getCol();

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

		if (targets.find(neighbor) != targets.end()) {
			neighbor->setIsPath(true);
			cell.setIsPath(true);
			restorePath(*neighbor);
			restorePath(cell);
			solved = true;

			return;
		}

		neighbor->setParent(&cell);
		visiting.insert(neighbor);
	}
}

void BFSBiDirectionalSolver::restorePath(Cell& currentCell)
{
	Cell* temp = currentCell.getParent();
	while (temp != nullptr)
	{
		temp->setIsPath(true);
		temp = temp->getParent();
	}
}

void BFSBiDirectionalSolver::solveIterationHelper(set<Cell*>& visiting, set<Cell*>& targets)
{
	/*
	1. check if visitng set is empty
	2. empty the visting set to a temp stack && mark them as visted.
	3. push to visting set, temp's neighbors cells.
	4. check if one of temp's neighbors cells is in target set.
	5. if we found target - > solved = true.
	*/

	if (visiting.empty() || solved)
		return;

	vector <Cell*> temp;

	for (auto& e : visiting) {
		e->setVisited(true);
		temp.push_back(e);
	}
	
	visiting.clear();
	
	for (auto& e : temp) {
		if (solved)
			break;
		checkCellNeighbors(*e, visiting, targets);
	}

	/* if not solved remove last gray (temp) from visiting status */
	if (!solved) {
		for (auto& e: temp)
			e->setVisiting(false);
	}
}

void BFSBiDirectionalSolver::solveIteration()
{
	if (solved)
		return;

	solveIterationHelper(vistingStart, vistingTarget);
	solveIterationHelper(vistingTarget, vistingStart);
	
}
