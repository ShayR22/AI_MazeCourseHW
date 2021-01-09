#ifndef __CELL__
#define __CELL__

#include <iostream>
#include <vector>
#include "glut.h"
#include "Drawable.h"

constexpr auto NUM_WALLS = 4;

class Cell : public Drawable {

private:
	/* left, top, right, down */
	bool restoreWalls[NUM_WALLS];
	bool walls[NUM_WALLS];
	int row;
	int col;
	Cell* parent;
	bool visited;
	bool visiting;
	bool isPath;
	bool hasCoin;

	void drawPoly(float x, float y, float w, float h);
	void drawCoin(float x, float y, float w, float h);
	void drawWalls(int numWalls, float x, float y, float w, float h);
public:
	Cell();
	Cell(int row, int col);

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
	inline void setRestoreWalls(bool *otherWalls) { memcpy(restoreWalls, otherWalls, sizeof(restoreWalls)); }
	inline void wallRestoration() { memcpy(walls, restoreWalls, sizeof(walls)); }


	inline int getX() const { return row; }
	inline int getY() const { return col; }

	inline void setVisiting(bool v) { visiting = v; }
	inline bool getVisiting() const { return visiting; }
	
	inline void setVisited(bool v) { visited = v; }
	inline bool getVisited() const { return visited; }

	inline void setParent(Cell* p) { parent = p; }
	inline Cell* getParent() const { return parent; }

	void setLocation(int r, int c);
	
	inline void setIsPath(bool path) { isPath = path; }
	inline bool getPath() { return isPath; }

	inline void setHasCoin(bool c) { hasCoin = c; }
	inline bool getHasCoin() { return hasCoin; }

	inline bool operator==(const Cell& other) const { return (row == other.row && col == other.col); }
	inline bool equal(int r, int c) const { return (row == r && col == c); }

	virtual void draw() override;
	/* smart draw for preformance for the maze usage*/
	void drawTopLeft();
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