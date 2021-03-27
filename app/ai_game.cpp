#include <time.h>
#include "glut.h"
#include "Game.hpp"
#include <iostream>

using namespace std;

Game *game = nullptr;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	if (game)
		game->draw();

	glutSwapBuffers(); // show all
}

void idle()
{
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