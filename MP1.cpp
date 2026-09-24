#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

// game state
float playerX = 0.0f;
float playerY = -0.7f;

// asteroid 1
float asteroid1X = -0.4f;
float asteroid1Y = 1.0f;
float asteroid1Speed = 0.015f;

// asteroid 2
float asteroid2X = 0.4f;
float asteroid2Y = 1.3f;
float asteroid2Speed = 0.022f;

int score = 0;
int lives = 3;
bool isGameOver = false;

void drawBitmapString(void* font, const char* str)
{
    for (const char* c = str; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

// ship
void drawShip()
{
    // main body
    glColor3f(0.0f, 0.6f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(playerX - 0.05f, playerY - 0.05f);
        glVertex2f(playerX + 0.05f, playerY - 0.05f);
        glVertex2f(playerX + 0.05f, playerY + 0.05f);
        glVertex2f(playerX - 0.05f, playerY + 0.05f);
    glEnd();

    // left wing
    glColor3f(0.0f, 0.9f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(playerX - 0.05f, playerY - 0.05f);
        glVertex2f(playerX - 0.12f, playerY - 0.08f);
        glVertex2f(playerX - 0.05f, playerY + 0.02f);
    glEnd();

    // right wing
    glBegin(GL_TRIANGLES);
        glVertex2f(playerX + 0.05f, playerY - 0.05f);
        glVertex2f(playerX + 0.12f, playerY - 0.08f);
        glVertex2f(playerX + 0.05f, playerY + 0.02f);
    glEnd();

    // cockpit
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(playerX, playerY + 0.09f);
        glVertex2f(playerX - 0.03f, playerY + 0.02f);
        glVertex2f(playerX + 0.03f, playerY + 0.02f);
    glEnd();
}

void drawAsteroid(float x, float y)
{
    glColor3f(0.8f, 0.3f, 0.1f); // Rusty Orange-Red
    glBegin(GL_QUADS);
        glVertex2f(x - 0.06f, y - 0.06f);
        glVertex2f(x + 0.06f, y - 0.06f);
        glVertex2f(x + 0.06f, y + 0.06f);
        glVertex2f(x - 0.06f, y + 0.06f);
    glEnd();
}

void drawHUD()
{
    char buffer[50];

    // score
    snprintf(buffer, sizeof(buffer), "Score: %d", score);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.9f, 0.85f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_18, buffer);

    // lives
    snprintf(buffer, sizeof(buffer), "Lives: %d", lives);
    glRasterPos2f(0.7f, 0.85f);
    drawBitmapString(GLUT_BITMAP_HELVETICA_18, buffer);

    if (isGameOver)
    {
        // game over banner
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.35f, 0.1f);
        drawBitmapString(GLUT_BITMAP_HELVETICA_18, "GAME OVER");

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.45f, -0.05f);
        drawBitmapString(GLUT_BITMAP_HELVETICA_18, "Press [R] to Restart or [ESC] to Exit");
    }
    else
    {
        // controls
        glColor3f(0.7f, 0.7f, 0.7f);
        glRasterPos2f(-0.9f, -0.9f);
        drawBitmapString(GLUT_BITMAP_HELVETICA_12, "Controls: [W][A][S][D] to Move | [R] to Restart | [ESC] to Quit");
    }
}

bool checkCollision(float ax, float ay)
{
    float distanceX = playerX - ax;
    float distanceY = playerY - ay;

    if (distanceX < 0) distanceX = -distanceX;
    if (distanceY < 0) distanceY = -distanceY;

    return distanceX < 0.11f && distanceY < 0.11f;
}

void resetGame()
{
    playerX = 0.0f;
    playerY = -0.7f;
    score = 0;
    lives = 3;
    asteroid1X = -0.4f;
    asteroid1Y = 1.0f;
    asteroid2X = 0.4f;
    asteroid2Y = 1.3f;
    isGameOver = false;
    cout << "Game restarted." << endl;
}

void updateGame(int value)
{
    if (!isGameOver)
    {
        asteroid1Y -= asteroid1Speed;
        asteroid2Y -= asteroid2Speed;

        score += 1;

        if (asteroid1Y < -1.1f)
        {
            asteroid1Y = 1.1f;
            asteroid1X = (float)((rand() % 16) - 8) / 10.0f;
        }

        if (asteroid2Y < -1.1f)
        {
            asteroid2Y = 1.2f;
            asteroid2X = (float)((rand() % 16) - 8) / 10.0f;
        }

        if (checkCollision(asteroid1X, asteroid1Y))
        {
            lives--;
            cout << "Collision! Lives remaining: " << lives << endl;
            asteroid1Y = 1.1f;
            if (lives <= 0) isGameOver = true;
        }

        if (checkCollision(asteroid2X, asteroid2Y))
        {
            lives--;
            cout << "Collision! Lives remaining: " << lives << endl;
            asteroid2Y = 1.1f;
            if (lives <= 0) isGameOver = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateGame, 0); // 60 fps rate
}

void display()
{
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!isGameOver)
    {
        drawShip();
        drawAsteroid(asteroid1X, asteroid1Y);
        drawAsteroid(asteroid2X, asteroid2Y);
    }
    drawHUD();
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
        case 'W':
            if (!isGameOver) playerY += 0.06f;
            break;
        case 's':
        case 'S':
            if (!isGameOver) playerY -= 0.06f;
            break;
        case 'a':
        case 'A':
            if (!isGameOver) playerX -= 0.06f;
            break;
        case 'd':
        case 'D':
            if (!isGameOver) playerX += 0.06f;
            break;
        case 'r':
        case 'R':
            resetGame();
            break;
        case 27: // esc key
            exit(0);
            break;
    }

    if (playerX > 0.85f) playerX = 0.85f;
    if (playerX < -0.85f) playerX = -0.85f;
    if (playerY > 0.75f) playerY = 0.75f;
    if (playerY < -0.85f) playerY = -0.85f;

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 700);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Machine Problem 1 - The Cosmic Dodger");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, updateGame, 0);
    glutMainLoop();
    return 0;
}
