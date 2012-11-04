#ifndef SOUND_H
#define SOUND_H

#include <SDL/SDL_mixer.h>

class Sound
{
private:
    Mix_Chunk *sound;
public:
    static bool init();
    static void kill();
    Sound(){sound = NULL;};
    ~Sound(){};
    void load(char fileName[]);
    void play();
    void play(int times);
    void free();
};

class Music
{
private:
    Mix_Music *music;
public:
    Music(){music = NULL;};
    ~Music(){};
    void load(char fileName[]);
    void play();
    void pause();
    void stop();
    bool isPlaying();
    void free();
};

#endif
