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
#define TITLE "IDI: Lights and Models"
#endif

// Define models
Model model;

struct Object {
  std::string filename;
  GLfloat scale_factor;
  bool vertex_normal;
  GLfloat translateX, translateY, translateZ;
  GLfloat rotateX, rotateY, rotateZ;
} obj;

// View
bool axiometricView = false;            // Enable axiometric view
bool perspectiveView = !axiometricView; // Enable prespective view

// Camera
bool eulerCamera = true;                // Enable euler camera
bool lookCamera = !eulerCamera;         // Enale gluLookAt camera

float EyeDist = 7.0f;                   // Camera distance
float theta = 10.0f;
float psi = 15.0f;
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
bool RightClicked = false;              // Right button clicked
Point mouse(0.0f, 0.0f, 0.0f);          // Mouse position

// For lights
int lightPosition = 0;
float posLight1[][4] = {
    { 1.0f, 0.0f, -1.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f, 1.0f },
    { -1.0f, 0.0f, 1.0f, 1.0f },
    { -1.0f, 0.0f, -1.0f, 1.0f }
};
bool showCameraLight = true;
bool showSceneLight = false;

// Other
bool VERBOSE = true;                    // Enable verbose
bool Axes = false;                       // Enable axes

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
    // Init models
    obj.filename = "models/cow.obj";
    obj.scale_factor = 2.0f;
    obj.vertex_normal = true;
    obj.translateX = 0.0f; 
    obj.translateY = 0.64f; 
    obj.translateZ = 0.0f;
    obj.rotateX = -90.0f; 
    obj.rotateY = -75.0f; 
    obj.rotateZ = 0.0f;
    


    // Init Glut.
    glutInit(&argc, (char **) argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Load model
    model.load(obj.filename);

    // Create window with WIDTH width and HEIGHT height.
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow(TITLE);

    glutDisplayFunc(Display);                   // Set draw function.
    glutMouseFunc(MouseButton);                 // Mouse events.
    glutMotionFunc(MouseMotion);                // Drag mouse events.
    glutKeyboardFunc(KeyboardPress);            // Keyboard events.
    glutSpecialFunc(NonASCIIKeyboardPress);     // Keyboard special events.
    glutReshapeFunc(ResizeWindow);              // Set resize function.
    glutIdleFunc(Display);                      // Set IDLE function.

    // Set Flags
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_COLOR_MATERIAL);

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
    cout << TERM__Purple << "  f:\t\t\t"   << TERM__Off << "Toggle scene floor." << endl;
    cout << TERM__Purple << "  s:\t\t\t"   << TERM__Off << "Change scene floor." << endl;
    cout << TERM__Purple << "  c:\t\t\t"   << TERM__Off << "Toggle camera floor." << endl;
    cout << TERM__Purple << "  a:\t\t\t"   << TERM__Off << "Toggle view axes." << endl;
    cout << TERM__Purple << "  r/R:\t\t\t"   << TERM__Off << "Rotate up vector positive and negative respectively." << endl;
    cout << TERM__Purple << "  Arrow up/down:\t" << TERM__Off << "Change camera position arround X axis." << endl;
    cout << TERM__Purple << "  Arrow left/right:\t" << TERM__Off << "Change camera position arround Y axis." << endl;
    cout << TERM__Purple << "  .:\t" << TERM__Off << "Reset camera position." << endl;
}

void SceneLight() {
    lightPosition = lightPosition % 4;

    glLightfv(GL_LIGHT1,GL_AMBIENT, white);
    glLightfv(GL_LIGHT1,GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1,GL_SPECULAR, white);
    glLightfv(GL_LIGHT1,GL_POSITION, posLight1[lightPosition]);
}

void DrawFloor()
{
    GLfloat color[] = {blue[0], blue[1], blue[2]};
    GLfloat whitef[] = {white[0], white[1], white[2]};
    GLfloat shininess = 120.0f;
    glColor3f(blue[0], blue[1], blue[2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whitef);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);


    glBegin(GL_QUADS);
        glNormal3f(-50, 0, 50);
        glVertex3f(-50, 0, 50);

        glNormal3f(-50, 0, -50);
        glVertex3f(-50, 0, -50);

        glNormal3f(50, 0, -50);
        glVertex3f(50, 0, -50);

        glNormal3f(50, 0, 50);
        glVertex3f(50, 0, 50);
    glEnd();
}

void DrawAxes (void)
{
    GLfloat shininess = 0.0f;
    GLfloat redf[] = {red[0], red[1], red[2]};
    GLfloat bluef[] = {blue[0], blue[1], blue[2]};
    GLfloat greenf[] = {green[0], green[1], green[2]};

    if (Axes) {
        glBegin (GL_LINES);
            // X axis in red.
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redf);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redf);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redf);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
            glColor3f(red[0], red[1], red[2]);
            
            glVertex3f(-50, 0, 0);
            glVertex3f(50, 0, 0);

            // Y axis in blue
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bluef);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bluef);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bluef);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
            glColor3f(blue[0], blue[1], blue[2]);

            glVertex3f(0, -50, 0);
            glVertex3f(0, 50, 0);

            // Z axis in green
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, greenf);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greenf);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, greenf);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
            glColor3f(green[0], green[1], green[2]);

            glVertex3f(0, 0, -50);
            glVertex3f(0, 0, 50);
        glEnd();
    }
}

