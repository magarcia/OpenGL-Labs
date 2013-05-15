// Planet.h

#ifndef PLANET_H
#define PLANET_H

#include "color.h"
#include "model.h"
using namespace std;

/**
 * @class Planet
 *
 * Helper class that helps us to operate with Planets.
 */
class Planet
{
    private:
        float inclination;
        float orbitDuration;
        float orbitRadius;
        float rotationDuration;
        float radius;
        Color color;

        float Days;
        float Hours;
        float Increment;

        bool moon;
        bool astronaut;
        Model model;
        bool solid;


    public:
        /// Default constructor.
        Planet()
        {
            inclination = 0;
            orbitDuration = 0;
            orbitRadius = 0;
            rotationDuration = 0;
            radius = 0.1;
            color = Color (1, 1, 1, 1);

            Days = 0.0;
            Hours = 0.0;
            moon = false;
            Increment = 1;
            solid = false;
        }

        /// Copy constructor.
        Planet(float i, float oD, float oR, float rD, float r, Color c)
        {
            inclination = i;
            orbitDuration = oD;
            orbitRadius = oR;
            rotationDuration = rD;
            radius = r;
            color = c;

            Days = 0.0;
            Hours = 0.0;
            moon = false;
            Increment = 1;
            solid = false;
        }

        /// Destructor.
        ~Planet() {}

        /// Accessors

        float getInclination() const { return inclination; }
        float getOrbitDuration() const { return orbitDuration; }
        float getOrbitRadius() const { return orbitRadius; }
        float getRotationDuration() const { return rotationDuration; }
        float getRadius() const { return radius; }
        float getIncrement() const { return Increment; }
        float getSolid() const { return solid; }
        const float* getColor() const { return color.rgba(); }

        /// Setters
        void setInclination(float i) { inclination = i; }
        void setOrbitDuration(float oD) { orbitDuration = oD; }
        void setOrbitRadius(float oR) { orbitRadius = oR; }
        void setRotationDuration(float rD) { rotationDuration = rD; }
        void setRadius(float r) { radius = r;}
        void setIncrement(float i) { Increment = i; }
        void makeSolid(float s) { solid = s; }
        void setColor(float r, float g, float b, float a)
        {
            color.setRed(r);
            color.setGreen(g);
            color.setBlue(b);
            color.setAlpha(a);
        }
        void addMoon() {
            moon = true;
        }
        void addAstronaut(char* file) {
            astronaut = true;
            model.load(file);
        }


        /// Utility functions
        void draw()
        {
            if (not moon and not astronaut) {
                glPushMatrix();
                    glRotatef(inclination, 0.0, 0.0, 1.0);

                    if (orbitDuration != 0)
                        glRotatef( 360.0 * (Days/orbitDuration), 0.0, 1.0, 0.0);

                    glTranslatef(orbitRadius, 0.0, 0.0 );

                    if (rotationDuration != 0)
                        glRotatef( 360.0 * (Hours/rotationDuration), 0.0, 1.0, 0.0 );

                    glRotatef( -90.0, 1.0, 0.0, 0.0 );

                    glColor3f(color[0], color[1], color[2]);
                    if (!solid){
                        glutWireSphere(radius, 50, 50);
                    } else {
                        glutSolidSphere(radius, 50, 50);
                    }
                glPopMatrix();
            } else {
                glPushMatrix();
                    glPushMatrix();
                        glRotatef(inclination, 0.0, 0.0, 1.0);

                        if (orbitDuration != 0)
                            glRotatef( 360.0 * (Days/orbitDuration), 0.0, 1.0, 0.0);

                        glTranslatef(orbitRadius, 0.0, 0.0 );

                        if (rotationDuration != 0)
                            glRotatef( 360.0 * (Hours/rotationDuration), 0.0, 1.0, 0.0 );

                        glRotatef( -90.0, 1.0, 0.0, 0.0 );

                        glColor3f(color[0], color[1], color[2]);
                        if (!solid){
                            glutWireSphere(radius, 50, 50);
                        } else {
                            glutSolidSphere(radius, 50, 50);
                        }
                    glPopMatrix();

                    

                    if (moon) {
                        glRotatef(inclination, 0.0, 0.0, 1.0);
                        glRotatef(360.0 * (Days/orbitDuration), 0.0, 1.0, 0.0);
                        glTranslatef(orbitRadius, 0.0, 0.0 );
                        glRotatef(360.0 * Days/100, 0.0, 1.0, 0.0 );
                        glTranslatef(radius + 0.02, 0.0, 0.0 );
                        glColor3f(1, 1, 1);

                        if (!solid) {
                            glutWireSphere(0.01, 50, 50);
                        } else {
                            glutSolidSphere(0.01, 50, 50);
                        }
                    }
                    if (astronaut) {
                        glRotatef(inclination, 0.0, 0.0, 1.0);
                        glRotatef(360.0 * (Days/orbitDuration), 0.0, 1.0, 0.0);
                        glTranslatef(orbitRadius, 0.0, 0.0 );
                        glRotatef(360.0 * Days/500, 0.0, 1.0, 0.0 );
                        glTranslatef(radius + 0.05, 0.0, 0.0 );    
                        glColor3f(1, 1, 1);
                        glScalef(0.05, 0.05, 0.05);

                        glBegin(GL_TRIANGLES);
                            for(unsigned int i = 0; i < model.faces().size(); ++i){
                                const Face &f = model.faces()[i];
                                glVertex3dv(&model.vertices()[f.v[0]]);
                                glVertex3dv(&model.vertices()[f.v[1]]);
                                glVertex3dv(&model.vertices()[f.v[2]]);
                            }
                        glEnd();
                    }

                glPopMatrix();

            }

            if (orbitDuration != 0)
            {
                Days += Increment;
            }
            if (rotationDuration != 0) {
                Hours += Increment;
            }
        }

        /// Overloaded operators
        inline Planet & operator=(const Planet &planet)
        {
            inclination      = planet.inclination;
            orbitDuration    = planet.orbitDuration;
            orbitRadius      = planet.orbitRadius;
            rotationDuration = planet.rotationDuration;
            radius           = planet.radius;
            color            = planet.color;
            Days             = planet.Days;
            Hours            = planet.Hours;
            return *this;
        }
};

std::ostream & operator<<(std::ostream &os, const Planet &p)
{
    os << "  Inclination: "       << p.getInclination()      << std::endl;
    os << "  Orbit duration: "    << p.getOrbitDuration()    << std::endl;
    os << "  Orbit radius: "      << p.getOrbitRadius()      << std::endl;
    os << "  Rotation duration: " << p.getRotationDuration() << std::endl;
    os << "  Radius: "            << p.getRadius()           << std::endl;
    os << "  Color: "             << p.getColor()            << std::endl;
    return os;
};
#endif //PLANET_H