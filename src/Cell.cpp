#include "Cell.hpp"
#include "Drawer.hpp"

Cell::Cell(int x, int y) : x(x), y(y)
{
	/* DO NOTHING */
}

void Cell::draw(float offsetX, float offsetY)
{
	Drawer::rect(offsetX, offsetY, 1, 1, DrawerColor::WHITE);
}