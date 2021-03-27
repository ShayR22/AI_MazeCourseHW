#include "Game.hpp"
#include "Drawer.hpp"
#include "vec2.h"

#include <iostream>
#include <stdio.h>
#include <map>
#include <vector>

constexpr auto ROOM_WIDTH_MIN = 5;
constexpr auto ROOM_HEIGHT_MIN = 5;
constexpr auto CORRIDOR_MIN_LEGTH = 2;

constexpr auto MAX_ROOM_ROWS = 3;
constexpr auto MAX_ROOM_COLS = 3;

constexpr auto ROOM_START_OFFSET_X = 1;
constexpr auto ROOM_START_OFFSET_Y = 1;

using namespace std;

static inline int randomizeNumber(int from, int to)
{
	return (rand() % (to - from)) + from + 1;
}

/* columns means the width, and rows are the height*/
CellMat Game::generateCellMat(int numCols, int numRows)
{
	CellMat cells(numRows);
	vector<Cell> cur_row;

	for (int i = 0; i < numRows; i++) {
		cells[i].reserve(numCols);

		for (int j = 0; j < numCols; j++)
			cells[i].push_back(Cell(i, j));
	}

	return cells;
}

vector<vector<vec2i>> Game::generateRoomsOn2DGrid(vec2i& gridOffset, vec2i& maxWH)
{
	/* create a matrix of rooms an connect them to one another with Corridors */
	vector<vector<vec2i>> roomsWH(MAX_ROOM_ROWS);

	/* room generation*/
	for (int i = 0; i < MAX_ROOM_ROWS; i++) {
		roomsWH[i].reserve(MAX_ROOM_COLS);
		int yOffset = i * gridOffset.y + ROOM_START_OFFSET_Y;
		for (int j = 0; j < MAX_ROOM_COLS; j++) {
			int xOffset = j * gridOffset.x + ROOM_START_OFFSET_X;
			int width = randomizeNumber(ROOM_WIDTH_MIN, maxWH.x);
			int height = randomizeNumber(ROOM_HEIGHT_MIN, maxWH.y);

			vec2f xyOffsets(static_cast<float>(xOffset), static_cast<float>(yOffset));
			CellMat cells = generateCellMat(width, height);
			rooms.push_back(Room(cells, xyOffsets));

			vec2i roomWH(width, height);
			roomsWH[i].push_back(roomWH);
		}
	}
	return roomsWH;
}

vector<vector<vec2i>> Game::generateCorridorsOn2DGrid(vector<vector<vec2i>>& roomsWH, vec2i &gridOffset)
{
	/* 2d matrix first array left to right, second array top to down */
	vector<vector<vec2i>> corridorsOffsets(2);

	/* corridor generation left to right */
	for (int i = 0; i < MAX_ROOM_ROWS; i++) {
		for (int j = 0; j < MAX_ROOM_COLS - 1; j++) {
			int yOffset = i * gridOffset.y + randomizeNumber(0, ROOM_HEIGHT_MIN);
			int xOffset = j * gridOffset.x + roomsWH[i][j].x;

			int width = ((j + 1) * gridOffset.x) - xOffset + ROOM_START_OFFSET_X;

			vec2f xyOffset(static_cast<float>(xOffset), static_cast<float>(yOffset));
			CellMat cells = generateCellMat(width, 1);

			corridors.push_back(Corridor(cells, xyOffset));
			corridorsOffsets[0].push_back(vec2i(xOffset, yOffset));
		}
	}

	/* corridor generation top to bottom */
	for (int i = 0; i < MAX_ROOM_COLS; i++) {
		for (int j = 0; j < MAX_ROOM_ROWS - 1; j++) {
			int xOffset = i * gridOffset.x + randomizeNumber(0, ROOM_WIDTH_MIN);
			int yOffset = j * gridOffset.y + roomsWH[j][i].y;

			int height = ((j + 1) * gridOffset.y) - yOffset + ROOM_START_OFFSET_Y;

			vec2f xyOffset(static_cast<float>(xOffset), static_cast<float>(yOffset));
			CellMat cells = generateCellMat(1, height);

			corridors.push_back(Corridor(cells, xyOffset));
			corridorsOffsets[1].push_back(vec2i(xOffset, yOffset));

		}
	}
	return corridorsOffsets;
}


