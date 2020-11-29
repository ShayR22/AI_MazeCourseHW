#ifndef __BFSSOLVER__
#define __BFSSOLVER__

#include <stack>
#include "Solvable.h"
#include "Maze.h"


class BFSSolver : public Solveable {
private:
	Maze& maze;
	cell_mat& cells;
	std::stack <Cell*> visting;

	void checkCellNeighbors(Cell& cell);
	void restorePath(Cell& currentCell);

public:
	BFSSolver(Maze& maze);
	virtual void solveIteration() override;
};

#endif