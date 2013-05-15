
#include "opengl.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include "planet.h"
#include "point.h"
#include "color.h"
#include "model.h"
#include "terminalcolors.h"
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
#define TITLE "IDI: Solar System"
#endif

// Colors
Color blue (0.0f, 0.0f, 1.0f, 1.0f);
Color green (0.0f, 1.0f, 0.0f, 1.0f);
Color cyan (0.0f, 1.0f, 1.0f, 1.0f);
Color yellow (1.0f, 1.0f, 0.0f, 1.0f);
Color orange (1.0f, 0.5f, 0.0f, 1.0f);
Color red (1.0f, 0.0f, 0.0f, 1.0f);
Color magenta (1.0f, 0.0f, 1.0f, 1.0f);
Color white (1.0f, 1.0f, 1.0f, 1.0f);
Color grey (0.25f, 0.25f, 0.25f);
Color black (0.0f, 0.0f, 0.0f, 0.0f);

Color Background (black);               // Define background color

const GLfloat TO_RADIANS = M_PI/180.0;
const GLfloat TO_DEGREES = 180.0/M_PI;

// Planets
/* Planet name (inclination, orbitDuration, orbitRadius, rotationDuration, radius, color)*/
Planet sun (7.25, 0, 0, 1200, 0.22, yellow);
Planet mercury (7, 87.9, 0.32, 58.65, 0.02, orange);
Planet venus (3, 583.92, 0.45, -243.01, 0.05, magenta);
Planet earth (7, 365.25, 0.65, 24, 0.10, blue);
Planet mars (2, 686.97, 0.9, 24.6, 0.09, red);
Planet jupiter (6.09, 4332.59, 1.7, 320.3, 0.15, orange);
Planet saturn (15.09, 10759.22, 2.1, 378, 0.12, grey);
Planet uranus (9.29, 30799.25, 2.5, 369, 0.10, cyan);
Planet neptune (-2, 60799.25, 3, 367.49, 0.11, blue);

// Rotation
bool makeRotation = false;              // Enable solar system rotation
int rotationAngle = 30;                 // Rotation angle for solar system
Point rotationAxis(1, 0, 0);            // Rotation axis
Point n;

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

float cameraIncrement = 0.1f;           // Camera increment for changes.

bool fly = true;                       // Enable Fly-through mode.
bool walk = !fly;                      // Enable Walk mode.
float pan = 0.0f;

// Mouse controls
bool LeftClicked = false;               // Left button clicked
bool RightClicked = false;              // Right button clicked
Point mouse(0.0f, 0.0f, 0.0f);          // Mouse position

// Other
// float scalation = 1.2f;              // Scale proportion
bool VERBOSE = true;                    // Enable verbose
bool Floor = true;                      // Enable floor
bool Axes = true;                       // Enable axes
        GLdouble m[16];

/***** Function prototypes *****/

void KeyboardPress(unsigned char key, int x, int y);
void NonASCIIKeyboardPress(int key, int x, int y);
void MouseMotion(int x, int y);
void MouseButton(int button, int state, int x, int y);

void Display();
void ResizeWindow(GLsizei w, GLsizei h);
void ChangeCamera();

void Help();
void Print(const std::string c, const std::string s);
void Log(const std::string s);
void Error(const std::string s);
void Warning(const std::string s);
void PixelToCoordinates(int x, int y, Point *p);

void DrawFloor();
void DrawAxes();
void DrawPlanets();



/***** Function implementations *****/

