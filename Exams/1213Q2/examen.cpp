// System includes
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>

// Our includes
#include "opengl.h"
#include "point.h"
#include "colors.h"
#include "helpers.h"
#include "model.h"

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
#define TITLE "IDI: Prova de Laboratori"
#endif

// Define models
Model model;

// Camera
float EyeDist = 10.0f;                   // Camera distance
float theta = 0.0f;
float psi = 0.0f;
float phi = 0.0f;

Point VRP(0.0f, 0.0f, 0.0f);            // Visual point reference
// Temporal Point for transformations
Point Trans(cos(theta * TO_RADIANS) * sin(psi * TO_RADIANS),
            sin(theta * TO_RADIANS),
            cos(theta * TO_RADIANS) * cos(psi * TO_RADIANS));

Point OBS(VRP + EyeDist * Trans);       // Observer position
Point UPv(0.0f, 1.0f, 0.0f);            // UP vector

// Mouse controls
bool LeftClicked = false;               // Left button clicked
Point mouse(0.0f, 0.0f, 0.0f);          // Mouse position

// For lights
int lightPosition = 0;
float posLight1[][4] = {
    { 5.0f, 9.0f, 5.0f, 1.0f },
    { 5.0f, 9.0f, -5.0f, 1.0f },
    { -5.0f, 9.0f, -5.0f, 1.0f },
    { -5.0f, 9.0f, 5.0f, 1.0f },
    { 0.0f, 9.0f, 0.0f, 1.0f }
};
bool showSceneLight = true;

// Other
bool VERBOSE = true;                    // Enable verbose
bool Axes = false;                      // Enable axes

float Xtranslation = 0.0f;

/***** Function prototypes *****/

void KeyboardPress(unsigned char key, int x, int y);
void NonASCIIKeyboardPress(int key, int x, int y);
void MouseMotion(int x, int y);
void MouseButton(int button, int state, int x, int y);

void Display();
void ResizeWindow(GLsizei w, GLsizei h);
void ChangeCamera();
void RotateCamera();
void SceneLight();

void DrawFloor();
void DrawAxes();
void Draw();

/***** Function implementations *****/

int main(int argc, const char *argv[])
{
    // Init Glut.
    glutInit(&argc, (char **) argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Create window with WIDTH width and HEIGHT height.
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow(TITLE);

    // Load model
    model.load("legoman.obj");

    glutDisplayFunc(Display);                   // Set draw function.
    glutMouseFunc(MouseButton);                 // Mouse events.
    glutMotionFunc(MouseMotion);                // Drag mouse events.
    glutKeyboardFunc(KeyboardPress);            // Keyboard events.
    glutSpecialFunc(NonASCIIKeyboardPress);     // Keyboard special events.
    glutReshapeFunc(ResizeWindow);              // Set resize function.

    // Set Flags
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHT1);
    SceneLight();

    ChangeCamera();

    // Main loop
    glutMainLoop();
    return 0;
}

void Help()
{
    // Show help in command line
    cout << TERM__Yellow << TITLE << TERM__Off << endl;
    cout << TERM__Purple << "  Esc:\t\t\t" << TERM__Off << "Exit." << endl;
    cout << TERM__Purple << "  h:\t\t\t" << TERM__Off << "Show this help." << endl;
    cout << TERM__Purple << "  v:\t\t\t"   << TERM__Off << "Toggle verbose mode." << endl;
    cout << TERM__Purple << "  f:\t\t\t"   << TERM__Off << "Toggle scene light." << endl;
    cout << TERM__Purple << "  s:\t\t\t"   << TERM__Off << "Change scene light." << endl;
    cout << TERM__Purple << "  a:\t\t\t"   << TERM__Off << "Toggle view axes." << endl;
    cout << TERM__Purple << "  Arrows/Mous:\t" << TERM__Off << "Change camera position arround X axis." << endl;
    cout << TERM__Purple << "  z/Z:\t" << TERM__Off << "Zoom out/Zoom in." << endl;
}

void SceneLight() {
    lightPosition = lightPosition % 5;

    glLightfv(GL_LIGHT1,GL_AMBIENT, white);
    glLightfv(GL_LIGHT1,GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1,GL_SPECULAR, white);
    glLightfv(GL_LIGHT1,GL_POSITION, posLight1[lightPosition]);
}

void DrawFloor()
{
    GLfloat shininess = 128.0f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blueDark);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);


    glBegin(GL_QUADS);
        glNormal3f(-5, 0, 5);
        glVertex3f(-5, 0, 5);

        glNormal3f(-5, 0, -5);
        glVertex3f(-5, 0, -5);

        glNormal3f(5, 0, -5);
        glVertex3f(5, 0, -5);

        glNormal3f(5, 0, 5);
        glVertex3f(5, 0, 5);
    glEnd();
}

void DrawAxes (void)
{
    GLfloat shininess = 0.0f;

    if (Axes) {
        glBegin (GL_LINES);
            // X axis in red.
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
            
            glVertex3f(-100, 0, 0);
            glVertex3f(100, 0, 0);

            // Y axis in blue
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

            glVertex3f(0, -100, 0);
            glVertex3f(0, 100, 0);

            // Z axis in green
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, green);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

            glVertex3f(0, 0, -100);
            glVertex3f(0, 0, 100);
        glEnd();
    }
}

void Draw()
{
    glPushMatrix();
        glTranslatef(Xtranslation, 2.0f, 0);
        GLfloat shininess = 128.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grayDark);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gray);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
        glutSolidCube(4.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(Xtranslation, 6.0f, 0);
        model.draw(4);
    glPopMatrix();
}

