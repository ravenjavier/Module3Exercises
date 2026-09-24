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
#include <cmath>
#include <ctime>
using namespace std;

const int MAX_BURSTS = 10;

struct Burst {
    bool active;
    int type;
    float x, y;
    float radius;
    float maxRadius;
    float alpha;
    float r, g, b;
};

Burst bursts[MAX_BURSTS];
int activeBurstCount = 0;

void drawText(float x, float y, const string& text) 
{ 
    glRasterPos2f(x, y); 
    for (char c : text) 
    { 
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c); 
    } 
}

void spawnBurst(int type, float x, float y)
{
    for (int i = 0; i < MAX_BURSTS; i++)
    {
        if (!bursts[i].active)
        {
            bursts[i].active = true;
            bursts[i].type = type;
            bursts[i].x = x;
            bursts[i].y = y;
            bursts[i].radius = 0.02f;
            bursts[i].maxRadius = 0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.3f));
            bursts[i].alpha = 1.0f;

            if (type == 1) {
                bursts[i].r = 1.0f; bursts[i].g = 0.3f; bursts[i].b = 0.3f; // red / pink
            } else if (type == 2) {
                bursts[i].r = 0.3f; bursts[i].g = 1.0f; bursts[i].b = 0.4f; // green
            } else {
                bursts[i].r = 0.2f; bursts[i].g = 0.6f; bursts[i].b = 1.0f; // blue and cyan
            }

            cout << "Spawned Burst Type " << type << " at (" << x << ", " << y << ")" << endl;
            break;
        }
    }
}

void drawBurst(const Burst& b)
{
    glColor3f(b.r * b.alpha, b.g * b.alpha, b.b * b.alpha);

    if (b.type == 1) 
    {
        // concentric squares
        glBegin(GL_LINE_LOOP);
            glVertex2f(b.x - b.radius, b.y - b.radius);
            glVertex2f(b.x + b.radius, b.y - b.radius);
            glVertex2f(b.x + b.radius, b.y + b.radius);
            glVertex2f(b.x - b.radius, b.y + b.radius);
        glEnd();

        if (b.radius > 0.1f) {
            glBegin(GL_LINE_LOOP);
                glVertex2f(b.x - (b.radius * 0.5f), b.y - (b.radius * 0.5f));
                glVertex2f(b.x + (b.radius * 0.5f), b.y - (b.radius * 0.5f));
                glVertex2f(b.x + (b.radius * 0.5f), b.y + (b.radius * 0.5f));
                glVertex2f(b.x - (b.radius * 0.5f), b.y + (b.radius * 0.5f));
            glEnd();
        }
    } 
    else if (b.type == 2) 
    {
        // diamond Lattice
        glBegin(GL_LINE_LOOP);
            glVertex2f(b.x, b.y + b.radius);
            glVertex2f(b.x + b.radius, b.y);
            glVertex2f(b.x, b.y - b.radius);
            glVertex2f(b.x - b.radius, b.y);
        glEnd();

        float innerR = b.radius * 0.6f;
        glBegin(GL_LINES);
            glVertex2f(b.x - innerR, b.y); glVertex2f(b.x + innerR, b.y);
            glVertex2f(b.x, b.y - innerR); glVertex2f(b.x, b.y + innerR);
        glEnd();
    } 
    else if (b.type == 3) 
    {
        // starburst Triangles
        int numRays = 8;
        glBegin(GL_LINES);
        for (int i = 0; i < numRays; i++) {
            float angle = i * (2.0f * 3.14159f / numRays);
            float innerX = b.x + (b.radius * 0.3f) * cos(angle);
            float innerY = b.y + (b.radius * 0.3f) * sin(angle);
            float outerX = b.x + b.radius * cos(angle);
            float outerY = b.y + b.radius * sin(angle);
            glVertex2f(innerX, innerY);
            glVertex2f(outerX, outerY);
        }
        glEnd();
    }
}

void updateBursts(int value)
{
    activeBurstCount = 0;
    for (int i = 0; i < MAX_BURSTS; i++)
    {
        if (bursts[i].active)
        {
            // expansion
            bursts[i].radius += 0.008f;
            // fade out
            bursts[i].alpha -= 0.02f;
            if (bursts[i].alpha <= 0.0f || bursts[i].radius >= bursts[i].maxRadius)
            {
                bursts[i].active = false;
            }
            else
            {
                activeBurstCount++;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateBursts, 0); // 60 fps
}

void drawSidebar()
{
    char buffer[60];
    glColor3f(0.9f, 0.9f, 0.9f);

    drawText(-0.95f, 0.88f, "=== DYNAMIC FIREWORKS STUDIO ===");
    drawText(-0.95f, 0.78f, "[1] - Square Burst");
    drawText(-0.95f, 0.70f, "[2] - Diamond Lattice");
    drawText(-0.95f, 0.70f, ""); // spacing
    drawText(-0.95f, 0.62f, "[3] - Starburst Triangles");
    drawText(-0.95f, 0.54f, "[SPACE] - Random Burst");

    snprintf(buffer, sizeof(buffer), "Active Bursts: %d / %d", activeBurstCount, MAX_BURSTS);
    drawText(-0.95f, 0.40f, buffer);

    drawText(-0.95f, -0.85f, "[ESC] - Exit Program");
}

void display()
{
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawSidebar();
    for (int i = 0; i < MAX_BURSTS; i++)
    {
        if (bursts[i].active)
        {
            drawBurst(bursts[i]);
        }
    }
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    float randX = -0.4f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.8f));
    float randY = -0.4f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.8f));

    switch (key)
    {
        case '1':
            spawnBurst(1, randX, randY);
            break;
        case '2':
            spawnBurst(2, randX, randY);
            break;
        case '3':
            spawnBurst(3, randX, randY);
            break;
        case ' ':
            {
                int randomType = (rand() % 3) + 1;
                spawnBurst(randomType, randX, randY);
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
    // initialize all the bursts as inactive
    for (int i = 0; i < MAX_BURSTS; i++) {
        bursts[i].active = false;
    }
    glutInitWindowSize(900, 700);
    glutInitWindowPosition(150, 100);
    glutCreateWindow("Machine Problem 3 - Dynamic Fireworks & Shape Burst Studio");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, updateBursts, 0);
    glutMainLoop();
    return 0;
}