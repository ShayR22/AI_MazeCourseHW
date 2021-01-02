#ifndef __BFSSOLVER__
#define __BFSSOLVER__

#include <stack>
#include "Solvable.h"
#include "Maze.h"
#include <set>


class BFSSolver : public Solveable {
private:
	Maze& maze;
	cell_mat& cells;
	std::vector<Cell*> visiting;
	std::vector<Cell*> targets;
	Cell* start;

	void checkCellNeighbors(Cell& cell);
	void restorePath(Cell& currentCell);


public:
	BFSSolver(Maze& maze);
	void clear();
	void setStartTarget(Cell& s, std::vector<Cell*> targets);
	virtual void solveIteration() override;
};

#endif