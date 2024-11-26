// music

#include "./externals.h"
#include "../delegates/header.h"
RMusic __defaulRMusic;

// tool

RMusic *_load_rmusic_by_path(CString path)
{
    RMusic m = replay_LoadMusic(path);
    RMusic *music = (RMusic *)malloc(sizeof(m));
    music[0] = m;
    return music;
}

void __release_rmusic(Loadable *loadable) {
    if (loadable->obj) {
        RMusic *music = loadable->obj;
        RMusic m = music[0];
        replay_UnloadMusic(m);
        pct_free(music);
        loadable->obj = NULL;
    }
}

RMusic music_from_bridge(Bridge *bridge)
{
    Loadable *loadable = Bridge_receiveValue(bridge, UG_TYPE_STF);
    RMusic *music = loadable->obj;
    RMusic m = music != NULL ? music[0] : __defaulRMusic;
    return m;
}

// api

void native_music_load(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    RMusic *music = _load_rmusic_by_path(path);
    Loadable *loadable = Loadable_newStuf(music, ALIAS_music, path, __release_rmusic);
    Bridge_returnValue(bridge, loadable);
}

void native_music_play(Bridge *bridge)
{
    RMusic music = music_from_bridge(bridge);
    replay_PlayMusic(music);
    Bridge_returnEmpty(bridge);
}

void native_music_stop(Bridge *bridge)
{
    RMusic music = music_from_bridge(bridge);
    replay_StopMusic(music);
    Bridge_returnEmpty(bridge);
}

void native_music_resume(Bridge *bridge)
{
    RMusic music = music_from_bridge(bridge);
    replay_ResumeMusic(music);
    Bridge_returnEmpty(bridge);
}

void native_music_pause(Bridge *bridge)
{
    RMusic music = music_from_bridge(bridge);
    replay_PauseMusic(music);
    Bridge_returnEmpty(bridge);
}

void native_music_is_playing(Bridge *bridge)
{
    RMusic music = music_from_bridge(bridge);
    bool r = replay_IsMusicPlaying(music);
    Bridge_returnBoolean(bridge, r);
}

void native_music_set_volume(Bridge *bridge)
{
    RMusic music = music_from_bridge(bridge);
    float volume = Bridge_receiveNumber(bridge);
    replay_SetMusicVolume(music, volume);
    Bridge_returnEmpty(bridge);
}

void native_music_update(Bridge *bridge)
{
    RMusic music = music_from_bridge(bridge);
    replay_UpdateMusic(music);
    Bridge_returnEmpty(bridge);
}

void native_music_get_length(Bridge *bridge)
{
    RMusic music = music_from_bridge(bridge);
    float r = replay_GetMusicLength(music);
    Bridge_returnNumber(bridge, r);
}

void native_music_get_position(Bridge *bridge)
{
    RMusic music = music_from_bridge(bridge);
    float r = replay_GetMusicTime(music);
    Bridge_returnNumber(bridge, r);
}

void native_music_set_position(Bridge *bridge)
{
    RMusic music = music_from_bridge(bridge);
    float position = Bridge_receiveNumber(bridge);
    replay_SetMusicTime(music, position);
    Bridge_returnEmpty(bridge);
}