void ChangeCamera()
{
    Trans.x = cos(theta * TO_RADIANS) * sin(psi * TO_RADIANS);
    Trans.y = sin(theta * TO_RADIANS);
    Trans.z = cos(theta * TO_RADIANS) * cos(psi * TO_RADIANS);

    OBS = VRP + EyeDist * Trans;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -EyeDist);
    glRotatef(theta, 1.0f, 0.0f, 0.0f);
    glRotatef(-psi, 0.0f, 1.0f, 0.0f);
    glTranslatef(-VRP.x, -VRP.y, -VRP.z);
}

void RotateCamera() {

    Trans.x = sin(theta * TO_RADIANS) * cos(psi * TO_RADIANS);
    Trans.y = sin(theta * TO_RADIANS);
    Trans.z = cos(theta * TO_RADIANS) * cos(psi * TO_RADIANS);

    VRP = OBS + Trans;
    ChangeCamera();
}

void ToggleSceneLight() {
    if (VERBOSE) Log("Toggle scene light");
    if (!showSceneLight) {
        glEnable(GL_LIGHT1);
    } else {
        glDisable(GL_LIGHT1);
    }
    showSceneLight = !showSceneLight;
}

void Display()
{

    // Set bolor background
    glClearColor(black[R], black[G], black[B], black[A]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Draw();
    DrawFloor();

    // Extras
    if (Axes) DrawAxes();

    glutSwapBuffers();
}

void ResizeWindow(GLsizei w, GLsizei h)
{
    // Set Viewport
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

        gluPerspective (90, (GLfloat)w / (GLfloat)h, 1.0, 1000.0); 

    glMatrixMode(GL_MODELVIEW);

    ChangeCamera();
}

void MouseButton(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            switch (state) {
                case GLUT_DOWN:
                    LeftClicked = true;
                    break;
                case GLUT_UP:
                    LeftClicked = false;
                    break;
                default:
                    mouse.x = x;
                    mouse.y = y;
                    break;
            }
        default:
            break;
    }

}

void MouseMotion(int x, int y)
{
    if (LeftClicked) {
        if (mouse.x < x) {
            --psi;
            ChangeCamera();
            glutPostRedisplay();
            mouse.x = x;
            mouse.y = y;
        }
        if (mouse.x > x) {
            ++psi;
            ChangeCamera();
            glutPostRedisplay();
            mouse.x = x;
            mouse.y = y;
        }

        if (mouse.y < y) {
            ++theta;
            ChangeCamera();
            glutPostRedisplay();
            mouse.x = x;
            mouse.y = y;
        }
        if (mouse.y > y) {
            --theta;
            ChangeCamera();
            glutPostRedisplay();
            mouse.x = x;
            mouse.y = y;
        }

    }
}

void KeyboardPress(unsigned char key, int x, int y)
{
    switch (key) {
        // Controls
        case 27:        // ESCAPE key.
            exit(0);
            break;
        case 'q':        // ESCAPE key.
            exit(0);
            break;
        case 'h':       // Show help.
            Help();
            break;
        case 'H':       // Show help.
            Help();
            break;
        case '?':       // Show help.
            Help();
            break;
        case 'V':       // Toggle Verbose mode.
            VERBOSE = !VERBOSE;
            break;
        case 'v':       // Toggle Verbose mode.
            VERBOSE = !VERBOSE;
            break;

        // Displacement
        case 'd':
            if (VERBOSE) Log("Move objects X+1.");
            Xtranslation += 1;
            glutPostRedisplay();
            break;
        case 'e':
            if (VERBOSE) Log("Move objects X-1.");
            Xtranslation -= 1;
            glutPostRedisplay();
            break;

        // Camera
        case 'z':               // Zoom out
            if (VERBOSE) Log("Zoom out.");
            EyeDist += 5;
            if(EyeDist < 0) EyeDist = 0;
            ChangeCamera();
            glutPostRedisplay();
            break;
        case 'Z':               // Zoom in
            if (VERBOSE) Log("Zoom in.");
            EyeDist -= 5;
            if(EyeDist > 50) EyeDist = 50;
            ChangeCamera();
            glutPostRedisplay();
            break;

        // Lights
        case 's':               // Change scene light position
            if (VERBOSE) Log("Change scene light position.");
            lightPosition += 1;
            SceneLight();
            glutPostRedisplay();
            break;
        case 'f':               // Toggle scene light
            ToggleSceneLight();
            glutPostRedisplay();
            break;

        // Extras
        case 'a':               // Toggle axes
            if (!Axes) {
                if (VERBOSE) Log("Enable axes.");
            } else {
                if (VERBOSE) Log("Disable axes.");
            }
            Axes = !Axes;
            glutPostRedisplay();
            break;
        
        default:
            // Do nothing
            glutPostRedisplay();
            break;
    }
}

void NonASCIIKeyboardPress(int key, int x, int y)
{
    switch(key) {
        // Camera position
        case GLUT_KEY_DOWN:
            if (VERBOSE) Log("Rotate camera.");
            ++theta;
            ChangeCamera();
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if (VERBOSE) Log("Rotate camera.");
            --theta;
            ChangeCamera();
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            if (VERBOSE) Log("Rotate camera.");
            ++psi;
            ChangeCamera();
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            if (VERBOSE) Log("Rotate camera.");
            --psi;
            ChangeCamera();
            glutPostRedisplay();
            break;
        // End camera position
        default:
            break;
    };
}
