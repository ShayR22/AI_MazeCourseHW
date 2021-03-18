#include "Drawer.hpp"
#include "vec2.h"
#include "glut.h"

#include <iostream>

using namespace std;

constexpr auto CIRCLE_NUM_SEGMENTS = 20;
constexpr auto EPSILLON = 0.001f;

void Drawer::setColor(DrawerColor color)
{
	switch (color) {
	case DrawerColor::WHITE:
		glColor3d(1, 1, 1);
		break;
	case DrawerColor::BLACK:
		glColor3d(0, 0, 0);
		break;
	case DrawerColor::RED:
		glColor3d(1, 0, 0);
		break;
	case DrawerColor::BLUE:
		glColor3d(0, 0, 1);
		break;
	case DrawerColor::GREEN:
		glColor3d(0, 1, 0);
		break;
	}
}

void Drawer::drawRect(float xGL, float yGL, float wGL, float hGL)
{
	glBegin(GL_POLYGON);
	glVertex2f(xGL, yGL);
	glVertex2f(xGL, yGL + hGL);
	glVertex2f(xGL + wGL, yGL + hGL);
	glVertex2f(xGL + wGL, yGL);
	glEnd();
}

void Drawer::rect(float x, float y, float w, float h, DrawerColor color)
{
	setColor(color);
	float xGL = 2 * (x / width) - 1;
	float yGL = -(2 * y / height - 1);
	float wGL = w / width * 2;
	float hGL = -h / height * 2;

	drawRect(xGL, yGL, wGL, hGL);
}

void Drawer::rectWithGrid(float x, float y, float w, float h, DrawerColor color)
{

	float xGL = 2 * x / width - 1;
	float yGL = -(2 * y / height - 1);
	float wGL = w / width * 2;
	float hGL = -h / height * 2;

	//cout << "xGL" << xGL << " yGL " << yGL << " wGL " << wGL << " hGL" << hGL << endl;

	setColor(color);
	drawRect(xGL, yGL, wGL, hGL);

	float xStart = xGL;
	float xEnd = xGL + wGL + EPSILLON;
	float yStart = yGL;
	float yEnd = yGL + hGL - EPSILLON;

	setColor(DrawerColor::BLACK);
	glBegin(GL_LINES);
	for (float i = xStart; i <= xEnd; i += 2 * Drawer::cubeW) {
		glVertex2f(i, yGL);
		glVertex2f(i, yGL + hGL);
	}

	for (float i = yStart; i >= yEnd; i -= 2 * Drawer::cubeH) {
		glVertex2f(xGL, i);
		glVertex2f(xGL + wGL, i);
	}
	glEnd();
}


void Drawer::circle(float cx, float cy, float diameter, DrawerColor color) {
	float cxGL = 2 * (cx / width) - 1;
	float cyGL = -(2 * ((cy / height)) - 1);
	float rGL = diameter / width / 2;

	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < CIRCLE_NUM_SEGMENTS; ii++) {
		float theta = 2.0f * (float)M_PI * ii / float(CIRCLE_NUM_SEGMENTS);
		float x = rGL * cosf(theta);
		float y = rGL * sinf(theta);
		glVertex2f(x + cxGL, y + cyGL);
	}
	glEnd();
}

/* default values to be initalize too. */
int Drawer::width = 40;
int Drawer::height = 40;
float Drawer::circleR = 1.0f / Drawer::width;
float Drawer::cubeW = 1.0f / Drawer::width;
float Drawer::cubeH = 1.0f / Drawer::height;