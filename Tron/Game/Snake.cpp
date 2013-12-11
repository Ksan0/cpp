#include "Snake.h"
//#include <iostream>
//#include <iomanip>
//using std::cout;
//using std::endl;


Snake::Snake(Circle pos, Point speed, CMap *map)
    : _energyRegen(30), _energyUse(100), _energyMax(100)
{
    _energyCurrent = 50;
    _ghost = false;
    _superGhost = false;
    SetPos(pos);
    SetSpeed(speed);
    _map = map;
    _rotateSpeed = 0;
    _aceleration = 0;
    _active = true;
    _effect = nullptr;
}
Snake::~Snake()
{
    if(_effect != nullptr)
        delete _effect;
}
void Snake::SetPos(Circle pos)
{
    _pos = pos;
}
void Snake::SetSpeed(Point speed)
{
    _speed = speed;
}
void Snake::SetSpeedBorder(float min, float max)
{
    _speedMin = min;
    _speedMax = max;
}
void Snake::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
    _color = 0;
    _color = (b << 16) | (g << 8) | r;
}
void Snake::SetEnergy(float en)
{
    _energyCurrent = en;
    if(_energyCurrent < 0)
        _energyCurrent = 0;
    else if(_energyCurrent > _energyMax)
        _energyCurrent = _energyMax;
}
void Snake::SetActive(bool ac)
{
    _active = ac;
}
void Snake::SetSuperGhost(bool val)
{
    _superGhost = val;
}
float Snake::GetEnergy() const
{
    return _energyCurrent;
}
bool Snake::IsGhost() const
{
    return _ghost || _superGhost;
}

void Snake::KeyDown(SDL_Keycode key)
{
    if(!_active)
        return;

    static float rotate_speed = float(M_PI/180 * 90),
                 aceleration = 50;

    if(key == SDLK_a || key == SDLK_LEFT)
        _rotateSpeed = -rotate_speed;
    else if(key == SDLK_d || key == SDLK_RIGHT)
        _rotateSpeed = rotate_speed;
    else if(key == SDLK_w || key == SDLK_UP)
        _aceleration = aceleration;
    else if(key == SDLK_s || key == SDLK_DOWN)
        _aceleration = -aceleration;
    else if(key == SDLK_SPACE || key == SDLK_RCTRL)
        if(_energyCurrent > 0)
            _ghost = true;

}
void Snake::KeyUp(SDL_Keycode key)
{
    if(!_active)
        return;

    if((key == SDLK_a || key == SDLK_LEFT) && _rotateSpeed < 0)
        _rotateSpeed = 0;
    else if((key == SDLK_d || key == SDLK_RIGHT) && _rotateSpeed > 0)
        _rotateSpeed = 0;
    else if((key == SDLK_w || key == SDLK_UP) && _aceleration > 0)
        _aceleration = 0;
    else if((key == SDLK_s || key == SDLK_DOWN) && _aceleration < 0)
        _aceleration = 0;
    else if(key == SDLK_SPACE || key == SDLK_RCTRL)
        _ghost = false;
}
void Snake::UpAllKeys()
{
    _aceleration = 0;
    _rotateSpeed = 0;
}


void Snake::MoveDrawFillMap(SDL_Renderer *rnd, float sPast)
{
    Draw(rnd, sPast);
    Move(sPast);
    FillMap(_pos);
}


