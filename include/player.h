#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../include/renderObject.h"
#include "../include/physicsObject.h"
#include "../include/keyboard.h"


typedef struct 
{
    RenderObject* render;
    PhysicsObject* physics;
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

void handlePlayerInputs(Player* player, float dt, KeyboardStates* keyboardInputs);

void handlePlayerAnimation(Player* player);

void handlePlayerLives(Player* player);

#endif