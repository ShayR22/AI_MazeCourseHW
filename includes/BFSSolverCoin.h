#ifndef __BFSSOLVERCOIN__
#define __BFSSOLVERCOIN__

#include <stack>
#include "Solvable.h"
#include "Maze.h"
#include <set>

class BFSSolverCoin : public Solveable {
private:
	Maze& maze;
	cell_mat& cells;
	std::vector<Cell*> visiting;
	Cell* start;

	void checkCellNeighbors(Cell& cell);
	void restorePath(Cell& currentCell);

public:
	BFSSolverCoin(Maze& maze);
	void clear();
	void setStart(Cell& s);
	virtual void solveIteration() override;
};

#endif