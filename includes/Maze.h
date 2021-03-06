#ifndef __MAZE__
#define __MAZE__

#include <vector>
#include "Drawable.h"
#include "Cell.h"

typedef std::vector<std::vector<Cell>> cell_mat;

class Maze : public Drawable {
private:
	bool hideColor;
	std::vector<Cell*> starts;
	Cell* target;
	cell_mat cells;
	void allocateMaze(int numRows, int numCols);
	void removeWall(Cell* c, Cell* neighbor, int index);
	/* set neighbors [left, top, right , down], neighbors array of poitners*/
	void getNeighbors(Cell* c, Cell** neighbors, int* numNeighbors);
	Cell* getPrevNeighborDirection(Cell** neighbors, int prevIndex);
	Cell* randomizeNeighbor(Cell** neighbors, int numNeighbors, int* neighborIndex);
	void randomRemove();
	void buildMaze();
	void drawBackground();
public:
	Maze(int numRows, int numCols, bool setStartTarget = true);
	inline cell_mat& getCells() { return cells; }
	inline Cell& getCell(int r, int c) { return cells[r][c]; }
	inline void setTarget(int r, int c) { target = &cells[r][c]; }
	inline std::vector<Cell*>& getStarts() { return starts; }
	inline Cell& getTarget() { return *target; }

	inline void setColorHidden(bool ch) { hideColor = ch; }
	inline bool getColorHidden() { return hideColor; }

	bool addStart(int r, int c);
	bool removeStart(int r, int c);
	std::vector<Cell*> getNeighbors(Cell& c);
	bool isLeadingToDeadEnd(Cell& src, Cell& explore, int depth);
	virtual void draw() override;
};


#endif /* __MAZE_/_ */