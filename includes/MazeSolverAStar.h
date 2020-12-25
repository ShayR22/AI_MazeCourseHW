#ifndef __MAZE_SOLVER_A_STAR__
#define	__MAZE_SOLVER_A_STAR__

#include "Maze.h"
#include "Solvable.h"
#include <map>
#include <set>

class MazeSolverAStar : public Solveable {
	cell_mat& cells;
	std::vector<Cell*>& starts;
	Cell* target;

	std::map<Cell*, Cell*> nextInPath;

	// this map will show for each node its cost from the start
	std::map<Cell*, double> moveCost; // g

	// this map will show the heuristic estimation for each node to the end
	std::map<Cell*, double> heuristicCost; // h

	// heuristic + moveCost
	std::map<Cell*, double> funcValues; // f = g + h

	std::set<Cell*> openSet;
	std::set<Cell*> closedSet;
	Cell* findNeighborWithLowestFuncValue();
	void calculateStepInPathFromCurrentCell(Cell* lowestF);
	std::vector<Cell*> getCellNeighbors(Cell* cell);

	void restorePath();
	void clear();
public:
	MazeSolverAStar(Maze& m);
	void setStartTarget(Cell& start, Cell& target);
	virtual void solveIteration() override;
	std::map<Cell*, Cell*>& getNextInPath() { return nextInPath; }

};


#endif // ! __MAZE_SOLVER_A_STAR__
