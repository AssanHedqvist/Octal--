#include "../include/player.h"

void initPlayers(int amountOfPlayers, Player *players, RenderObject *renderObject, PhysicsObject *physicsObject, float posX, float posY)
{
    int spawnSpace = 64;
    for(int i = 0; i < amountOfPlayers; i++)
    {
        players[i].render = &renderObject[i+2];
        players[i].physics = &physicsObject[i];
        players[i].physics->position = vec2(posX+ i*(64), posY);
        players[i].amountOfJumpsLeft = 2;
    }
}
