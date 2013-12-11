#include "Impact.h"
Point Impact::ImpactPoint;


Impact::Type Impact::impact(const Snake &sn, float time)
{
	CMap *map = sn._map;
    double angle = atan2(sn._speed.y, sn._speed.x) + 2*M_PI;
    Point p = map->GetWorldSize();
    Sint32 x, y, w = Sint32(p.x), h = Sint32(p.y);
    float fx, fy;
    Uint8 res;

    for(double a = angle - M_PI/3; a <= angle + M_PI/3; a += M_PI*30/180)
    {
        fx = (float)(sn._pos.x + (sn._pos.r - 1.5f) * cos(a));
        fy = (float)(sn._pos.y + (sn._pos.r - 1.5f) * sin(a));
        x = (Sint32)fx;
        y = (Sint32)fy;

        if(x < 0 || x >= w || y < 0 || y >= h)
            continue;

        res = map->GetPoint(x, y);
        ImpactPoint = Point(fx, fy);

        switch(res)
        {
        case Map::PointType.Snake:
            if(!sn.IsGhost())
                return Type::snake_crash;
            break;
        case Map::PointType.Bonus:
            return Type::snake_bonus;
        }
    }

	return Type::nothing;
}

