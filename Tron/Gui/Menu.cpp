#include "Menu.h"

// ----------------------------MenuItem Begin ------------------------------------------

MenuItem::MenuItem()
{
    _mode = Mode::HMenu;
    _inFocus = 0;
}
MenuItem::MenuItem(const char *selection)
{
    Set(selection);
    _inFocus = 0;
}
MenuItem::MenuItem(const string &selection)
{
    Set(selection);
    _inFocus = 0;
}
MenuItem::MenuItem(const vector<string> &selections)
{
    Set(selections);
    _inFocus = 0;
}
MenuItem::MenuItem(bool flag)
{
    Set(flag);
    _inFocus = 0;
}


void MenuItem::SetMode(Mode mode)
{
    _mode = mode;
}
void MenuItem::Set(const char *selection)
{
    Set(string(selection));
}
void MenuItem::Set(const string &selection)
{
    _mode = Mode::HMenu;
    _selections.clear();
    _selections.push_back(selection);
}
void MenuItem::Set(const vector<string> &selections)
{
    _mode = Mode::HMenu;
    _selections = selections;
}
void MenuItem::Set(bool flag)
{
    _mode = Mode::Bool;
    _flag = flag;
}

string MenuItem::GetFocusedString()
{
    if(_selections.empty())
        return "";
    return _selections[_inFocus];
}
void MenuItem::KeyDown(SDL_Keycode key)
{
    if(key == SDLK_a || key == SDLK_LEFT)
    {
        if(_inFocus == 0)
            _inFocus = _selections.size() - 1;
        else
            --_inFocus;
       return;
    }

    if(key == SDLK_d || key == SDLK_RIGHT)
    {
        if(_inFocus == _selections.size() - 1)
            _inFocus = 0;
        else
            ++_inFocus;
        return;
    }
}

// ----------------------------MenuItem End  -------------------------------------------

// ----------------------------Menu Begin  ---------------------------------------------

Menu::Menu(TTF_Font *font) : _inFocus(0), _pos(0, 0), _font(font)
{
}
Menu::Menu(Point pos, TTF_Font *font) : _inFocus(0), _pos(pos), _font(font)
{
}
Menu::~Menu()
{
    for(auto it = _menuItems.begin(); it != _menuItems.end(); ++it)
        delete *it;
}

void Menu::SetPos(Point pos)
{
    _pos = pos;
}
void Menu::SetFont(TTF_Font *font)
{
    _font = font;
}
void Menu::SetTextColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    _textColor.r = r;
    _textColor.g = g;
    _textColor.b = b;
    _textColor.a = a;
}
void Menu::SetBgColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    _bgColor = (a << 24) | (r << 16) | (g << 8) | (b << 0);
}
void Menu::AddItem(MenuItem *item, unsigned int pos)
{
    if(pos == (unsigned int)-1)
        _menuItems.push_back(item);
    else
        _menuItems.insert(_menuItems.begin() + pos, item);
}

void Menu::Draw(SDL_Renderer *renderer)
{
    if(_font == nullptr)
        throw Exception("No menu font");

    vector<SDL_Surface*> surfaces;
    Uint32 maxLenIndex = 0;

    for(Uint32 i = 0; i < _menuItems.size(); ++i)
    {
        string str = _menuItems[i]->GetFocusedString();
        SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(_font, str.data(), _textColor, -1);
        surfaces.push_back(surf);
        if(surf->w > surfaces[maxLenIndex]->w)
            maxLenIndex = i;
    }

    Uint32 x, y, dx, dy, step_y;
    Converter::ScreenPercentToPixel(_pos.x, _pos.y, &x, &y);
    Converter::ScreenPercentToPixel(0.05f, 0.02f, &dx, &dy);
    Converter::ScreenPercentToPixel(0, 0.01f, nullptr, &step_y);

    SDL_Rect rectScreen = {x, y, surfaces[maxLenIndex]->w + 2*dx};
    for(Uint32 i = 0; i < surfaces.size(); ++i)
    {
        SDL_Surface *surf = SDL_CreateRGBSurface(0, surfaces[maxLenIndex]->w + 2*dx, surfaces[i]->h + 2*dy, 32, 0, 0, 0, 0);
        
        if(i == _inFocus)
            SDL_FillRect(surf, nullptr, (~_bgColor) | 0xFF);
        else
            SDL_FillRect(surf, nullptr, _bgColor);

        SDL_Rect rect = {(surf->w - surfaces[i]->w)/2, dy};
        SDL_BlitSurface(surfaces[i], nullptr, surf, &rect);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

        rectScreen.h = surfaces[i]->h + 2*dy;
        SDL_RenderCopy(renderer, texture, nullptr, &rectScreen);
        
        rectScreen.y += surf->h + step_y;

        // cannot free "surfaces" here: we will free MaxLenSurface
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(texture);
    }

    for(Uint32 i = 0; i < surfaces.size(); ++i)
        SDL_FreeSurface(surfaces[i]);
}

void Menu::KeyDown(SDL_Keycode key)
{
    if(key == SDLK_w || key == SDLK_UP)
    {
        if(_inFocus == 0)
            _inFocus = _menuItems.size() - 1;
        else
            --_inFocus;
       return;
    }

    if(key == SDLK_s || key == SDLK_DOWN)
    {
        if(_inFocus == _menuItems.size() - 1)
            _inFocus = 0;
        else
            ++_inFocus;
        return;
    }

    _menuItems[_inFocus]->KeyDown(key);
}

string Menu::GetFocusedString()
{
    return _menuItems[_inFocus]->GetFocusedString();
}

// ----------------------------Menu End  -----------------------------------------------
