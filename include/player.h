#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../include/renderObject.h"
#include "../include/physicsObject.h"
#include "../include/keyboard.h"
#include "../include/sounds.h"


typedef struct
{
    RenderObject *render;
    PhysicsObject *physics;
    unsigned char amountOfJumpsLeft;
    unsigned short health;
    unsigned char lives;
    unsigned char recentlyHit;
    unsigned char animationState;

    float timeSinceLastJump;
    float timeSinceLastPunch;
    float timeSinceHit;
    //  add more
} Player;

//   numbers represent rows in spritesheet
typedef enum {
    IDLE = 0,
    RUN = 1,
    JUMP = 2
} animationState;




void handlePlayerInputs(Player *player, float dt, KeyboardStates *keyboardInputs, SoundEffect soundEffect);

void handlePlayerAnimation(Player *player);

void handlePlayerLives(Player *player, SoundEffect soundEffect);

#endif