void Game::connectRoomCorridorLeftToRight(int i, int j, vector<vec2i>& corridorsLeftToRight, vec2i& gridOffset)
{
	vec2i& offset = corridorsLeftToRight[i * (MAX_ROOM_COLS - 1) + j];

	Room& leftRoom = rooms[i * MAX_ROOM_COLS + j];
	Room& rightRoom = rooms[i * MAX_ROOM_COLS + j + 1];
	Corridor& corridor = corridors[i * (MAX_ROOM_COLS - 1) + j];
	int yCell = offset.y - i * gridOffset.y - ROOM_START_OFFSET_Y;

	/* add corridor to left room */
	CellMat& leftRoomCells = leftRoom.getCells();
	/* extract most right cell from the corridor offset */
	Cell* leftConnector = &leftRoomCells[yCell][leftRoomCells[0].size() - 1];
	map<Cell*, Corridor*>& leftCorridorConnector = leftRoom.getCorridorConnections();
	leftCorridorConnector[leftConnector] = &corridor;
	/* TODO check if needed */
	leftRoom.setCorridorConnections(leftCorridorConnector);

	/* add corrdior to right room */
	CellMat& rightRoomCells = rightRoom.getCells();
	/* extract most right cell from the corridor offset */
	Cell* rightConnector = &rightRoomCells[yCell][0];
	map<Cell*, Corridor*>& rightCorridorConnector = rightRoom.getCorridorConnections();
	rightCorridorConnector[rightConnector] = &corridor;
	/* TODO check if needed */
	rightRoom.setCorridorConnections(rightCorridorConnector);

	CellMat& corridorCells = corridor.getCells();
	map<Cell*, Room*>& roomConnections = corridor.getRoomConnections();
	/* connect left room */
	Cell* leftRoomConnector = &corridorCells[0][0];
	roomConnections[leftRoomConnector] = &leftRoom;
	/* connect right room */
	Cell* rightRoomConnector = &corridorCells[0][corridorCells[0].size() - 1];
	roomConnections[rightRoomConnector] = &rightRoom;

}

void Game::connectRoomCorridorTopToBottom(int i, int j, vector<vec2i>& corridorsTopToBottom, vec2i& gridOffset)
{
	vec2i& offset = corridorsTopToBottom[j * (MAX_ROOM_COLS - 1) + i];

	Room& topRoom = rooms[i * MAX_ROOM_COLS + j];
	Room& bottomRoom = rooms[(i+1) * MAX_ROOM_COLS + j];

	int numCorridorsLeftToRight = (MAX_ROOM_ROWS * (MAX_ROOM_COLS - 1));

	Corridor& corridor = corridors[numCorridorsLeftToRight + j * (MAX_ROOM_ROWS - 1) + i];
	int xCell = offset.x - j * gridOffset.x - ROOM_START_OFFSET_X;

	/* add corridor to top room */
	CellMat& topRoomCells = topRoom.getCells();
	/* extract most right cell from the corridor offset */
	Cell* topConnector = &topRoomCells[topRoomCells.size() - 1][xCell];
	map<Cell*, Corridor*>& topCorridorConnector = topRoom.getCorridorConnections();
	topCorridorConnector[topConnector] = &corridor;
	/* TODO check if needed */
	topRoom.setCorridorConnections(topCorridorConnector);

	/* add corrdior to right room */
	CellMat& bottomRoomCells = bottomRoom.getCells();
	/* extract most bottom cell from the corridor offset */
	Cell* bottomConnector = &bottomRoomCells[0][xCell];
	map<Cell*, Corridor*>& bottomCorridorConnector = bottomRoom.getCorridorConnections();
	bottomCorridorConnector[bottomConnector] = &corridor;
	/* TODO check if needed */
	bottomRoom.setCorridorConnections(bottomCorridorConnector);

	CellMat& corridorCells = corridor.getCells();
	map<Cell*, Room*>& roomConnections = corridor.getRoomConnections();
	/* connect top room */
	Cell* topRoomConnector = &corridorCells[0][0];
	roomConnections[topRoomConnector] = &topRoom;
	/* connect bottom room */
	Cell* bottomRoomConnector = &corridorCells[corridorCells[0].size() - 1][0];
	roomConnections[bottomRoomConnector] = &bottomRoom;
}


void Game::connectRoomsCorridors(vector<vector<vec2i>>& corrdirosOffsets, vec2i& gridOffset)
{
	/* connect rooms cells to corridors */
	vector<vec2i>& corridorsLeftToRight = corrdirosOffsets[0];
	vector<vec2i>& corridorsTopToBottom = corrdirosOffsets[1];

	/* corridor generation left to right */
	for (int i = 0; i < MAX_ROOM_ROWS; i++) {
		for (int j = 0; j < MAX_ROOM_COLS - 1; j++) {
			connectRoomCorridorLeftToRight(i, j, corridorsLeftToRight, gridOffset);
		}
	}

	/* corridor generation top to bottom */
	for (int i = 0; i < MAX_ROOM_ROWS - 1; i++) {
		for (int j = 0; j < MAX_ROOM_COLS; j++) {
			connectRoomCorridorTopToBottom(i, j, corridorsTopToBottom, gridOffset);
		}
	}
}


void Game::randomizeMap()
{
	vec2i gridOffset(Drawer::width / MAX_ROOM_COLS, Drawer::height / MAX_ROOM_ROWS);
	vec2i roomMaxWH(Drawer::width / (MAX_ROOM_COLS + 1), Drawer::height / (MAX_ROOM_ROWS + 1));

	vector<vector<vec2i>> roomsWH = generateRoomsOn2DGrid(gridOffset, roomMaxWH);
	vector<vector<vec2i>> corrdiorOffsets = generateCorridorsOn2DGrid(roomsWH, gridOffset);

	connectRoomsCorridors(corrdiorOffsets, gridOffset);
}

Game::Game()
{
	randomizeMap();
}

Game* Game::getInstance()
{
	if (game)
		return game;
	game = new Game();


	return game;
}

void Game::draw()
{
	for (auto& c : corridors)
		c.draw();

	for (auto& r : rooms)
		r.draw();

}

void Game::start()
{

}

Game* Game::game = nullptr;
