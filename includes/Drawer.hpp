#ifndef __DRAWER__
#define __DRAWER__

#include "glut.h"

enum class DrawerColor{
	WHITE,
	BLACK,
	RED,
	BLUE,
	GREEN,
	PURPULE
};

class Drawer {
private:
	static void drawRect(float xGL, float yGL, float wGL, float hGL);
	static void drawCircle(GLenum glMode, float cxGL, float cyGL, float rGL);
	static void setColor(DrawerColor color);
public:
	static int width;
	static int height;
	static float circleR;
	static float cubeW;
	static float cubeH;


	static void rect(float x, float y, float w, float h, DrawerColor color);
	static void rectWithGrid(float x, float y, float w, float h, DrawerColor color);
	static void circle(float cx, float cy, float diamater, DrawerColor color);
	static void filledCircle(float cx, float cy, float diamater, DrawerColor color);
	static void line(float sx, float sy, float tx, float ty, DrawerColor color);

};

#endif
