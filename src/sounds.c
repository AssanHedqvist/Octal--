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