#ifndef __CELL__
#define __CELL__

#include <iostream>
#include "glut.h"

constexpr auto NUM_WALLS = 4;

enum class CellState {
	wall,
	space,
	path
};

class Cell {

private:
	/* left, top, right, down */
	bool walls[NUM_WALLS];
	int row;
	int col;
	Cell* parent;
	bool visited;
	bool visiting;
	CellState state;
public:
	Cell();
	Cell(int row, int col, const CellState& state);

	inline bool getWallLeft() { return walls[0]; }
	inline bool getWallTop() { return walls[1]; }
	inline bool getWallRight() { return walls[2]; }
	inline bool getWallDown() { return walls[3]; }
	inline bool* getWalls() { return walls; }
	inline void setWallLeft(bool w) { walls[0] = w; }
	inline void setWallTop(bool w) { walls[1] = w; }
	inline void setWallRight(bool w) { walls[2] = w; }
	inline void setWallDown(bool w) { walls[3] = w; }
	inline void setWalls(bool* otherWalls) { memcpy(walls, otherWalls, sizeof(walls)); }

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

	void draw();
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