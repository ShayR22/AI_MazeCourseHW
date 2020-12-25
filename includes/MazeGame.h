#ifndef __MAZE_GAME__
#define __MAZE_GAME__

#include "Drawable.h"
#include "Solvable.h"
#include "Maze.h"
#include "Cell.h"
#include "Player.h"

class MazeGame : public Drawable, public Solveable {
private:
	int numCoins;
	Maze *maze;
	Cell* start;
	Player *player;
public:
	MazeGame(int size);
	~MazeGame();
	virtual void draw() override;
	virtual void solveIteration() override;
};


#endif // !__MAZE_GAME__
