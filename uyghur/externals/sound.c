// sound

#include "raylib.h"
#include "../uyghur.c"

// tool

USTRING get_audio_tag_for_sound(USTRING path)
{
    return tools_format("<R-SOUND:%s>", path);
}

Sound raylib_load_sound_by_tag(USTRING tag, USTRING path)
{
    Sound *sound = Hashmap_get(resourcesMap, tag);
    if (sound != NULL) {
        return sound[0];
    }
    Sound s = LoadSound(path);
    sound = (Sound *)malloc(sizeof(s));
    sound[0] = s;
    Hashmap_set(resourcesMap, tag, sound);
    return s;
}

Sound raylib_get_sound_by_tag(USTRING tag)
{
    Sound *sound = Hashmap_get(resourcesMap, tag);
    if (sound == NULL) return LoadSound("");
    return sound[0];
}

void raylib_unload_sound_by_tag(USTRING tag)
{
    Sound *sound = Hashmap_get(resourcesMap, tag);
    if (sound == NULL) return;
    UnloadSound(sound[0]);
}

// api

void native_sound_load(Bridge *bridge)
{
    USTRING path = Bridge_receiveString(bridge);
    USTRING tag = get_audio_tag_for_sound(path);
    raylib_load_sound_by_tag(tag, path);
    Bridge_returnString(bridge, tag);
}

void native_sound_unload(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    raylib_unload_sound_by_tag(tag);
    Bridge_returnEmpty(bridge);
}

void native_sound_play(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    PlaySound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_stop(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    StopSound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_resume(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    ResumeSound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_pause(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    PauseSound(sound);
    Bridge_returnEmpty(bridge);
}

void native_sound_is_playing(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    bool r = IsSoundPlaying(sound);
    Bridge_returnBoolean(bridge, r);
}

void native_sound_set_volume(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    float volume = Bridge_receiveNumber(bridge);
    SetSoundVolume(sound, volume);
    Bridge_returnEmpty(bridge);
}

// other

void lib_sound_register(Bridge *bridge)
{
    //
    Bridge_startBox(bridge);
    //
    BRIDGE_BIND_NATIVE(sound_load);
    BRIDGE_BIND_NATIVE(sound_unload);
    BRIDGE_BIND_NATIVE(sound_play);
    BRIDGE_BIND_NATIVE(sound_stop);
    BRIDGE_BIND_NATIVE(sound_pause);
    BRIDGE_BIND_NATIVE(sound_resume);
    BRIDGE_BIND_NATIVE(sound_is_playing);
    BRIDGE_BIND_NATIVE(sound_set_volume);
    //
    Bridge_register(bridge, ALIAS_SOUND);
}
