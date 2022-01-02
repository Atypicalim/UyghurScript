// sound

#include "raylib.h"
#include "../uyghur/uyghur.c"

// tool

char *get_audio_tag_for_sound(char *path)
{
    return tools_format("<R-SOUND:%s>", path);
}

Sound raylib_load_sound_by_tag(char *tag, char *path)
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

Sound raylib_get_sound_by_tag(char *tag)
{
    Sound *sound = Hashmap_get(resourcesMap, tag);
    if (sound == NULL) return LoadSound("");
    return sound[0];
}

void raylib_unload_sound_by_tag(char *tag)
{
    Sound *sound = Hashmap_get(resourcesMap, tag);
    if (sound == NULL) return;
    UnloadSound(sound[0]);
}

// api

void ug_board_sound_load(Bridge *bridge)
{
    char *path = Bridge_nextString(bridge);
    char *tag = get_audio_tag_for_sound(path);
    raylib_load_sound_by_tag(tag, path);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, tag);
    Bridge_return(bridge);
}

void ug_board_sound_unload(Bridge *bridge)
{
    char *tag = Bridge_nextString(bridge);
    raylib_unload_sound_by_tag(tag);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_sound_play(Bridge *bridge)
{
    char *tag = Bridge_nextString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    PlaySound(sound);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_sound_stop(Bridge *bridge)
{
    char *tag = Bridge_nextString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    StopSound(sound);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_sound_resume(Bridge *bridge)
{
    char *tag = Bridge_nextString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    ResumeSound(sound);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_sound_pause(Bridge *bridge)
{
    char *tag = Bridge_nextString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    PauseSound(sound);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_sound_is_playing(Bridge *bridge)
{
    char *tag = Bridge_nextString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    bool r = IsSoundPlaying(sound);
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, r);
    Bridge_return(bridge);
}

void ug_board_sound_set_volume(Bridge *bridge)
{
    char *tag = Bridge_nextString(bridge);
    Sound sound = raylib_get_sound_by_tag(tag);
    float volume = Bridge_nextNumber(bridge);
    SetSoundVolume(sound, volume);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

// other

void lib_raylib_sound_register(Bridge *bridge)
{
    //
    Bridge_startBox(bridge, "awaz");
    //
    Bridge_pushKey(bridge, "ekirish");
    Bridge_pushFunction(bridge, ug_board_sound_load);
    Bridge_pushKey(bridge, "tazilash");
    Bridge_pushFunction(bridge, ug_board_sound_unload);
    Bridge_pushKey(bridge, "quyush");
    Bridge_pushFunction(bridge, ug_board_sound_play);
    Bridge_pushKey(bridge, "toxtitish");
    Bridge_pushFunction(bridge, ug_board_sound_stop);
    Bridge_pushKey(bridge, "turghuzush");
    Bridge_pushFunction(bridge, ug_board_sound_pause);
    Bridge_pushKey(bridge, "mangghuzush");
    Bridge_pushFunction(bridge, ug_board_sound_resume);
    Bridge_pushKey(bridge, "quyuliwatamdu");
    Bridge_pushFunction(bridge, ug_board_sound_is_playing);
    Bridge_pushKey(bridge, "yuqiriliqiniBikitish");
    Bridge_pushFunction(bridge, ug_board_sound_set_volume);
    //
    Bridge_register(bridge);
}
