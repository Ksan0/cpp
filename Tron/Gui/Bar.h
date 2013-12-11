#ifndef _GUI_BAR_H
#define _GUI_BAR_H


#include <SDL2_gfxPrimitives.h>
#include "../Core/Object.h"
#include "../Core/Converter.h"
#include <string>
using std::string;


class Bar : public Object
{
public:
    Bar();
    Bar(const Rect &pos);
    ~Bar();

    void SetPos(const Rect &pos);
    void SetVal(float val);
    void SetMaxVal(float maxval);
    void LoadTexture(SDL_Renderer *rnd, const string &path);

    float GetVal();
    float GetMaxVal();

    void Draw(SDL_Renderer *renderer);
private:
    Rect _pos;
    float _val, _maxval;
    SDL_Texture *_texture; // delete at destructor
};


#endif
