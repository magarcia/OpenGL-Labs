
#include "point.h"
#include <iostream>
#include <cmath>


Point::Point(float px, float py, float pz) : x(px), y(py), z(pz)
{
    /* There's nothing to do here */
}

Point::Point(const Point &p) 
{
    x = p.x;
    y = p.y;
    x = p.z;

}

float Point::length() {
    return sqrt(x * x + y * y + z * z);
}

void Point::normalize() {
    int l = this->length();
    x = x/l;
    y = y/l;
    z = z/l;
}

Point operator+(const Point &p1, const Point &p2)
{
    return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

Point operator-(const Point &p1, const Point &p2)
{
    return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

Point operator*(const Point &p1, const float &x)
{
    return Point(p1.x * x, p1.y * x, p1.z * x);
}

Point operator*(const float &x, const Point &p1)
{
    return Point(p1.x * x, p1.y * x, p1.z * x);
}

Point operator/(const Point &p1, const float &x)
{
    return Point(p1.x / x, p1.y / x, p1.z / x);
}

Point operator/(const float &x, const Point &p1)
{
    return Point(p1.x / x, p1.y / x, p1.z / x);
}

Point & operator+=(Point &p1, const Point &p2)
{
    p1.x += p2.x;
    p1.y += p2.y;
    p1.z += p2.z;
    return p1;
}

Point & operator-=(Point &p1, const Point &p2)
{
    p1.x -= p2.x;
    p1.y -= p2.y;
    p1.z -= p2.z;
    return p1;
}

Point & operator*=(Point &p1, const float &x)
{
    p1.x *= x;
    p1.y *= x;
    p1.z *= x;
    return p1;
}

Point & operator/=(Point &p1, const float &x)
{
    p1.x /= x;
    p1.y /= x;
    p1.z /= x;
    return p1;
}

std::ostream & operator<<(std::ostream &os, const Point &p)
{
    os << p.x << ", " << p.y << ", " << p.z << " ";
    return os;
};
