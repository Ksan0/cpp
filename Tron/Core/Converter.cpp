#include "Converter.h"
unsigned int Converter::_scrW, Converter::_scrH;
Rect Converter::_mapSize;
Point Converter::_worldSize;


void Converter::InitGui(unsigned int scrW, unsigned int scrH)
{
    _scrW = scrW;
    _scrH = scrH;
}
void Converter::InitMap(const Rect &mapSize, Point worldSize)
{
    _mapSize = mapSize;
    _worldSize = worldSize;
}


void Converter::ScreenPercentToPixel(float percentX, float percentY, unsigned int *pixelX, unsigned int *pixelY)
{
    if(pixelX != nullptr)
        *pixelX = (unsigned int)(float(_scrW) * percentX);
    if(pixelY != nullptr)
        *pixelY = (unsigned int)(float(_scrH) * percentY);
}
void Converter::GameWorldToPixel(float gameX, float gameY, unsigned int *pixelX, unsigned int *pixelY)
{
    unsigned int x, y;

    ScreenPercentToPixel(_mapSize.x, _mapSize.y, &x, &y);

    if(pixelX != nullptr)
        *pixelX = (unsigned int)(x + gameX);
    if(pixelY != nullptr)
        *pixelY = (unsigned int)(y + gameY);
}