void Snake::Draw(SDL_Renderer *rnd, float sPast)
{
    if(!_active)
        return;

    if(_ghost)
    {
        _energyCurrent -= _energyUse * sPast;
        if(_energyCurrent < 0)
            _energyCurrent = 0;
        if(_energyCurrent == 0)
            _ghost = false;
    } else {
        _energyCurrent += _energyRegen * sPast * sqrt(_speed.x*_speed.x + _speed.y*_speed.y) / _speedMax;
        if(_energyCurrent > _energyMax)
            _energyCurrent = _energyMax;
    }

    const Uint32 steps_n = Uint32(sPast * 5 * sqrt(_speed.x*_speed.x + _speed.y*_speed.y) / _pos.r + 1.5);
    float distX = _speed.x * sPast / steps_n, distY = _speed.y * sPast / steps_n;

    auto flamb = [](Uint32 color, Uint8 alpha, bool white) -> Uint32
    {
        if(!white)
            return (color & 0x00FFFFFF) | (alpha << 24);
        Uint8 r = color & 0x000000FF, g = (color & 0x0000FF00) >> 8, b = (color & 0x00FF0000) >> 16;
#define max(a, b) (a > b ? (a) : (b))
        Uint8 m = max(r, max(g, b));
#undef max
        return (alpha << 24) | (m << 16) | (m << 8) | m;
    };

    Uint32 x, y;
    for(Uint32 i = 0; i < steps_n; ++i)
    {
        Converter::GameWorldToPixel(_pos.x + distX * i, _pos.y + distY * i, &x, &y);
        if(_ghost || _superGhost)
        {
            filledCircleColor(rnd, x, y, Uint32(_pos.r/3), flamb(_color, 10, false));
            filledCircleColor(rnd, x, y, Uint32(_pos.r/1.5), flamb(_color, 5, false));
        } else {
            filledCircleColor(rnd, x, y, Uint32(_pos.r/3), flamb(_color, 24, true));
            filledCircleColor(rnd, x, y, Uint32(_pos.r/2), flamb(_color, 8, false));
            filledCircleColor(rnd, x, y, Uint32(_pos.r/1), flamb(_color, 3, false));
        }
    }
}


void Snake::Move(float sPast)
{
    if(!_active)
        return;

    if(_rotateSpeed != 0)
    {
        double  speed = sqrt(_speed.x * _speed.x + _speed.y * _speed.y),
                angle = atan2(_speed.y, _speed.x);

        angle += _rotateSpeed * sPast;

        _speed.x = float(speed * cos(angle));
        _speed.y = float(speed * sin(angle));
    }

    if(_aceleration != 0)
    {
        double angle = atan2(_speed.y, _speed.x);
        double speed = sqrt(_speed.x*_speed.x + _speed.y*_speed.y);
        speed += _aceleration * sPast;
        if(speed < _speedMin)
            speed = _speedMin;
        else if(speed > _speedMax)
            speed = _speedMax;
        _speed.x = float(speed * cos(angle));
        _speed.y = float(speed * sin(angle));
    }

    // move
    if(_ghost || _superGhost)
    {
        double angle = atan2(_speed.y, _speed.x);
        _pos.x += float(_speedMax * cos(angle) * sPast);
        _pos.y += float(_speedMax * sin(angle) * sPast);
    } else {
        _pos.x += _speed.x * sPast;
        _pos.y += _speed.y * sPast;
    }

    // out of map
    Point worldSize = _map->GetWorldSize();
    if(_pos.x < _pos.r)
        _pos.x += worldSize.x - 2*_pos.r;
    else if(_pos.x >= worldSize.x - _pos.r)
        _pos.x -= worldSize.x - 2*_pos.r;

    if(_pos.y < _pos.r)
        _pos.y += worldSize.y - 2*_pos.r;
    else if(_pos.y >= worldSize.y - _pos.r)
        _pos.y -= worldSize.y - 2*_pos.r;
}


void Snake::FillMap(const Circle &pos)
{
    if(!_active)
        return;

    Sint32 x = Sint32(pos.x), y = Sint32(pos.y), r = Sint32(pos.r * 0.33);
    
    Point worldSize = _map->GetWorldSize();
    Sint32 w = (Sint32)worldSize.x, h = (Sint32)worldSize.y;

    for(Sint32 i = x - r; i <= x + r; ++i)
    {
        if(i < 0 || i >= w)
            continue;
        
        for(Sint32 j = y - r; j <= y + r; ++j)
        {
            if(j < 0 || j >= h)
                continue;

            _map->SetPoint(i, j, Map::PointType.Snake);
        }
    }
}

void Snake::CreateCrashEffect()
{
    if(_effect != nullptr)
        delete _effect;

    _effect = new Effect(Impact::ImpactPoint, this);
}
void Snake::RemoveCrashEffect()
{
    if(_effect == nullptr)
        return;

    delete _effect;
    _effect = nullptr;
}
Effect* Snake::GetCrashEffect()
{
    return _effect;
}
