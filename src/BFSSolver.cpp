#include "BFSSolver.h"

using namespace std;

BFSSolver::BFSSolver(Maze& maze) : maze(maze),cells(maze.getCells())
{
	start = maze.getStarts()[0];
	start->setVisiting(true);
	visiting.push_back(start);
	targets.push_back(&maze.getTarget());
	solved = false;
}

void BFSSolver::clear()
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

void BFSSolver::setStartTarget(Cell& s, vector<Cell*> targets)
{
	start = &s;
	this->targets = targets;
	clear();
	start->setVisiting(true);
	visiting.push_back(start);
}


void BFSSolver::checkCellNeighbors(Cell& cell)
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

	for(int i = 0; i < 4; i++) {
		if (walls[i])
			continue;

		int r = xys[i][0];
		int c = xys[i][1];
		Cell* neighbor = &cells[r][c];
		if (neighbor->getVisited())
			continue;

		neighbor->setVisiting(true);
		neighbor->setParent(&cell);
		if (find(targets.begin(), targets.end(), neighbor) != targets.end()) {
			restorePath(*neighbor);
			targets.erase(remove(targets.begin(), targets.end(), neighbor), targets.end());
			if (targets.empty()) {
				solved = true;
				return;
			}
		}

		visiting.push_back(neighbor);
	}	
}

void BFSSolver::restorePath(Cell& currentCell)
{
	Cell* temp = currentCell.getParent();
	while (temp != nullptr)
	{
		temp->setIsPath(true);
		temp = temp->getParent();
	}
}

void BFSSolver::solveIteration()
{
	/*
	1. check if stack is empty || solved
	2. empty the visting stack to a temp stack && mark them as visted.
	3. push to visting temp's neighbors cells.
	4. check if one of temp's neighbors cells is the target.
	5. if we found target - > solved = true.
	*/

	if (solved) {
		return;
	}
	
	if (visiting.empty()) {
		targets.erase(remove(targets.begin(), targets.end(), start), targets.end());

		if (targets.empty()) {
			solved = true;
			return;
		}
	}

	Cell* first = visiting.front();

	first->setVisited(true);
	first->setVisiting(false);
	checkCellNeighbors(*first);

	visiting.erase(visiting.begin());
}