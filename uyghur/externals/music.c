// music

#include "raylib.h"
#include "../uyghur.c"

// tool

USTRING get_audio_tag_for_music(USTRING path)
{
    return tools_format("<R-MUSIC:%s>", path);
}

Music raylib_load_music_by_tag(USTRING tag, USTRING path)
{
    Music *music = Hashmap_get(resourcesMap, tag);
    if (music != NULL) {
        return music[0];
    }
    Music m = LoadMusicStream(path);
    music = (Music *)malloc(sizeof(m));
    music[0] = m;
    Hashmap_set(resourcesMap, tag, music);
    return m;
}

Music raylib_get_music_by_tag(USTRING tag)
{
    Music *music = Hashmap_get(resourcesMap, tag);
    if (music == NULL) return LoadMusicStream("");
    return music[0];
}

void raylib_unload_music_by_tag(USTRING tag)
{
    Music *music = Hashmap_get(resourcesMap, tag);
    if (music == NULL) return;
    UnloadMusicStream(music[0]);
}

// api

void native_music_load(Bridge *bridge)
{
    USTRING path = Bridge_receiveString(bridge);
    USTRING tag = get_audio_tag_for_music(path);
    raylib_load_music_by_tag(tag, path);
    Bridge_returnString(bridge, tag);
}

void native_music_unload(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    raylib_unload_music_by_tag(tag);
    Bridge_returnEmpty(bridge);
}

void native_music_play(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    PlayMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void native_music_stop(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    StopMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void native_music_resume(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    ResumeMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void native_music_pause(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    PauseMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void native_music_is_playing(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    bool r = IsMusicStreamPlaying(music);
    Bridge_returnBoolean(bridge, r);
}

void native_music_set_volume(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float volume = Bridge_receiveNumber(bridge);
    SetMusicVolume(music, volume);
    Bridge_returnEmpty(bridge);
}

void native_music_update(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    UpdateMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void native_music_get_length(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float r = GetMusicTimeLength(music);
    Bridge_returnNumber(bridge, r);
}

void native_music_get_position(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float r = GetMusicTimePlayed(music);
    Bridge_returnNumber(bridge, r);
}

void native_music_set_position(Bridge *bridge)
{
    USTRING tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float position = Bridge_receiveNumber(bridge);
    SeekMusicStream(music, position);
    Bridge_returnEmpty(bridge);
}
