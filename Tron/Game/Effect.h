#ifndef _EFFECT_H
#define _EFFECT_H

#include "../Primitives.h"
#include "Snake.h"
#include <vector>
using std::vector;

class Snake;
class Effect
{
public:
    Effect(Point impact, Snake *snake);
    void Move(float sPast);
    void Draw(SDL_Renderer *rnd);
private:
    struct Particle
    {
        Point pos, speed;
    };

    vector<Particle> _particles;
    Uint32 _color;
    const CMap *const _map; // don't delete at destructor
};


#endif
