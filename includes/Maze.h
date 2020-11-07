#ifndef __MAZE__
#define __MAZE__

#include "Cell.h"
#include <vector>

typedef std::vector<std::vector<Cell>> cell_mat;

class Maze {
private:
	std::vector<Cell*> starts;
	Cell* target;
	cell_mat cells;
	void allocateMaze(int numRows, int numCols);
	void buildMaze();
public:
	Maze(int numRows, int numCols, bool setStartTarget = true);
	inline cell_mat getCells() { return cells; }
	inline Cell& getCell(int r, int c) { return cells[r][c]; }
	inline void setTarget(int r, int c) { target = &cells[r][c]; }
	bool addStart(int r, int c);
	bool removeStart(int r, int c);
	void drawMaze();
};


#endif /* __MAZE_/_ */