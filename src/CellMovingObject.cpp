#include "CellMovingObject.hpp"
#include "Cell.hpp"
#include "vec2.h"
#include <iostream>

CellMovingObject::CellMovingObject(vec2f& location, vec2f& maxSpeed, vec2f& target, float boundingRadius, BoardCells& board) :
	MovingObject(location, maxSpeed, target, boundingRadius), board(board)
{

}

void CellMovingObject::setTarget(Cell& cell)
{
	MovingObject::setTarget(getCellCenter(cell));
}

void CellMovingObject::setLocation(Cell& cell)
{
	MovingObject::setLocation(getCellCenter(cell));
}

inline Cell& CellMovingObject::getCellLocation()
{
	CellMat& cells = board.getCells();
	vec2f boardLocation = board.getXYOffset();
	vec2f relativeLocation = location - boardLocation;
	
	int x = static_cast<int>(round(relativeLocation.x));
	int y = static_cast<int>(round(relativeLocation.y));

	return cells[x][y];
}

inline Cell& CellMovingObject::getLastCellLocation()
{
	CellMat& cells = board.getCells();

	return cells[lastPos.x][lastPos.y];
}


void CellMovingObject::updateLastLocation()
{
	int newX = (int)location.x;
	int newY = (int)location.y;
	
	if (newX == lastPos.x && newY == lastPos.y)
		return;

	lastPos = { newX, newY };
}