#include "Sound.h"
    
bool Sound::init()
{
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4596) == -1)
        return false;
        
    return true;
}

void Sound::kill()
{
    Mix_CloseAudio();     
}

void Sound::load(char fileName[])
{
    sound = Mix_LoadWAV(fileName);     
}

void Sound::play()
{
    Mix_PlayChannel(-1,sound,0);
}

void Sound::play(int times)
{
    Mix_PlayChannel(-1,sound,times);
}

void Sound::free()
{
    if(sound != NULL)
    {
        Mix_FreeChunk(sound);
        sound = NULL;
    }
}

void Music::load(char fileName[])
{
    music = Mix_LoadMUS(fileName);
}

void Music::play()
{
    if(!isPlaying())
        Mix_PlayMusic(music,-1);
    else
        if(Mix_PausedMusic())
        {
            Mix_ResumeMusic();
        }
}

void Music::pause()
{
    Mix_PauseMusic();
}

void Music::stop()
{
    Mix_HaltMusic();
}

bool Music::isPlaying()
{
    return (bool)Mix_PlayingMusic();
}

void Music::free()
{
    Mix_FreeMusic(music);     
}     
