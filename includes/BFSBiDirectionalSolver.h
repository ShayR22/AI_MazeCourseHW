#ifndef __BFSBIDIRECTIONALSOLVER__
#define __BFSBIDIRECTIONALSOLVER__

#include <stack>
#include <set>
#include "Maze.h"


class BFSBiDirectionalSolver
{

private:
	Maze& maze;
	cell_mat& cells;
	//std::stack <Cell*> visting;
	std::set <Cell*> vistingStart;
	std::set <Cell*> vistingTarget;
	bool solved;

	void solveIterationHelper(std::set<Cell*>& visiting, std::set<Cell*>& targets);
	void checkCellNeighbors(Cell& cell, std::set<Cell*>& visiting, std::set<Cell*>& targets);
	void restorePath(Cell& currentCell);

public:
	BFSBiDirectionalSolver(Maze& maze);

	void solveIteration();
};

#endif