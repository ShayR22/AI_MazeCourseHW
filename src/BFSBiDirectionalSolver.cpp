#include "BFSBiDirectionalSolver.h"

using namespace std;

BFSBiDirectionalSolver::BFSBiDirectionalSolver(Maze& maze) : maze(maze), solved(false), cells(maze.getCells())
{
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

	Cell* neighbors[4];
	neighbors[0] = &cells[row][col - 1]; // left neighbor
	neighbors[1] = &cells[row][col + 1]; // right neighbor
	neighbors[2] = &cells[row - 1][col]; // top neighbor
	neighbors[3] = &cells[row + 1][col]; // buttom neighbor

	for (int i = 0; i < 4; i++) {
		if (neighbors[i]->getVisited() || neighbors[i]->getState() == CellState::wall)
			continue;

		neighbors[i]->setVisiting(true);

		if (targets.find(neighbors[i]) != targets.end()) {
			neighbors[i]->setState(CellState::path);
			cell.setState(CellState::path);
			restorePath(*neighbors[i]);
			restorePath(cell);
			solved = true;
			return;
		}

		neighbors[i]->setParent(&cell);
		visiting.insert(neighbors[i]);
	}
}

void BFSBiDirectionalSolver::restorePath(Cell& currentCell)
{
	Cell* temp = currentCell.getParent();
	while (temp != nullptr)
	{
		temp->setState(CellState::path);
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
