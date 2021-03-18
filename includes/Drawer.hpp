#ifndef __DRAWER__
#define __DRAWER__

enum class DrawerColor{
	WHITE,
	BLACK,
	RED,
	BLUE,
	GREEN
};

class Drawer {
private:
	static void drawRect(float xGL, float yGL, float wGL, float hGL);
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
};

#endif
