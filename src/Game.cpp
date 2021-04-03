#include "Game.hpp"
#include "Bot.hpp"
#include "SupportBot.hpp"
#include "Team.hpp"
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
			cells[i].push_back(Cell(j, i));
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
			/* add -1 to force corridors to have at least 2 cells*/
			int width = randomizeNumber(ROOM_WIDTH_MIN, maxWH.x - 1);
			int height = randomizeNumber(ROOM_HEIGHT_MIN, maxWH.y - 1);

			vec2f xyOffsets(static_cast<float>(xOffset), static_cast<float>(yOffset));
			CellMat cells = generateCellMat(width, height);

			Room *r = new Room(cells, xyOffsets);
			rooms.push_back(r);

			CellMat& cellss = rooms.back()->getCells();
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
			int yOffset = i * gridOffset.y + randomizeNumber(1, ROOM_HEIGHT_MIN);
			int xOffset = j * gridOffset.x + roomsWH[i][j].x + ROOM_START_OFFSET_X;

			int width = ((j + 1) * gridOffset.x) - xOffset + ROOM_START_OFFSET_X;

			vec2f xyOffset(static_cast<float>(xOffset), static_cast<float>(yOffset));
			CellMat cells = generateCellMat(width, 1);

			corridors.push_back(new Corridor(cells, xyOffset));
			corridorsOffsets[0].push_back(vec2i(xOffset, yOffset));
		}
	}

	/* corridor generation top to bottom */
	for (int i = 0; i < MAX_ROOM_COLS; i++) {
		for (int j = 0; j < MAX_ROOM_ROWS - 1; j++) {
			int xOffset = i * gridOffset.x + randomizeNumber(1, ROOM_WIDTH_MIN);
			int yOffset = j * gridOffset.y + roomsWH[j][i].y + ROOM_START_OFFSET_Y;


			int height = ((j + 1) * gridOffset.y) - yOffset + ROOM_START_OFFSET_Y;

			vec2f xyOffset(static_cast<float>(xOffset), static_cast<float>(yOffset));
			CellMat cells = generateCellMat(1, height);

			corridors.push_back(new Corridor(cells, xyOffset));
			corridorsOffsets[1].push_back(vec2i(xOffset, yOffset));

		}
	}
	return corridorsOffsets;
}


