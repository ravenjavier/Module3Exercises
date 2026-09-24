#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <ctime>
using namespace std;

// game state variables
int score = 0;
bool isGameOver = false;

// color codes: 0 - Red, 1 - Green, 2 - Blue
int playerColorCode = 0;
int barrierColorCode = 1;

// barrier movement variables
float barrierY = 1.0f;
float barrierSpeed = 0.012f;
const float maxBarrierSpeed = 0.05f;

void drawText(float x, float y, const string& text) 
{ 
    glRasterPos2f(x, y); 
    for (char c : text) 
    { 
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c); 
    } 
}

void setOpenGLColor(int colorCode)
{
    if (colorCode == 0)
        glColor3f(1.0f, 0.0f, 0.0f); // Red
    else if (colorCode == 1)
        glColor3f(0.0f, 1.0f, 0.0f); // Green
    else if (colorCode == 2)
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
}

void drawPlayer()
{
    setOpenGLColor(playerColorCode);
    glBegin(GL_QUADS);
        glVertex2f(-0.15f, -0.55f);
        glVertex2f(0.15f, -0.55f);
        glVertex2f(0.15f, -0.25f);
        glVertex2f(-0.15f, -0.25f);
    glEnd();
}

void drawBarrier()
{
    setOpenGLColor(barrierColorCode);
    glBegin(GL_QUADS);
        glVertex2f(-0.4f, barrierY);
        glVertex2f(0.4f, barrierY);
        glVertex2f(0.4f, barrierY + 0.12f);
        glVertex2f(-0.4f, barrierY + 0.12f);
    glEnd();
}

void drawDashboard()
{
    char buffer[50];
    glColor3f(1.0f, 1.0f, 1.0f);

    drawText(-0.9f, 0.85f, "=== CHROMA-SHIFT: COLOR-MATCHING GATE ===");
    
    snprintf(buffer, sizeof(buffer), "Score: %d", score);
    drawText(-0.9f, 0.72f, buffer);

    string colorName = (playerColorCode == 0) ? "RED" : (playerColorCode == 1) ? "GREEN" : "BLUE";
    drawText(-0.9f, 0.62f, "Player Color: " + colorName);

    drawText(-0.9f, 0.48f, "Controls: [R]-Red | [G]-Green | [B]-Blue | [ESC]-Exit");
    drawText(-0.9f, 0.38f, "Rule: Match barrier color! Mismatch = GAME OVER.");

    if (isGameOver)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawText(-0.35f, 0.0f, "GAME OVER - Press [R] to Restart");
    }
}

bool colorsMatch()
{
    return playerColorCode == barrierColorCode;
}

void spawnNextBarrier()
{
    barrierY = 1.0f;
    barrierColorCode = rand() % 3;
}

void checkGateCollision()
{
    if (barrierY <= -0.4f && barrierY >= -0.55f)
    {
        if (colorsMatch())
        {
            score++;
            cout << "Correct Match! Score: " << score << endl;
            
            if (barrierSpeed < maxBarrierSpeed)
            {
                barrierSpeed += 0.002f;
            }
            
            spawnNextBarrier();
        }
        else
        {
            cout << "Mismatch! Game Over." << endl;
            isGameOver = true;
        }
    }
}

void updateBarrier(int value)
{
    if (!isGameOver)
    {
        barrierY -= barrierSpeed;

        checkGateCollision();

        if (barrierY < -1.0f)
        {
            cout << "Missed gate! Game Over." << endl;
            isGameOver = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateBarrier, 0); // 60 fps rate
}

void display()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawDashboard();
    if (!isGameOver)
    {
        drawPlayer();
        drawBarrier();
    }
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'r':
        case 'R':
            if (isGameOver)
            {
                score = 0;
                barrierSpeed = 0.012f;
                isGameOver = false;
                spawnNextBarrier();
                cout << "Game restarted." << endl;
            }
            else
            {
                playerColorCode = 0; // red
                cout << "Player Color set to RED" << endl;
            }
            break;

        case 'g':
        case 'G':
            if (!isGameOver)
            {
                playerColorCode = 1; // green
                cout << "Player Color set to GREEN" << endl;
            }
            break;

        case 'b':
        case 'B':
            if (!isGameOver)
            {
                playerColorCode = 2; // blue
                cout << "Player Color set to BLUE" << endl;
            }
            break;

        case 27: // esc key
            exit(0);
            break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    srand(time(0));
    glutInitWindowSize(800, 700);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Machine Problem 2 - Chroma-Shift: The Color-Matching Gate");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, updateBarrier, 0);
    glutMainLoop();
    return 0;
}
