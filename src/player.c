#include "../include/player.h"
#include "../include/spriteHandler.h"



void handlePlayerInputs(Player* player) 
{
    if (player->keyInputs.keyState[SDLK_a])
    {
        player->render->flip = 1; 
        player->physics->oldPos = vsum(player->physics->oldPos, vec2(0.5,0.0));
        if (player->render->imageExtents.x == spriteHandler(*player->render,1))
        {
            player->render->imageExtents.x = spriteHandler(*player->render,2);
        }
        else
        {
            player->render->imageExtents.x = spriteHandler(*player->render,1);
        }
    }

    if (player->keyInputs.keyState[SDLK_d])
    {
        player->render->flip = 0; 
        player->physics->oldPos = vsum(player->physics->oldPos, vec2(-0.5,0.0));
        if (player->render->imageExtents.x == spriteHandler(*player->render,1))
        {
            player->render->imageExtents.x = spriteHandler(*player->render,2);
        }
        else
        {
            player->render->imageExtents.x = spriteHandler(*player->render,1);
        }
    }

    if (player->keyInputs.keyState[SDLK_SPACE])
    {
        player->physics->oldPos = vsum(player->physics->oldPos, vec2(0.0,-0.5));
    }
}

void handlePlayerAnimation(Player* player) 
{

}