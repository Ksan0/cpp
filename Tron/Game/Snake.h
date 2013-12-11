#ifndef _GAME_SNAKE_H
#define _GAME_SNAKE_H


#include <SDL2_gfxPrimitives.h>
#include "../Core/Object.h"
#include "../Core/Converter.h"
#include "../Core/Map.h"
#include "Impact.h"
#include "Effect.h"
#include "../Core/ConfigFile.h"


class Effect;
class Snake
{
public:
    Snake(Circle pos, Point speed, CMap *map);
    ~Snake();

    void SetPos(Circle pos);

    void SetSpeed(Point speed);
    void SetSpeedBorder(float min, float max);
    void SetColor(Uint8 r, Uint8 g, Uint8 b);
    void SetEnergy(float en);
    void SetActive(bool ac);
    void SetSuperGhost(bool val);

    void MoveDrawFillMap(SDL_Renderer *rnd, float sPast);
    
    void CreateCrashEffect();
    void RemoveCrashEffect();
    Effect* GetCrashEffect();

    void KeyDown(SDL_Keycode key);
    void KeyUp(SDL_Keycode key);
    void UpAllKeys();

    float GetEnergy() const;
    bool IsGhost() const;
private:
    void Move(float sPast);
    void Draw(SDL_Renderer *rnd, float sPast);
    void FillMap(const Circle &pos);

    Circle _pos;
    Point _speed;
    float _speedMin, _speedMax;
    float _rotateSpeed, _aceleration;
    Uint32 _color;
    CMap *_map; // don't delete at destructor

    const float _energyRegen, _energyUse, _energyMax;
    float _energyCurrent;
    bool _ghost, _superGhost;

    Effect *_effect; // delete at destructor
    bool _active;

    friend class Impact;
    friend class Effect;
};


#endif
