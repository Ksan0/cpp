#ifndef _GUI_OBJECT_H
#define _GUI_OBJECT_H


#include <SDL.h>
#include "../Primitives.h"
#include "../Patterns/Singleton.h"
#include <vector>
using std::vector;


class Object;
class Gui
{
public:
    void Init(SDL_Renderer *renderer);
    void Action();
private:
    Gui();
    Gui(const Gui &);
    Gui& operator=(const Gui &);

    void Register(Object *obj);
    void Unregister(Object *obj);

    vector<Object*> _objects; // delete all at destructor
    SDL_Renderer *_renderer; // don't delete at destructor

    friend class Object;
    friend class Singleton<Gui>;
};



template<>
class Singleton<Gui>
{
public:
    static Gui& GetInstance();
};


class Object
{
public:
    Object();
    virtual ~Object();
private:
    virtual void Draw(SDL_Renderer *renderer) = 0;

    friend class Gui;
};


#endif
