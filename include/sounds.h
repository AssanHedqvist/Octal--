#ifndef SOUNDS_H_INCLUDED
#define SOUNDS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

void togglePlay();

typedef struct 
{
    Mix_Chunk *jump;
    Mix_Chunk *knockback;
    Mix_Chunk *punch;
    Mix_Chunk *landOnGround;
    Mix_Chunk *pickUpWeapon;
    Mix_Chunk *dropWeapon;
    Mix_Chunk *run;
    Mix_Chunk *death;
    Mix_Chunk *buttonClick;
} SoundEffect;

void loadSoundEffects(SoundEffect *sounds);

void freeSoundEffects(SoundEffect *soundEffect);

#endif
