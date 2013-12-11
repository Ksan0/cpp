#include "Primitives.h"


Point::Point()
{
}
Point::Point(float __x, float __y)
{
    x = __x;
    y = __y;
}

Circle::Circle()
{
}
Circle::Circle(float __x, float __y, float __r)
{
    x = __x;
    y = __y;
    r = __r;
}

Rect::Rect()
{
}
Rect::Rect(float __x, float __y, float __w, float __h)
{
    x = __x;
    y = __y;
    w = __w;
    h = __h;
}
