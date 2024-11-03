// sound

#include "raylib.h"
#include "../uyghur.c"

// tool

Sound *raylib_load_sound_by_path(CString path)
{
    Sound s = LoadSound(path);
    Sound *sound = (Sound *)malloc(sizeof(s));
    sound[0] = s;
    return sound;
}

void _raylib_release_sound(Loadable *loadable) {
    if (loadable->obj) {
        Sound *sound = loadable->obj;
        Sound s = sound[0];
        UnloadSound(s);
        pct_free(sound);
        loadable->obj = NULL;
    }
}

Sound sound_from_bridge(Bridge *bridge)
{
    Loadable *loadable = Bridge_receiveValue(bridge, UG_TYPE_STF);
    Sound *sound = loadable->obj;
    Sound s = sound != NULL ? sound[0] : defaulSound;
    return s;
}

// api

void native_sound_load(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    Sound *sound = raylib_load_sound_by_path(path);
    Loadable *loadable = Loadable_newStuf(sound, ALIAS_sound, path, _raylib_release_sound);
    Bridge_returnValue(bridge, loadable);
}

void native_sound_unload(Bridge *bridge) {}

void native_sound_play(Bridge *bridge)
{
    Sound sound = sound_from_bridge(bridge);
    PlaySound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_stop(Bridge *bridge)
{
    Sound sound = sound_from_bridge(bridge);
    StopSound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_resume(Bridge *bridge)
{
    Sound sound = sound_from_bridge(bridge);
    ResumeSound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_pause(Bridge *bridge)
{
    Sound sound = sound_from_bridge(bridge);
    PauseSound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_is_playing(Bridge *bridge)
{
    Sound sound = sound_from_bridge(bridge);
    bool r = IsSoundPlaying(sound);
    Bridge_returnBoolean(bridge, r);
}

void native_sound_set_volume(Bridge *bridge)
{
    Sound sound = sound_from_bridge(bridge);
    float volume = Bridge_receiveNumber(bridge);
    SetSoundVolume(sound, volume);
    Bridge_returnEmpty(bridge);
}
