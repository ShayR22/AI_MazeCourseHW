#ifndef __BiDirectionalBFSMazeSolver__
#define __BiDirectionalBFSMazeSolver__

#include <vector>
#include "Maze.h"

class BiDirectionalBFSMazeSolver {
private:
	Maze& maze;
	std::vector<Cell*> solvedPath;
public:
	BiDirectionalBFSMazeSolver(Maze& maze);
	void solve();
	void solveIteration();
};


#endif /* BiDirectionalBFSMazeSolver */

