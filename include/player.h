#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../include/renderObject.h"
#include "../include/physicsObject.h"
#include "../include/keyboard.h"


typedef struct 
{
    RenderObject* render;
    PhysicsObject* physics;
    KeyboardStatesAlt keyInputs;
    int amountOfJumpsLeft;
    //  add more
} Player;

void handlePlayerInputs(Player* player, float dt);

void handlePlayerAnimation(Player* player);

#endif