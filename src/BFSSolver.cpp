#include "BFSSolver.h"

using namespace std;

BFSSolver::BFSSolver(Maze& maze) : maze(maze), solved(false) ,cells(maze.getCells())
{
	Cell* start = maze.getStarts()[0];
	start->setVisiting(true);
	visting.push(start);
}

void BFSSolver::checkCellNeighbors(Cell& cell)
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
		if (*neighbor == maze.getTarget()) {
			restorePath(*neighbor);
			solved = true;
			return;
		}

		visting.push(neighbor);
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

	if (visting.empty() || solved)
		return;

	stack <Cell*> temp(visting);

	while (!visting.empty()) {
		Cell* cell = visting.top();
		cell->setVisited(true);
		cell->setVisiting(false);
		visting.pop();
	}
	
	while (!temp.empty() && !solved) {
		checkCellNeighbors(*temp.top());
		temp.pop();
	}
	

}