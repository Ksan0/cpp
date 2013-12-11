#ifndef _GUI_MENU_H
#define _GUI_MENU_H


#include <SDL.h>
#include <SDL_ttf.h>
#include "../Core/Object.h"
#include "../Core/Converter.h"
#include <string>
#include <vector>
using std::string;
using std::vector;


class MenuItem
{
public:
    enum Mode
    {
        HMenu = 0,
        Bool
    };

    MenuItem();
    MenuItem(const char *selection);
    MenuItem(const string &selection);
    MenuItem(const vector<string> &selections);
    MenuItem(bool flag);

    void SetMode(Mode mode);
    void Set(const char *selection);
    void Set(const string &selection);
    void Set(const vector<string> &selections);
    void Set(bool flag);

    string GetFocusedString();
    void KeyDown(SDL_Keycode key);
private:
    Mode _mode;
    vector<string> _selections;
    unsigned int _inFocus;
    bool _flag;
};


class Menu : public Object
{
public:
    Menu(TTF_Font *font = nullptr);
    Menu(Point pos, TTF_Font *font = nullptr);
    ~Menu();

    void SetPos(Point pos);
    void SetFont(TTF_Font *font);
    void SetTextColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void SetBgColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void AddItem(MenuItem *item, unsigned int pos = -1);

    void KeyDown(SDL_Keycode key);
    string GetFocusedString();
private:
    void Draw(SDL_Renderer *renderer);

    vector<MenuItem*> _menuItems; // delete at destructor
    unsigned int _inFocus;
    Point _pos;

    SDL_Color _textColor;
    Uint32 _bgColor;

    TTF_Font *_font; // don't delete at destructor
};


#endif
