#include "../include/sounds.h"

void togglePlay()
{
    if (Mix_PausedMusic()==1)
    {
        Mix_ResumeMusic();
    }
    else
    {
        Mix_PauseMusic();
    }
}

void loadSoundEffects(SoundEffect *soundEffect)
{
    soundEffect->jump = Mix_LoadWAV("resources/sounds/jump1.wav");
    soundEffect->buttonClick = Mix_LoadWAV("resources/sounds/buttonClick.wav");
    soundEffect->landOnGround = Mix_LoadWAV("resources/sounds/landOnGround1.wav");
    soundEffect->punch = Mix_LoadWAV("resources/sounds/punch3.wav");
    soundEffect->knockback = Mix_LoadWAV("resources/sounds/punch2.wav");
    soundEffect->death = Mix_LoadWAV("resources/sounds/windowsGoodbye.wav");
}

void freeSoundEffects(SoundEffect *soundEffect)
{
    Mix_FreeChunk(soundEffect->jump);
    Mix_FreeChunk(soundEffect->buttonClick);
    Mix_FreeChunk(soundEffect->landOnGround);
    Mix_FreeChunk(soundEffect->punch);
    Mix_FreeChunk(soundEffect->knockback);
    Mix_FreeChunk(soundEffect->death);
}