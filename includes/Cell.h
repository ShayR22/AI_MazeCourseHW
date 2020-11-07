#ifndef __CELL__
#define __CELL__

#include <iostream>
#include "glut.h"


enum class CellState {
	wall,
	space,
	path
};

class Cell {

private:
	int row;
	int col;
	Cell* parent;
	bool visited;
	bool visiting;
	CellState state;
public:
	Cell();
	Cell(int row, int col, const CellState& state);

	inline int getRow() const { return row; }
	inline int getCol() const { return col; }

	inline void setVisiting(bool v) { visiting = v; }
	inline bool getVisiting() const { return visiting; }
	
	inline void setVisited(bool v) { visited = v; }
	inline bool getVisited() const { return visited; }

	inline void setParent(Cell* p) { parent = p; }
	inline Cell* getParent() const { return parent; }

	void setLocation(int r, int c);
	
	inline void setState(const CellState& s) { state = s; }
	inline const CellState& getState() const { return state; }

	inline bool operator==(const Cell& other) const { return (row == other.row && col == other.col); }
	inline bool equal(int r, int c) const { return (row == r && col == c); }

	void setOpenGLColor();
	
	static inline void setOpenGLStartColor() 
	{
		glColor3d(0, 1, 1);   // cyan
	}
	static inline void setOpenGLTargetColor()
	{
		glColor3d(1, 0, 0);   // red
	}

	friend std::ostream& operator<<(std::ostream& os, const Cell& c);
};



#endif /* __CELLL__ */