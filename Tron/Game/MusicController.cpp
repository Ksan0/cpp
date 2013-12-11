#include "MusicController.h"


MusicController& MusicController::GetInstance()
{
    static MusicController mc;
    return mc;
}


MusicController::MusicController()
{
}


void MusicController::Quit()
{
    for(Uint32 i = 1; i < _chunks.size(); ++i)
        Mix_FreeMusic(_chunks[i]);
        

    Mix_CloseAudio();
}


void MusicController::Init()
{
    if( Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
        throw Exception("Can't open audio");

    stringstream ss;
    string str;
    Mix_Music *chunk;
    ConfigFile &cf = Singleton<ConfigFile>::GetInstance();

    int beg = 0, end = 0;
    try
    {
        beg = cf.GetField_Uint32("track_begin");
        end = cf.GetField_Uint32("track_end") + 1;
    }
    catch(Exception)
    {
    }


    for(int i = beg; i < end; ++i)
    {
        ss << "audio/track" << i << ".ogg";
        ss >> str;
        ss.clear();
        chunk = Mix_LoadMUS(str.data());
        if(chunk == nullptr)
            throw Exception("Can't load audio");
        _chunks.push_back(chunk);
    }
}


void MusicController::Action()
{
    static bool music_off = Singleton<ConfigFile>::GetInstance().GetField_bool("music_off");

    if(!music_off && Mix_PlayingMusic() == 0 && !_chunks.empty())
    {
        Mix_Music *chunk = _chunks[rand()%_chunks.size()];
        Mix_PlayMusic(_chunks[rand()%_chunks.size()], 0);
    }
}
