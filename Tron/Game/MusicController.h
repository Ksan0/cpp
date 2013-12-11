#ifndef _MUSIC_CONTROLLER_H
#define _MUSIC_CONTROLLER_H

#include "../Core/ConfigFile.h"
#include <SDL_mixer.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>
using std::map;
using std::string;
using std::stringstream;
using std::vector;


class MusicController
{
public:
    static MusicController& GetInstance();
    void Init();
    void Quit();
    void Action();
private:
    MusicController();
    MusicController(const MusicController &);
    MusicController& operator=(const MusicController &);

    vector<Mix_Music*> _chunks; // delete at destructor
};


#endif
