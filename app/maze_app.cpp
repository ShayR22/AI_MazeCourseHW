#include <time.h>
#include "glut.h"
#include "Maze.h"

constexpr int MSIZE = 25;

Maze* maze;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	maze->drawMaze();
	glutSwapBuffers(); // show all
}


void idle()
{
	/* FIXME */
}

void menu(int choice)
{
	/* FIXME */
	/* cast to remove warnings */
	(void)choice;

	//switch (choice)
	//{
	//case 1: // BFS
	//	bfs_is_on = true;
	//	break;
	//case 2: // DFS
	//	break;
	//}
}

void init()
{
	glClearColor(0.8, 0.7, 0.5, 0);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);
	srand(time(0));

	maze = new Maze(MSIZE, MSIZE);
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 0);
	glutCreateWindow("First Example");
	glEnable(GL_COLOR_MATERIAL);

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("BFS", 1);
	glutAddMenuEntry("DFS", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();
	glutMainLoop();

	delete maze;
}