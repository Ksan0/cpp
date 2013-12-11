#ifndef _GAME_IMPACT_H
#define _GAME_IMPACT_H


#include "../Primitives.h"
#include "Snake.h"
#include "../Core/Map.h"


class Snake;
class Impact
{
public:
    enum Type
    {
        nothing = 0,
        snake_crash,
        snake_bonus
    };

    static Type impact(const Snake &sn1, float time);

    static Point ImpactPoint;
};


#endif