void Draw()
{
    glPushMatrix();
        glTranslatef(obj.translateX, obj.translateY, obj.translateZ);
        // glRotated(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
        glRotated(obj.rotateZ, 0.0f, 0.0f, 1.0f);
        glRotated(obj.rotateY, 0.0f, 1.0f, 0.0f);
        glRotated(obj.rotateX, 1.0f, 0.0f, 0.0f);
        glScalef(obj.scale_factor,
                 obj.scale_factor,
                 obj.scale_factor);
        model.vertex_normal = obj.vertex_normal;
        model.draw();
    glPopMatrix();
}

void ChangeCamera()
{
    Trans.x = cos(theta * TO_RADIANS) * sin(psi * TO_RADIANS);
    Trans.y = sin(theta * TO_RADIANS);
    Trans.z = cos(theta * TO_RADIANS) * cos(psi * TO_RADIANS);

    OBS = VRP + EyeDist * Trans;

    if(lookCamera) {
        glLoadIdentity();
        gluLookAt(OBS.x, OBS.y, OBS.z,
                  VRP.x, VRP.y, VRP.z,
                  UPv.x, UPv.y, UPv.z);
        return;
    }
    if(eulerCamera) {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -EyeDist);
        glRotatef(-phi, 0.0f, 0.0f, 1.0f);
        glRotatef(theta, 1.0f, 0.0f, 0.0f);
        glRotatef(-psi, 0.0f, 1.0f, 0.0f);
        glTranslatef(-VRP.x, -VRP.y, -VRP.z);
    }
}

void RotateCamera() {

    Trans.x = sin(theta * TO_RADIANS) * cos(psi * TO_RADIANS);
    Trans.y = sin(theta * TO_RADIANS);
    Trans.z = cos(theta * TO_RADIANS) * cos(psi * TO_RADIANS);

    VRP = OBS + Trans;
    ChangeCamera();
}

void ToggleCameraLight() {
    if (VERBOSE) Log("Toggle camera light");
    if (!showCameraLight) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }
    showCameraLight = !showCameraLight;
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

    // Extras
    if (Axes) DrawAxes();
    DrawFloor();

    // Lights
    if (showCameraLight) glEnable(GL_LIGHT0);
    if (showSceneLight) SceneLight();

    glutSwapBuffers();
}

void ResizeWindow(GLsizei w, GLsizei h)
{
    float fov, zNear, zFar, left, right, bottom, top;

    // Set variables
    if(w <= h)
    {
        if (axiometricView) {
            left = -1.0f;
            right = 1.0f;
            bottom = -1.0f * w/h;
            top = 1.0f * w/h;
            zNear = -1.0f;
            zFar = 1.0f;
        }
        if (perspectiveView) {
            fov = 2 * (TO_DEGREES * (atan(0.5/2))/1);
            zNear = 1.0f;
            zFar = 1000.0f;
        }
    }
    else
    {
        if (axiometricView) {
            left = -1.0f * w/h;
            right = 1.0f * w/h;
            bottom = -1.0f;
            top = 1.0f;
            zNear = -1.0f;
            zFar = 1.0f;
        }
        if (perspectiveView) {
            fov = 2 * (TO_DEGREES * (atan(0.5/2))/1);
            zNear = 1.0f;
            zFar = 1000.0f;
        }
    }

    // Set Viewport
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Select between Ortho and Perspective
    if (axiometricView) {
        glOrtho(left, right, bottom, top, zNear, zFar);
    }
    if (perspectiveView) {
        gluPerspective(fov, w/h, zNear, zFar);
    }

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

        // Camera
        case '.':               // Reset camera to original position
            if (VERBOSE) Log("Reset camera position.");
            EyeDist = 7.0f;                   // Camera distance
            theta = 10.0f;
            psi = 15.0f;
            phi = 0.0f;
            VRP.x = 0;
            VRP.y = 0;
            VRP.z = 0;
            ChangeCamera();
            glutPostRedisplay();
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
        case 'r':               // Rotate UP vector
            if (VERBOSE) Log("Rotate up vector.");
            phi += 5;
            ChangeCamera();
            glutPostRedisplay();
            break;
        case 'R':               // Rotate UP vector
            if (VERBOSE) Log("Rotate up vector.");
            phi -= 5;
            ChangeCamera();
            glutPostRedisplay();
            break;

        // Lights
        case 's':               // Change scene light position
            if (VERBOSE) Log("Change scene light position.");
            lightPosition += 1;
            glutPostRedisplay();
            break;
        case 'f':               // Toggle scene light
            ToggleSceneLight();
            glutPostRedisplay();
            break;
        case 'c':       // Toggle camera light
            ToggleCameraLight();
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
