#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../include/renderObject.h"
#include "../include/physicsObject.h"
#include "../include/keyboard.h"


typedef struct 
{
    RenderObject* render;
    PhysicsObject* physics;
    int amountOfJumpsLeft;
    int lives;
    float timeSinceLastJump;
    int health;
    //  add more
} Player;

void handlePlayerInputs(Player* player, float dt, KeyboardStates* keyboardInputs);

void handlePlayerAnimation(Player* player);

void handlePlayerLives(Player* player);

void boundarySolve(Player players[4]);

#endif