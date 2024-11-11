// sound

#include "../include.h"
RSound __defaulRSound;

// tool

RSound *_load_rsound_by_path(CString path)
{
    RSound s = raudio_LoadSound(path);
    RSound *sound = (RSound *)malloc(sizeof(s));
    sound[0] = s;
    return sound;
}

void __release_rsound(Loadable *loadable) {
    if (loadable->obj) {
        RSound *sound = loadable->obj;
        RSound s = sound[0];
        raudio_UnloadSound(s);
        pct_free(sound);
        loadable->obj = NULL;
    }
}

RSound sound_from_bridge(Bridge *bridge)
{
    Loadable *loadable = Bridge_receiveValue(bridge, UG_TYPE_STF);
    RSound *sound = loadable->obj;
    RSound s = sound != NULL ? sound[0] : __defaulRSound;
    return s;
}

// api

void native_sound_load(Bridge *bridge)
{
    CString path = Bridge_receiveString(bridge);
    RSound *sound = _load_rsound_by_path(path);
    Loadable *loadable = Loadable_newStuf(sound, ALIAS_sound, path, __release_rsound);
    Bridge_returnValue(bridge, loadable);
}

void native_sound_play(Bridge *bridge)
{
    RSound sound = sound_from_bridge(bridge);
    raudio_PlaySound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_stop(Bridge *bridge)
{
    RSound sound = sound_from_bridge(bridge);
    raudio_StopSound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_resume(Bridge *bridge)
{
    RSound sound = sound_from_bridge(bridge);
    raudio_ResumeSound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_pause(Bridge *bridge)
{
    RSound sound = sound_from_bridge(bridge);
    raudio_PauseSound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_is_playing(Bridge *bridge)
{
    RSound sound = sound_from_bridge(bridge);
    bool r = raudio_IsSoundPlaying(sound);
    Bridge_returnBoolean(bridge, r);
}

void native_sound_set_volume(Bridge *bridge)
{
    RSound sound = sound_from_bridge(bridge);
    float volume = Bridge_receiveNumber(bridge);
    raudio_SetSoundVolume(sound, volume);
    Bridge_returnEmpty(bridge);
}
