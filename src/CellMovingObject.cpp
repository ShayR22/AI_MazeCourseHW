#include "CellMovingObject.hpp"
#include "Cell.hpp"
#include "vec2.h"
#include <iostream>

CellMovingObject::CellMovingObject(vec2f& location, vec2f& maxSpeed, vec2f& target, float boundingRadius, BoardCells& board) :
	MovingObject(location, maxSpeed, target, boundingRadius), board(board), nextBoard(nullptr)
{

}

vec2f CellMovingObject::getCellCenter(BoardCells& board, Cell& cell)
{
	vec2f& bXY = board.getXYOffset();
	return vec2f(bXY.x + cell.getX() + 0.5f, bXY.y + cell.getY() + 0.5f);
}

void CellMovingObject::setTarget(BoardCells& board,Cell& cell)
{
	this->nextBoard = &board;
	MovingObject::setTarget(getCellCenter(board,cell));
}

void CellMovingObject::setLocation(BoardCells& board,Cell& cell)
{
	this->board = board;
	MovingObject::setLocation(getCellCenter(board,cell));
}

Cell& CellMovingObject::getCellLocation()
{
	CellMat& cells = board.getCells();
	vec2f boardLocation = board.getXYOffset();
	vec2f relativeLocation = location - boardLocation;
	
	int x = static_cast<int>(round(relativeLocation.x));
	int y = static_cast<int>(round(relativeLocation.y));

	return cells[x][y];
}

Cell& CellMovingObject::getLastCellLocation()
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

void CellMovingObject::move()
{
	MovingObject::move();
	if (isAtTarget() && nextBoard) {
		board = *nextBoard;
	}
}