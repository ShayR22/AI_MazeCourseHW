#include "Cell.hpp"
#include "Drawer.hpp"

Cell::Cell(int x, int y) : x(x), y(y) , isOccupy(false)
{
	/* DO NOTHING */
}

Cell::Cell(const Cell& other) : x(other.x), y(other.y), isOccupy(other.isOccupy)
{

}

void Cell::draw(float offsetX, float offsetY)
{
	float x = offsetX + this->x;
	float y = offsetY + this->y;

	Drawer::rect(x, y, 1.0f, 1.0f, DrawerColor::WHITE);
}