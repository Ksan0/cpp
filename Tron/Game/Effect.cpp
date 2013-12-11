#include "Effect.h"


Effect::Effect(Point impact, Snake *snake) : _map(snake->_map)
{
    _color = snake->_color;

    float speed = (float) sqrt(snake->_speed.x*snake->_speed.x + snake->_speed.y*snake->_speed.y);
    double angle = atan2(snake->_speed.y, snake->_speed.x);

    float d_speed = (snake->_speedMax - snake->_speedMin)/4;
    double d_angle = M_PI/12 + M_PI/3 * (1.0 - speed/snake->_speedMax);


    auto random = [](double min, double max) -> double
    {
        return min + double(rand()) / RAND_MAX * (max - min);
    };
    auto random_angle = [random](double middle, double max_delta)
    {
        middle += 2*M_PI;
        return random(middle - max_delta, middle + max_delta);
    };
    auto random_speed = [random](double middle, double max_delta) -> double
    {
        return random(middle, max_delta);
    };

    _particles.resize(Uint32(50 * speed/snake->_speedMax));
    double p_speed, p_angle;
    for(Uint32 i = 0; i < _particles.size(); ++i)
    {
        _particles[i].pos = Point(snake->_pos.x, snake->_pos.y);
        p_speed = random_speed(speed, d_speed);
        p_angle = random_angle(angle, d_angle);
        _particles[i].speed = Point( float(p_speed * cos(p_angle)), float(p_speed * sin(p_angle)) );
    }
}


void Effect::Move(float sPast)
{
    Point worldSize = _map->GetWorldSize();

    for(Uint32 i = 0; i < _particles.size(); ++i)
    {
        _particles[i].pos.x += _particles[i].speed.x * sPast;
        _particles[i].pos.y += _particles[i].speed.y * sPast;

        if(_particles[i].pos.x < 0 || _particles[i].pos.x > worldSize.x)
        {
            _particles[i].speed.x *= -1;
            _particles[i].pos.x += 2*_particles[i].speed.x * sPast;
        }
        if(_particles[i].pos.y < 0 || _particles[i].pos.y > worldSize.y)
        {
            _particles[i].speed.y *= -1;
            _particles[i].pos.y += 2*_particles[i].speed.y * sPast;
        }
    }
}


void Effect::Draw(SDL_Renderer *rnd)
{
    Uint32 x, y;

    for(Uint32 i = 0; i < _particles.size(); ++i)
    {
        Converter::GameWorldToPixel(_particles[i].pos.x, _particles[i].pos.y, &x, &y);
        pixelColor(rnd, x, y, 0xFF000000 | (150 << 16) | 150);
    }

    Uint32  a = rand()% _particles.size(), 
            b = rand() % _particles.size(), 
            x2, y2;
    Converter::GameWorldToPixel(_particles[a].pos.x, _particles[a].pos.y, &x, &y);
    Converter::GameWorldToPixel(_particles[b].pos.x, _particles[b].pos.y, &x2, &y2);
    lineColor(rnd, x, y, x2, y2, (40 << 24) | (150 << 16) | (150 << 8));
}
