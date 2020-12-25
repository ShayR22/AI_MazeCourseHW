#include "MazeGame.h"
#include "OpenGL.h"

static float playerDx()
{
	constexpr auto PLAYER_MOVEMENT_PER_FRAME = 1;
	return 2 * 1.0f / OpenGL::width * PLAYER_MOVEMENT_PER_FRAME;
}

static float playerDy()
{
	constexpr auto PLAYER_MOVEMENT_PER_FRAME = 1;
	return 2 * 1.0f / OpenGL::height * PLAYER_MOVEMENT_PER_FRAME;
}

static float playerX(Cell *start)
{
	return start->getX() + 0.5f;
}

static float playerY(Cell* start)
{
	return start->getY() + 0.5f;
}

MazeGame::MazeGame(int size)
{
	solved = false;

	numCoins = size * size;
	maze = new Maze(size, size);
	start = maze->getStarts()[0];

	cell_mat& cells = maze->getCells();
	for (auto& cellsRow : cells)
		for (auto& cell : cellsRow)
			cell.setHasCoin(true);

	float sx = playerX(start);
	float sy = playerY(start);
	float dx = playerDx();
	float dy = playerDy();
	float tx = maze->getTarget().getX() + 0.5f;
	float ty = maze->getTarget().getY() + 0.5f;
	player = new Player(cells, sx, sy, dx, dy, tx, ty, &numCoins);
}

MazeGame::~MazeGame()
{
	delete maze;
	delete player;
}

void MazeGame::draw()
{
	maze->draw();
	player->draw();
}

void MazeGame::solveIteration()
{
	if (solved)
		return;

	player->move();
	if (numCoins == 0)
		solved = true;

}