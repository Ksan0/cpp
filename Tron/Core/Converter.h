#ifndef _CORE_CONVERTER_H
#define _CORE_CONVERTER_H


#include "../Primitives.h"


class Converter
{
public:
    static void InitGui(unsigned int scrW, unsigned int scrH); 
    static void InitMap(const Rect &mapSize, Point worldSize);
    static void ScreenPercentToPixel(float percentX, float percentY, unsigned int *pixelX, unsigned int *pixelY);
    static void GameWorldToPixel(float gameX, float gameY, unsigned int *pixelX, unsigned int *pixelY);
private:
    static unsigned int _scrW, _scrH;
    static Rect _mapSize;
    static Point _worldSize;
};


#endif