void Game::connectRoomCorridorLeftToRight(int i, int j, vector<vec2i>& corridorsLeftToRight, vec2i& gridOffset)
{
	vec2i& offset = corridorsLeftToRight[i * (MAX_ROOM_COLS - 1) + j];

	Room& leftRoom = *rooms[i * MAX_ROOM_COLS + j];
	Room& rightRoom = *rooms[i * MAX_ROOM_COLS + j + 1];
	Corridor& corridor = *corridors[i * (MAX_ROOM_COLS - 1) + j];
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

	Room& topRoom = *rooms[i * MAX_ROOM_COLS + j];
	Room& bottomRoom = *rooms[(i+1) * MAX_ROOM_COLS + j];

	int numCorridorsLeftToRight = (MAX_ROOM_ROWS * (MAX_ROOM_COLS - 1));

	Corridor& corridor = *corridors[numCorridorsLeftToRight + j * (MAX_ROOM_ROWS - 1) + i];
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
	Cell* bottomRoomConnector = &corridorCells[corridorCells.size() - 1][0];
	roomConnections[bottomRoomConnector] = &bottomRoom;
	corridor.setRoomConnections(roomConnections);
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


void Game::createTeam(Team& t, int roomRow)
{
	constexpr auto NUM_ASSAULT_BOT = 2;
	constexpr auto NUM_SUPPORT_BOT = 0;

	for (auto i = 0; i < NUM_ASSAULT_BOT; i++) {
		int roomIndex = roomRow * MAX_ROOM_COLS + i;
		Room& r = *rooms[roomIndex];
		vec2f& rXY = r.getXYOffset();
		/* start location is cell 0, 0 of the cell */
		vec2f location(rXY.x + 0.5f, rXY.y + 0.5f);
		vec2f maxSpeed = vec2f(0.01f, 0.01f);

		Bot* bot = new Bot(MAX_HEALTH, MAX_BULLETS, MAX_GRENADES, t, location, maxSpeed, location, 0.9f, r);
		vector<Bot*> bots = t.getBots();
		bots.push_back(bot);
		/* probably not required */
		t.setBots(bots);
	}

	for (auto i = 0; i < NUM_SUPPORT_BOT; i++) {
		int roomIndex = roomRow * MAX_ROOM_COLS + i + NUM_ASSAULT_BOT;
		Room& r = *rooms[roomIndex];
		vec2f& rXY = r.getXYOffset();
		/* start location is cell 0, 0 of the cell */
		vec2f location(rXY.x, rXY.y);
		vec2f maxSpeed = vec2f(1, 1);

		Bot* bot = new SupportBot(MAX_HEALTH, MAX_BULLETS, MAX_GRENADES, t, location, maxSpeed, location, 0.9f, r);
		vector<Bot*> bots = t.getBots();
		bots.push_back(bot);
		/* probably not required */
		t.setBots(bots);
	}

	collisionLogic = new CollisionLogic();

}

void Game::createTeams()
{
	/* start bots on rows 0 and 2 */
	int roomRowIndecies[2] = { 0, 2 };

	for (auto& roomRow : roomRowIndecies) {
		teams.push_back(new Team());
		Team* t = teams.back();
		createTeam(*t, roomRow);
	}

	teams[0]->setTeamColor(DrawerColor::GREEN);
	teams[1]->setTeamColor(DrawerColor::YELLOW);
}

Game::Game() : collisionLogic(nullptr), gameOver(false)
{
	randomizeMap();
	addConsumbles();
	addObstacles();
	removeConsumablesOccupation();
}

Game::~Game()
{
	for (auto& team : teams) {
		delete team;
	}

	for (auto& room : rooms) {
		delete room;
	}

	for (auto& corridor : corridors) {
		delete corridor;
	}

}

void Game::removeConsumablesOccupation()
{
	for (auto& r : rooms) {
		vector<Consumable*>& ammoBoxes = r->getAmmoBoxes();
		vector<Consumable*>& healthBoxes = r->getHealthBoxes();
		for (auto& ab : ammoBoxes) {
			ab->getLocation()->setIsOccupy(false);
		}

		for (auto& hb : healthBoxes) {
			hb->getLocation()->setIsOccupy(false);
		}
	}
}

void Game::addConsumbles()
{
	int cellX;
	int cellY;
	constexpr bool isHidden = false;
	constexpr int numBullets = 20;
	constexpr int numGrenades = 20;
	constexpr int healthAmmount = 100;

	for (auto& r : rooms) {
		do {
			cellX = rand() % (r->getCells()[0].size() - 3) + 1;
			cellY = rand() % (r->getCells().size() - 3) + 1;
		} while (r->getCells()[cellY][cellX].getIsOccupy());
		r->addAmmoBox(cellX, cellY, numBullets, numGrenades, isHidden);
	}

	for (auto& r : rooms) {
		do {
			cellX = rand() % (r->getCells()[0].size() - 3) + 1;
			cellY = rand() % (r->getCells().size() - 3) + 1;
		} while (r->getCells()[cellY][cellX].getIsOccupy());
		r->addHealthBox(cellX, cellY, healthAmmount, isHidden);
	}
}

void Game::addObstacles()
{
	int cellX;
	int cellY;
	bool isHorizontal = false;
	constexpr int healthPoints = 10;
	constexpr int destroyFrame = 100;

	for (auto& r : rooms) {
		vector<Cell*> cover;
		int maxWidth = (r->getCells()[0].size() - 1);
		int maxHeight = (r->getCells().size() - 1);
		do {
			cellX = rand() % (maxWidth - 2) + 1;
			cellY = rand() % (maxHeight - 2) + 1;
		} while (r->getCells()[cellY][cellX].getIsOccupy());

		if (isHorizontal) {
			for (int i = cellX; i < maxWidth; i++) {
				Cell* c = &r->getCells()[cellY][i];
				if (c->getIsOccupy())
					break;
				cover.push_back(c);
			}
		}
		else {
			for (int j = cellY; j < maxHeight; j++) {
				Cell* c = &r->getCells()[j][cellX];
				if (c->getIsOccupy())
					break;
				cover.push_back(c);
			}
		}
		
		r->addWall(healthPoints, destroyFrame,cover);

		isHorizontal = !isHorizontal;
	}
}

void Game::killGameBot(Bot* bot)
{
	if (!game) {
		return;
	}

	//game->lock.lock();
	for (auto& t : Game::getGameTeams()) {
		for (auto& b : t->getBots()) {
			if (bot == b) {
				t->removeBot(bot);
				return;
			}
		}
	}
	//game->lock.unlock();

}


Game* Game::getInstance()
{
	if (game)
		return game;
	game = new Game();


	return game;
}

Room* Game::getRoom(vec2f& cellLocation)
{
	for (auto& room : rooms) {
		if (room->isRelated2CellLocation(cellLocation))
			return room;
	}
	return nullptr;
}

void Game::draw()
{
	for (auto& c : corridors)
		c->draw();

	for (auto& r : rooms)
		r->draw();

	for (auto& t : teams)
		t->draw();

}

void Game::updateGameOver()
{
	int numAliveTeams = 0;
	for (auto& t : teams) {
		if (t->getBots().size() > 0)
			numAliveTeams++;
	}

	if (numAliveTeams <= 1) {
		gameOver = true;
	}
}

void Game::update()
{
	if (gameOver) {
		for (auto& t: teams) {
			if (t->getBots().size() > 0)
				cout << "Team " <<static_cast<int>(teams[0]->getTeamColor()) << " has Won, ";
		}
		cout << "Game Over " << endl;
		return;
	}

	for (auto& team : teams) {
		team->update();
	}

	if (collisionLogic) {
		collisionLogic->handleCollisions();
	}

	updateGameOver();
}

Game* Game::game = nullptr;
