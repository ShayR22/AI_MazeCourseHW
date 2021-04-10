#include <time.h>
#include "glut.h"
#include "Game.hpp"
#include "Drawer.hpp"
#include <iostream>
#include <chrono>
#include "PathFinder.hpp"
#include "GamePoint.hpp"
#include "GamePointEdge.hpp"
#include "Bot.hpp"

constexpr auto SCREEN_WIDTH = 1000;
constexpr auto SCREEN_HEIGHT = 1000;


using namespace std;

constexpr auto FRAME_RATE = 144;
constexpr int MILLI_PER_FRAME = 1000 / FRAME_RATE;

auto frameCounterTime = chrono::high_resolution_clock::now();
long lastFrameCounter = 0;
long frameCounter = 0;
long fps = 0;

auto lastTick = chrono::high_resolution_clock::now();
Game *game = nullptr;

void drawText(const std::string& text, const unsigned int x, const unsigned int y, const float r, const float g, const float b)
{
	void* m_glutFont = GLUT_BITMAP_9_BY_15;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(r, g, b);
	glRasterPos2i(x, y);
	for (const char c : text)
		glutBitmapCharacter(m_glutFont, (int)c);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void updateFrameRateDrawing()
{
	frameCounter++;
	chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> diffTime = now - frameCounterTime;

	if (diffTime.count() >= 1000) {
		fps = frameCounter - lastFrameCounter;
		lastFrameCounter = frameCounter;
		frameCounterTime = now;
	}

	string fpsStr(std::to_string(fps));
	drawText(fpsStr, 0, 0, 0, 0, 0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	if (game)
		game->draw();

	updateFrameRateDrawing();
	glutSwapBuffers(); // show all
}

static void keepFrameRate()
{
	chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> diffTime = now - lastTick;
	int amountToSleepMS = MILLI_PER_FRAME - static_cast<int>(diffTime.count());
	if (amountToSleepMS > 0) {
		this_thread::sleep_for(chrono::milliseconds(amountToSleepMS));
	}

	lastTick = chrono::high_resolution_clock::now();
}

void idle()
{
	game->update();
	keepFrameRate();
	glutPostRedisplay(); // indirect call to display
}

void restartGame()
{
	delete game;
	game = Game::getInstance();
}

void init()
{
	glClearColor(0.8f, 0.7f, 0.5f, 0.0f);// color of window background
	glOrtho(-1, 1, -1, 1, 1, -1);
	srand((unsigned int)time(0));

	game = Game::getInstance();
	game->createTeams();
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

	init();
	glutMainLoop();

	if (game)
		delete game;

	return 0;
}