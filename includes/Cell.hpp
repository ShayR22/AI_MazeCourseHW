#ifndef __CELL__
#define __CELL__

#include <vector>

class Cell {
private:
	int x;
	int y;
	bool isOccupy;
public:
	Cell(int x, int y);

	Cell(const Cell& cell);

	inline void setX(int x) { this->x = x; }
	inline int getX() { return x; }
	inline void setY(int y) { this->y = y; }
	inline int getY() { return y; }
	inline void setIsOccupy(bool isOccupy) { this->isOccupy = isOccupy; }
	inline bool getIsOccupy() { return isOccupy; }

	void draw(float offsetX, float offsetY);
};

typedef std::vector<std::vector<Cell>> CellMat;

#endif
