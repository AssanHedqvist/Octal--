#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../include/renderObject.h"
#include "../include/physicsObject.h"
#include "../include/keyboard.h"


typedef struct 
{
    RenderObject* render;
    PhysicsObject* physics;
    KeyboardStates keyInputs;
    int amountOfJumpsLeft;
    int lives;
    float timeSinceLastJump;
    int health;
    float timeSinceLastPunch;
    //  add more
} Player;

void handlePlayerInputs(Player* player, float dt);

void handlePlayerAnimation(Player* player);

void handlePlayerLives(Player* player);

#endif