#include "Player.h"
#include "OpenGL.h"
#include <iostream>

constexpr bool PLAYER_DEBUG = false;

Player::Player(cell_mat& cells, float x, float y, float maxDx, float maxDy, float targetX, float targetY, int& numCoins)
    : MazeMovingObj(cells, x, y, maxDx, maxDy, targetX, targetY), numCoins(numCoins),
    enemySearchRadius((float)sqrt(numCoins)/4.50f),  lastTick(std::chrono::high_resolution_clock::now())
{
    lastCellX = (int)x;
    lastCellY = (int)y;
    cells[lastCellX][lastCellY].setHasCoin(false);

    this->numCoins--;
}

void drawLastLocation(float lastX, float lastY)
{
    float y = -(2 * (float)lastX / OpenGL::width - 1);
    float x = (2 * (float)lastY / OpenGL::height - 1);

    /* draw black body and draw body yellow a bit smaller, in order to create a black
     * layer creating clear definition for packman
     */
    glPointSize(1 / OpenGL::circleR / 1.2f);
    glBegin(GL_POINTS);
    glColor3d(0, 0, 0);
    glVertex2f(x, y);
    glEnd();

    glPointSize(1 / OpenGL::circleR / 1.5f);
    glBegin(GL_POINTS);
    glColor3d(1, 0.5, 0.5);
    glVertex2f(x, y);
    glEnd();

}

void Player::draw()
{
    float y = -(2 * (float)this->x / OpenGL::width - 1);
    float x = (2 *(float)this->y / OpenGL::height - 1);

    /* draw black body and draw body yellow a bit smaller, in order to create a black 
     * layer creating clear definition for packman   
     */
    glPointSize(1 / OpenGL::circleR / 1.2f);
    glBegin(GL_POINTS);
    glColor3d(0, 0, 0);
    glVertex2f(x, y);
    glEnd();

    glPointSize(1 / OpenGL::circleR / 1.5f);
    glBegin(GL_POINTS);
    glColor3d(1, 1, 0);
    glVertex2f(x, y);
    glEnd();

    /* put location of the eye in the direction of movement*/
    y -= OpenGL::circleR * (dx / maxDx) / 4.5f;
    x += OpenGL::circleR * (dy / maxDy) / 4.5f;

    /* draw sort of an eye */
    glPointSize(1 / OpenGL::circleR / 4.0f);
    glBegin(GL_POINTS);
    glColor3d(0, 0, 0);
    glVertex2f(x, y);
    glEnd();

    if (PLAYER_DEBUG) {
        /* draw player's enemies radius detaction*/
        double openglW = 2 * enemySearchRadius / OpenGL::width;
        glColor3d(1, 0, 0);
        glBegin(GL_LINE_LOOP);
        glVertex2d(x, y - openglW);
        glVertex2d(x + openglW, y);
        glVertex2d(x, y + openglW);
        glVertex2d(x - openglW, y);
        glEnd();
    }
}

void Player::updateLastLocation()
{
    if ((int)x == lastCellX && (int)y == lastCellY)
        return;

    lastCellX = (int)x;
    lastCellY = (int)y;

    if (cells[lastCellX][lastCellY].getHasCoin()) {
        cells[lastCellX][lastCellY].setHasCoin(false);
        numCoins--;
    }
}