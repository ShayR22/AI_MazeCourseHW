#ifndef __CELL__
#define __CELL__

class Cell {
private:
	int x;
	int y;
public:
	Cell(int x, int y);

	inline void setX(int x) { this->x = x; }
	inline int getX() { return x; }
	inline void setY(int y) { this->y = y; }
	inline int getY() { return y; }
	
	void draw(float offsetX, float offsetY);
};

#endif
