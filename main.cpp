
#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include <iostream>
#include <cmath>
#include "color.h"
using namespace std;

// Window Size
#ifndef WIDTH
#define WIDTH  600
#endif

#ifndef HEIGHT
#define HEIGHT 600
#endif

// Window Title
#ifndef TITLE
#define TITLE "IDI: Practiques OpenGL"
#endif

// Structs
struct Point {
    float x, y, z;
};

struct Triangle {
    Point bl, br, top;
};

struct Rotation {
    double angle, x, y, z;
};

// Set application colors
Color Background (0.0f, 0.0f, 1.0f, 0.0f);
Color Front (1.0f, 0.0f, 0.0f, 0.0f);

// Define points
Point mouse = {0.0f, 0.0f, 0.0f};
Triangle t = {
    {-0.5f, -1.0f/3.0f, 0.0f},
    {0.5f, -1.0f/3.0f, 0.0f},
    {0.0f, 2.0f/3.0f, 0.0f}
};

// Define rotations
Rotation xAxis = {0.0, 1.0, 0.0, 0.0};
Rotation yAxis = {0.0, 0.0, 1.0, 0.0};
Rotation zAxis = {0.0, 0.0, 0.0, 1.0};

// Global functions
bool changeColor = false;
bool changeTriangle = false;
bool changeRotation = false;
unsigned int  triangleDefined = 0;


void help()
{
    // Show help in command line
    cout << TITLE << endl;
    cout << "\tPress \"Esc\" for exit." << endl;
    cout << "\tf:\tClick and move mouse for change background." << endl;
    cout << "\tt:\tClick three new vertex for triangle." << endl;
    cout << "\tr:\tEnable rotation mode with keys: a, d, w, s, q, e." << endl;
}

void refresh()
{
    glLoadIdentity();

    // Set bolor background
    glClearColor(Background[Color::R], Background[Color::G],
                 Background[Color::B], Background[Color::A]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Make rotations
    glRotated(xAxis.angle, xAxis.x, xAxis.y, xAxis.z);
    glRotated(yAxis.angle, yAxis.x, yAxis.y, yAxis.z);
    glRotated(zAxis.angle, zAxis.x, zAxis.y, zAxis.z);

    // Draw triangle
    glBegin(GL_TRIANGLES);
        // Set front color.
        glColor3f(Front[Color::R], Front[Color::G], Front[Color::B]);
        glVertex3f(t.bl.x, t.bl.y, t.bl.z);         // Bottom left vertex.
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(t.br.x, t.br.y, t.br.z);         // Bottom right vertex.
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(t.top.x, t.top.y, t.top.z);      // Top vertex.
    glEnd();

    glutSwapBuffers();
}

void resize(GLsizei w, GLsizei h)
{
    // Calculate new coordinates for bottom left point in original window.
    int nw = (WIDTH - w)/2;  
    int nh = (HEIGHT - h)/2;

    // Set new coordinates to viewport and keep original size.
    glViewport(-nw, -nh, WIDTH, HEIGHT);
    // if (w > h)
    //     glViewport(w/2 - h/2, 0, h, h);
    // else
    //     glViewport(0, h/2 - w/2, w, w);
}

void pixelToCoordinates(int x, int y, struct Point *p)
{
    // Declare local variables
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ = 0;
    GLdouble posX, posY, posZ;
 
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   // Get model view matrix
    glGetDoublev(GL_PROJECTION_MATRIX, projection); // Get projection matix
    glGetIntegerv(GL_VIEWPORT, viewport);           // Get viewport
 
    winX = (float) x;
    winY = (float) viewport[3] - (float) y;
 
    // Get coordinates from pixels in posX, posY, posZ
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX,
                 &posY, &posZ);
 
    p->x = posX; p->y = posY; p->z = posZ;  // Set new position
}

void MouseButton(int button, int state, int x, int y)
{
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
        mouse.x = x; mouse.y = y;   // Set mouse position
        if (changeTriangle) {
            switch (triangleDefined)
            {
                case 0:     // Set bottom left vertex for triangle
                    pixelToCoordinates(x, y, &t.bl);
                    triangleDefined += 1;
                    break;
                case 1:     // Set bottom right vertex for triangle
                    pixelToCoordinates(x, y, &t.br);
                    triangleDefined += 1;
                    break;
                case 2:     // Set top vertex for triangle
                    pixelToCoordinates(x, y, &t.top);
                    triangleDefined = 0;
                    changeTriangle = false;     // Disable change triangle mode
                    glutPostRedisplay();        // Refresh display
                    break;
            }
        }
    }

    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
        changeColor = false;    // Disable change color mode when click up
}

void MouseMotion(int x, int y)
{
    if (changeColor) {

        float diff = mouse.x - x;   // Get differences from mouse positions.
        float change = 0.0f;

        if (diff > 0) {
            change = 0.1f;          // More blue when move to left
        } else {
            change = -0.1f;         // More blue when move to right
        }

        if (fabs(diff) > WIDTH/100) {   // Only change when diff is 1% of pixels
            Background.setBlue(Background.blue() + change);
            mouse.x = x; mouse.y = y;   // Set mouse position
            glutPostRedisplay();        // Refresh display
        }
    }
}

void Keyboard(unsigned char key, int x, int y)
{
    mouse.x = x; mouse.y = y;
    switch (key) {
        case 27:        // ESCAPE key.
            exit(0);
            break;
        case 'f':       // Enable change background color mode.
            changeColor = !changeColor;
            break;
        case 't':       // Enable change triangle position.
            changeTriangle = !changeTriangle;
            break;
        case 'h':       // Show help.
            help();
            break;
        case 'r':       // Enable change rotation mode.
            changeRotation = !changeRotation;
            break;
        // Rotation commands
        case 'a':                   // Rotate Y axis negative
            if (changeRotation) {
                yAxis.angle -= 10.0;
                if (yAxis.angle < 0) yAxis.angle = 0;
                glutPostRedisplay();
            }
            break;
        case 'd':                   // Rotate Y axis positive
            if (changeRotation) {
                yAxis.angle += 10.0;
                if (yAxis.angle < 360) yAxis.angle = 360;
                glutPostRedisplay();
            }
            break;
        case 'w':                   // Rotate X axis negative
            if (changeRotation) {
                xAxis.angle -= 10.0;
                if (xAxis.angle < 0) xAxis.angle = 0;
                glutPostRedisplay();
            }
            break;
        case 's':                   // Rotate X axis positive
            if (changeRotation) {
                xAxis.angle += 10.0;
                if (xAxis.angle < 360) xAxis.angle = 360;
                glutPostRedisplay();
            }
            break;
        case 'q':                   // Rotate Z axis negative
            if (changeRotation) {
                zAxis.angle -= 10.0;
                if (zAxis.angle < 0) zAxis.angle = 0;
                glutPostRedisplay();
            }
            break;
        case 'e':                   // Rotate X axis positive
            if (changeRotation) {
                zAxis.angle += 10.0;
                if (zAxis.angle < 360) zAxis.angle = 360;
                glutPostRedisplay();
            }
            break;
    }
}

int main(int argc, const char *argv[])
{
    // Init Glut.
    glutInit(&argc, (char **) argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Create window with WIDTH width and HEIGHT height.
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow(TITLE);

    glutDisplayFunc(refresh);       // Set draw function.
    glutMouseFunc(MouseButton);     // Mouse events.
    glutMotionFunc(MouseMotion);    // Drag mouse events.
    glutKeyboardFunc(Keyboard);     // Keyboard events.
    glutReshapeFunc(resize);        // Set resize function.

    // Main loop
    glutMainLoop();
    return 0;
}
