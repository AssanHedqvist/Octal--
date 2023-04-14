#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <../include/renderObject.h>
#include <../include/keyboard.h>

typedef struct Player
{
    RenderObject* object;
    KeyboardStates keyInputs;
    
    //  add more
} Player;

void handlePlayerKeyInputs(Player* player);

#endif