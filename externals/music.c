// music

#include "raylib.h"
#include "../uyghur/uyghur.c"

// tool

char *get_audio_tag_for_music(char *path)
{
    return tools_format("<R-MUSIC:%s>", path);
}

Music raylib_load_music_by_tag(char *tag, char *path)
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

Music raylib_get_music_by_tag(char *tag)
{
    Music *music = Hashmap_get(resourcesMap, tag);
    if (music == NULL) return LoadMusicStream("");
    return music[0];
}

void raylib_unload_music_by_tag(char *tag)
{
    Music *music = Hashmap_get(resourcesMap, tag);
    if (music == NULL) return;
    UnloadMusicStream(music[0]);
}

// api

void ug_board_music_load(Bridge *bridge)
{
    char *path = Bridge_receiveString(bridge);
    char *tag = get_audio_tag_for_music(path);
    raylib_load_music_by_tag(tag, path);
    Bridge_returnString(bridge, tag);
}

void ug_board_music_unload(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    raylib_unload_music_by_tag(tag);
    Bridge_returnEmpty(bridge);
}

void ug_board_music_play(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    PlayMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void ug_board_music_stop(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    StopMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void ug_board_music_resume(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    ResumeMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void ug_board_music_pause(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    PauseMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void ug_board_music_is_playing(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    bool r = IsMusicStreamPlaying(music);
    Bridge_returnBoolean(bridge, r);
}

void ug_board_music_set_volume(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float volume = Bridge_receiveNumber(bridge);
    SetMusicVolume(music, volume);
    Bridge_returnEmpty(bridge);
}

void ug_board_music_update(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    UpdateMusicStream(music);
    Bridge_returnEmpty(bridge);
}

void ug_board_music_get_length(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float r = GetMusicTimeLength(music);
    Bridge_returnNumber(bridge, r);
}

void ug_board_music_get_position(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float r = GetMusicTimePlayed(music);
    Bridge_returnNumber(bridge, r);
}

void ug_board_music_set_position(Bridge *bridge)
{
    char *tag = Bridge_receiveString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float position = Bridge_receiveNumber(bridge);
    SeekMusicStream(music, position);
    Bridge_returnEmpty(bridge);
}

// other

void lib_raylib_music_register(Bridge *bridge)
{
    //
    Bridge_startBox(bridge, "muzika");
    //
    Bridge_bindNative(bridge, "ekirish", ug_board_music_load);
    Bridge_bindNative(bridge, "tazilash", ug_board_music_unload);
    Bridge_bindNative(bridge, "quyush", ug_board_music_play);
    Bridge_bindNative(bridge, "toxtitish", ug_board_music_stop);
    Bridge_bindNative(bridge, "turghuzush", ug_board_music_pause);
    Bridge_bindNative(bridge, "mangghuzush", ug_board_music_resume);
    Bridge_bindNative(bridge, "quyuliwatamdu", ug_board_music_is_playing);
    Bridge_bindNative(bridge, "yuqiriliqiniBikitish", ug_board_music_set_volume);
    Bridge_bindNative(bridge, "yingilash", ug_board_music_update);
    Bridge_bindNative(bridge, "uzunliqiniElish", ug_board_music_get_length);
    Bridge_bindNative(bridge, "orniniElish", ug_board_music_get_position);
    Bridge_bindNative(bridge, "orniniBikitish", ug_board_music_set_position);
    //
    Bridge_register(bridge);
}
