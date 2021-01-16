#ifndef __BFSBIDIRECTIONALSOLVER__
#define __BFSBIDIRECTIONALSOLVER__

#include <stack>
#include <set>
#include "Solvable.h"
#include "Maze.h"


class BFSBiDirectionalSolver : public Solveable {

private:
	Maze& maze;
	cell_mat& cells;
	std::set <Cell*> vistingStart;
	std::set <Cell*> vistingTarget;

	void solveIterationHelper(std::set<Cell*>& visiting, std::set<Cell*>& targets);
	void checkCellNeighbors(Cell& cell, std::set<Cell*>& visiting, std::set<Cell*>& targets);
	void restorePath(Cell& currentCell);

public:
	BFSBiDirectionalSolver(Maze& maze);
	virtual void solveIteration() override;
};

#endif