int main(int argc, const char *argv[])
{
    // Init Glut.
    glutInit(&argc, (char **) argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Add extras to planets
    earth.addMoon();
    mars.addAstronaut("models/HomerProves.obj");

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
    glEnable(GL_DEPTH_TEST);
    ChangeCamera();

    // Main loop
    glutMainLoop();
    return 0;
}

void Help()
{
    // Show help in command line
    cout << TERM__Yellow << "IDI Solar System" << TERM__Off << endl;
    cout << TERM__Purple << "  Esc:\t\t\t" << TERM__Off << "Exit." << endl;
    cout << TERM__Purple << "  h,?:\t\t\t" << TERM__Off << "Show this help." << endl;
    cout << TERM__Purple << "  V:\t\t\t"   << TERM__Off << "Toggle verbose mode." << endl;
    cout << TERM__Purple << "  p:\t\t\t"   << TERM__Off << "Switch planets between solid and wire." << endl;
    cout << TERM__Purple << "  f:\t\t\t"   << TERM__Off << "Toggle view floor." << endl;
    cout << TERM__Purple << "  F:\t\t\t"   << TERM__Off << "Toggle view axes." << endl;
    cout << TERM__Purple << "  +:\t\t\t"   << TERM__Off << "Increment velocity." << endl;
    cout << TERM__Purple << "  -:\t\t\t"   << TERM__Off << "Decrement velocity." << endl;
    cout << TERM__Purple << "  w/s:\t\t\t" << TERM__Off << "Walk forward/backward." << endl;
    cout << TERM__Purple << "  a/d:\t\t\t" << TERM__Off << "Walk left/right." << endl;
    cout << TERM__Purple << "  v:\t\t\t"   << TERM__Off << "Switch between axiometric and perspective view." << endl;
    cout << TERM__Purple << "  c:\t\t\t"   << TERM__Off << "Switch between gluLookAt and Euler camera." << endl;
    cout << TERM__Purple << "  r/R:\t\t\t"   << TERM__Off << "Rotate up vector positive and negative respectively." << endl;
    cout << TERM__Purple << "  Arrow up/down:\t" << TERM__Off << "Change camera position arround X axis." << endl;
    cout << TERM__Purple << "  Arrow left/right:\t" << TERM__Off << "Change camera position arround Y axis." << endl;
}

void Print(const std::string c, const std::string s)
{
    time_t rawtime;
    struct tm * ptm;
    time(&rawtime);
    ptm = gmtime(&rawtime);

    cout << c;  // Set color
    // Print HOUR
    if (ptm->tm_hour < 10){
        cout << 0 << (ptm->tm_hour)%24 << ":";
    } else {
        cout << (ptm->tm_hour)%24 << ":";
    }
    // Print MINUTES
    if (ptm->tm_min < 10){
        cout << 0 << (ptm->tm_min)%60 << ":";
    } else {
        cout << (ptm->tm_min)%60 << ":";
    }
    // Print SECONDS
    if (ptm->tm_sec < 10){
        cout << 0 << (ptm->tm_sec)%60;
    } else {
        cout << (ptm->tm_sec)%60;
    }
    // Print String
    cout << "  " << s << TERM__Off << endl;
}

void Log(const std::string s)
{
    Print(TERM__Blue, s);
}

void Error(const std::string s)
{
    Print(TERM__Red, s);
}

void Warning(const std::string s)
{
    Print(TERM__Yellow, s);
}

void DrawFloor()
{
    if (Floor) {
        glBegin(GL_LINES);
            for(int i=-50; i<=50; i++) {

                // Vertical lines
                glColor3f(grey[0], grey[1], grey[2]);
                glVertex3f(i, 0, -50);
                glVertex3f(i, 0, 50);

                // Horizontal lines
                glColor3f(grey[0], grey[1], grey[2]);
                glVertex3f(-50, 0, i);
                glVertex3f(50, 0, i);
            };
        glEnd();
    }
}

void DrawAxes (void)
{
    if (Axes) {
        glBegin (GL_LINES);
            // X axis in red.
            glColor3f(red[0], red[1], red[2]);
            glVertex3f(-50, 0, 0);
            glVertex3f(50, 0, 0);

            // Y axis in blue
            glColor3f(blue[0], blue[1], blue[2]);
            glVertex3f(0, -50, 0);
            glVertex3f(0, 50, 0);

            // Z axis in green
            glColor3f(green[0], green[1], green[2]);
            glVertex3f(0, 0, -50);
            glVertex3f(0, 0, 50);
        glEnd();
    }
}

void DrawPlanets()
{
    sun.draw();
    mercury.draw();
    venus.draw();
    earth.draw();
    mars.draw();
    // jupiter.draw();
    // saturn.draw();
    // uranus.draw();
    // neptune.draw();

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

void Display()
{

    // Set bolor background
    glClearColor(Background[Color::R], Background[Color::G],
                 Background[Color::B], Background[Color::A]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawPlanets();
    DrawAxes();
    DrawFloor();

    if (makeRotation) {
        glGetDoublev(GL_MODELVIEW_MATRIX, m);
        glLoadIdentity();
        glRotated(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
        glMultMatrixd(m);
        makeRotation = false;
    }

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

void pixelToCoordinates(int x, int y, Point *p)
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
        case 27:        // ESCAPE key.
            exit(0);
            break;
        case 'h':       // Show help.
            Help();
            break;
        case '?':       // Show help.
            Help();
            break;
        case 'V':       // Toggle Verbose mode.
            VERBOSE = !VERBOSE;
            break;
        case '+':       // Increment velocity of orbit period
            if (VERBOSE) Log("Increment velocity of orbit period.");
            sun.setIncrement(sun.getIncrement() + 1);
            mercury.setIncrement(mercury.getIncrement() + 1);
            venus.setIncrement(venus.getIncrement() + 1);
            earth.setIncrement(earth.getIncrement() + 1);
            mars.setIncrement(mars.getIncrement() + 1);
            // jupiter.setIncrement(jupiter.getIncrement() + 1);
            // saturn.setIncrement(saturn.getIncrement() + 1);
            // uranus.setIncrement(uranus.getIncrement() + 1);
            // neptune.setIncrement(neptune.getIncrement() + 1);
            glutPostRedisplay();
            break;
        case '-':        // Decrement velocity of orbit period
            if (VERBOSE) Log("Decrement velocity of orbit period.");
            sun.setIncrement(sun.getIncrement() - 1);
            mercury.setIncrement(mercury.getIncrement() - 1);
            venus.setIncrement(venus.getIncrement() - 1);
            earth.setIncrement(earth.getIncrement() - 1);
            mars.setIncrement(mars.getIncrement() - 1);
            // jupiter.setIncrement(jupiter.getIncrement() - 1);
            // saturn.setIncrement(saturn.getIncrement() - 1);
            // uranus.setIncrement(uranus.getIncrement() - 1);
            // neptune.setIncrement(neptune.getIncrement() - 1);
            glutPostRedisplay();
            break;
        case 'p':       // Switch planet render between solid and wire.
            if (VERBOSE) {
                if (!sun.getSolid()) {
                    Log("Enable solid planets.");
                } else {
                    Log("Disable solid planets.");
                }
            }
            sun.makeSolid(!sun.getSolid());
            mercury.makeSolid(!mercury.getSolid());
            venus.makeSolid(!venus.getSolid());
            earth.makeSolid(!earth.getSolid());
            mars.makeSolid(!mars.getSolid());
            // jupiter.makeSolid(!jupiter.getSolid());
            // saturn.makeSolid(!saturn.getSolid());
            // uranus.makeSolid(!uranus.getSolid());
            // neptune.makeSolid(!neptune.getSolid());
            glutPostRedisplay();
            break;
        case 'v':       // Switch between axiometric and perspective View
            if (VERBOSE) {
                if (!axiometricView) {
                    Log("Enable axiometric view.");
                } else {
                    Log("Enable perspective view.");
                }
            }
            axiometricView = !axiometricView;
            perspectiveView = !axiometricView;
            glutPostRedisplay();
            break;
        case 's':               // Walk forward
            VRP -= (VRP - OBS) * 0.02;
            ChangeCamera();
            glutPostRedisplay();
            break;
        case 'w':               // Walk backward
            VRP += (VRP - OBS) * 0.02;
            ChangeCamera();
            glutPostRedisplay();
            break;
        case 'a':               // Walk left
            VRP -= (VRP - OBS) * 0.02;
            ChangeCamera();
            glutPostRedisplay();
            break;
        case 'd':               // Walk right
            VRP += (VRP - OBS) * 0.2;
            ChangeCamera();
            glutPostRedisplay();
            break;
        // End rotate solar system
        case 'z':               // Zoom out
            if (VERBOSE) Log("Zoom out.");
            EyeDist += 1;
            if(EyeDist < 0) EyeDist = 0;
            ChangeCamera();
            glutPostRedisplay();
            break;
        case 'Z':               // Zoom in
            if (VERBOSE) Log("Zoom in.");
            EyeDist -= 1;
            if(EyeDist > 50) EyeDist = 50;
            ChangeCamera();
            glutPostRedisplay();
            break;
        case 'c':       // Switch between axiometric and perspective View
            if (VERBOSE) {
                if (!eulerCamera) {
                    Log("Enable Euler camera.");
                } else {
                    Log("Enable gluLookAt camera.");
                }
            }
            eulerCamera = !eulerCamera;
            lookCamera = !eulerCamera;
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
        case 'k':
            if(!fly) {
                if (VERBOSE) Log("Enable Fly-through mode.");
            } else {
                if (VERBOSE) Log("Enable Walk mode.");
            }
            fly = !fly;
            walk = !fly;
            glutPostRedisplay();
            break;
        case 'f':               // Toggle floor
            if (!Floor) {
                if (VERBOSE) Log("Enable floor.");
            } else {
                if (VERBOSE) Log("Disable floor.");
            }
            Floor = !Floor;
            glutPostRedisplay();
            break;
        case 'F':               // Toggle axes
            if (!Axes) {
                if (VERBOSE) Log("Enable axes.");
            } else {
                if (VERBOSE) Log("Disable axes.");
            }
            Axes = !Axes;
            glutPostRedisplay();
            break;
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
