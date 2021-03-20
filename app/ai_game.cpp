#include <time.h>
#include "glut.h"
#include "Game.hpp"
#include <iostream>

using namespace std;

Game *game = nullptr;

CellMat cells = {
	{Cell(0,0), Cell(0,0), Cell(0,0)},
	{Cell(0,0), Cell(0,0), Cell(0,0)},
	{Cell(0,0), Cell(0,0), Cell(0,0)},
	{Cell(0,0), Cell(0,0), Cell(0,0)},
	{Cell(0,0), Cell(0,0), Cell(0,0)},
	{Cell(0,0), Cell(0,0), Cell(0,0)}
};

vec2f xyOffset(0, 0);
Room r(cells, xyOffset);

CellMat cellsC = {
	{Cell(0,0), Cell(0,0), Cell(0,0)},
	{Cell(0,0), Cell(0,0), Cell(0,0)},
	{Cell(0,0), Cell(0,0), Cell(0,0)},
	{Cell(0,0), Cell(0,0), Cell(0,0)},
	{Cell(0,0), Cell(0,0), Cell(0,0)},
	{Cell(0,0), Cell(0,0), Cell(0,0)}
};

vec2f xyOffsetC(4, 3);
Corridor c(cellsC, xyOffsetC);


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	if (game)
		game->draw();
	//r.draw();
	//c.draw();
	glutSwapBuffers(); // show all
}


int i = 0;
void idle()
{
	//manager.solveIteration();
	if(++i % 250 == 0) {
		//vec2f xyOffset = r.getXYOffset();
		//xyOffset.x += 1;

		//r.setXYOffset(xyOffset);
		//cout << "x = " << xyOffset.x << " y = " << xyOffset.y << endl;

	}

	//Sleep(10);
	glutPostRedisplay(); // indirect call to display
}


void menu(int choice)
{
	
}

void init()
{
	glClearColor(0.8f, 0.7f, 0.5f, 0.0f);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);
	srand((unsigned int)time(0));

	game = new Game();

	int a = 2;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(400, 0);
	glutCreateWindow("First Example");

	/* make drawing points to draw circle instead of pixel */
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Restart", 0);
	glutAddMenuEntry("Graph", 1);
	glutAddMenuEntry("Maze", 2);
	glutAddMenuEntry("MAZE_BFS", 3);
	glutAddMenuEntry("MAZE_BFS BI", 4);
	glutAddMenuEntry("MAZE_A_STAR", 5);
	glutAddMenuEntry("GRAPH_A_STAR", 6);
	glutAddMenuEntry("GRAPH_BEST_FIRST_SEARCH", 7);
	glutAddMenuEntry("MAZE_GAME", 8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();
	glutMainLoop();

	if (game)
		delete game;

	return 0;
}