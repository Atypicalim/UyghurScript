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
    char *path = Bridge_popString(bridge);
    char *tag = get_audio_tag_for_music(path);
    raylib_load_music_by_tag(tag, path);
    Bridge_startResult(bridge);
    Bridge_pushString(bridge, tag);
    Bridge_return(bridge);
}

void ug_board_music_unload(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    raylib_unload_music_by_tag(tag);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_music_play(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    PlayMusicStream(music);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_music_stop(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    StopMusicStream(music);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_music_resume(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    ResumeMusicStream(music);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_music_pause(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    PauseMusicStream(music);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_music_is_playing(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    bool r = IsMusicStreamPlaying(music);
    Bridge_startResult(bridge);
    Bridge_pushBoolean(bridge, r);
    Bridge_return(bridge);
}

void ug_board_music_set_volume(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float volume = Bridge_popNumber(bridge);
    SetMusicVolume(music, volume);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_music_update(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    UpdateMusicStream(music);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

void ug_board_music_get_length(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float r = GetMusicTimeLength(music);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_board_music_get_position(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float r = GetMusicTimePlayed(music);
    Bridge_startResult(bridge);
    Bridge_pushNumber(bridge, r);
    Bridge_return(bridge);
}

void ug_board_music_set_position(Bridge *bridge)
{
    char *tag = Bridge_popString(bridge);
    Music music = raylib_get_music_by_tag(tag);
    float position = Bridge_popNumber(bridge);
    SeekMusicStream(music, position);
    Bridge_startResult(bridge);
    Bridge_return(bridge);
}

// other

void lib_raylib_music_register(Bridge *bridge)
{
    uyghurBridge = bridge;
    resourcesMap = Hashmap_new(NULL);
    //
    Bridge_startBox(bridge, "muzika");
    //
    Bridge_pushKey(bridge, "ekirish");
    Bridge_pushFunction(bridge, ug_board_music_load);
    Bridge_pushKey(bridge, "tazilash");
    Bridge_pushFunction(bridge, ug_board_music_unload);
    Bridge_pushKey(bridge, "quyush");
    Bridge_pushFunction(bridge, ug_board_music_play);
    Bridge_pushKey(bridge, "toxtitish");
    Bridge_pushFunction(bridge, ug_board_music_stop);
    Bridge_pushKey(bridge, "turghuzush");
    Bridge_pushFunction(bridge, ug_board_music_pause);
    Bridge_pushKey(bridge, "mangghuzush");
    Bridge_pushFunction(bridge, ug_board_music_resume);
    Bridge_pushKey(bridge, "quyuliwatamdu");
    Bridge_pushFunction(bridge, ug_board_music_is_playing);
    Bridge_pushKey(bridge, "yuqiriliqiniBikitish");
    Bridge_pushFunction(bridge, ug_board_music_set_volume);
    Bridge_pushKey(bridge, "yingilash");
    Bridge_pushFunction(bridge, ug_board_music_update);
    Bridge_pushKey(bridge, "uzunliqiniElish");
    Bridge_pushFunction(bridge, ug_board_music_get_length);
    Bridge_pushKey(bridge, "orniniElish");
    Bridge_pushFunction(bridge, ug_board_music_get_position);
    Bridge_pushKey(bridge, "orniniBikitish");
    Bridge_pushFunction(bridge, ug_board_music_set_position);
    //
    Bridge_register(bridge);
}
