#ifndef _PRIMITIVES_H
#define _PRIMITIVES_H


class Point
{
public:
    Point();
    Point(float __x, float __y);
    float x, y;
};
class Circle
{
public:
    Circle();
    Circle(float __x, float __y, float __r);
    float x, y, r;
};
class Rect
{
public:
    Rect();
    Rect(float __x, float __y, float __w, float __h);
    float x, y, w, h;
};


#endif