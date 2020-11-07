#ifndef __BFSSOLVER__
#define __BFSSOLVER__

#include <stack>
#include "Maze.h"


class BFSSolver
{

private:
	Maze& maze;
	cell_mat& cells;
	std::stack <Cell*> visting;
	bool solved;

	void checkCellNeighbors(Cell& cell);
	void restorePath(Cell& currentCell);

public:
	BFSSolver(Maze& maze);

	void solveIteration();
};

#endif