
#ifndef POINT_H_
#define POINT_H_

#include <iostream>
/**
 * @class Point
 *
 * Basic class that implements the idea of a point in a 3D scene.
 */
class Point
{
    public:
        /// Constructor
        Point(float px = 0., float py = 0., float pz = 0.);

        /// Copy constructor.
        Point(const Point &p);

    public:
        float x, y, z;

        float length();
        void normalize();

        /// Overloaded operators

        inline Point & operator=(const Point &p)
        {
            x = p.x;
            y = p.y;
            z = p.z;
            return *this;
        }
};

// Operators

Point operator+(const Point &, const Point &);
Point operator-(const Point &, const Point &);
Point operator*(const Point &, const float &);
Point operator*(const float &, const Point &);
Point operator/(const Point &, const float &);
Point operator/(const float &, const Point &);
Point & operator+=(Point &, const Point &);
Point & operator-=(Point &, const Point &);
Point & operator*=(Point &, const float &);
Point & operator/=(Point &, const float &);

std::ostream & operator<<(std::ostream &os, const Point &p);

#endif /* POINT_H_ */
