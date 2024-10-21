// music

#include "raylib.h"
#include "../uyghur.c"

// tool

Music *raylib_load_music_by_path(USTRING path)
{
    Music m = LoadMusicStream(path);
    Music *music = (Music *)malloc(sizeof(m));
    music[0] = m;
    return music;
}

void _raylib_release_music(Loadable *loadable) {
    if (loadable->obj) {
        Music *music = loadable->obj;
        Music m = music[0];
        UnloadMusicStream(m);
        pct_free(music);
        loadable->obj = NULL;
    }
}

Music music_from_bridge(Bridge *bridge)
{
    Loadable *loadable = Bridge_receiveValue(bridge, UG_TYPE_RSR);
    Music *music = loadable->obj;
    Music m = music != NULL ? music[0] : defaulMusic;
    return m;
}

// api

void native_music_load(Bridge *bridge)
{
    USTRING path = Bridge_receiveString(bridge);
    Music *music = raylib_load_music_by_path(path);
    Loadable *loadable = Loadable_newResource(music, ALIAS_music, path, _raylib_release_music);
    Bridge_returnValue(bridge, loadable);
}

void native_music_unload(Bridge *bridge){}

void native_music_play(Bridge *bridge)
{
    Music music = music_from_bridge(bridge);
    PlayMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void native_music_stop(Bridge *bridge)
{
    Music music = music_from_bridge(bridge);
    StopMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void native_music_resume(Bridge *bridge)
{
    Music music = music_from_bridge(bridge);
    ResumeMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void native_music_pause(Bridge *bridge)
{
    Music music = music_from_bridge(bridge);
    PauseMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void native_music_is_playing(Bridge *bridge)
{
    Music music = music_from_bridge(bridge);
    bool r = IsMusicStreamPlaying(music);
    Bridge_returnBoolean(bridge, r);
}

void native_music_set_volume(Bridge *bridge)
{
    Music music = music_from_bridge(bridge);
    float volume = Bridge_receiveNumber(bridge);
    SetMusicVolume(music, volume);
    Bridge_returnEmpty(bridge);
}

void native_music_update(Bridge *bridge)
{
    Music music = music_from_bridge(bridge);
    UpdateMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void native_music_get_length(Bridge *bridge)
{
    Music music = music_from_bridge(bridge);
    float r = GetMusicTimeLength(music);
    Bridge_returnNumber(bridge, r);
}

void native_music_get_position(Bridge *bridge)
{
    Music music = music_from_bridge(bridge);
    float r = GetMusicTimePlayed(music);
    Bridge_returnNumber(bridge, r);
}

void native_music_set_position(Bridge *bridge)
{
    Music music = music_from_bridge(bridge);
    float position = Bridge_receiveNumber(bridge);
    SeekMusicStream(music, position);
    Bridge_returnEmpty(bridge);
}
