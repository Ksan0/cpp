#include "Object.h"


Gui::Gui()
{
    _renderer = nullptr;
}
void Gui::Init(SDL_Renderer *renderer)
{
    _renderer = renderer;
}
void Gui::Register(Object *obj)
{
    _objects.push_back(obj);
}
void Gui::Unregister(Object *obj)
{
    for(auto it = _objects.begin(); it != _objects.end(); ++it)
        if(*it == obj)
        {
            _objects.erase(it);
            break;
        }
}
void Gui::Action()
{
    for(auto it = _objects.begin(); it != _objects.end(); ++it)
        (*it)->Draw(_renderer);
}


Gui& Singleton<Gui>::GetInstance()
{
    static Gui instance;
    return instance;
}


Object::Object()
{
    Singleton<Gui>::GetInstance().Register(this);
}
Object::~Object()
{
    Singleton<Gui>::GetInstance().Unregister(this);
}

void Object::Draw(SDL_Renderer *renderer)
{
}