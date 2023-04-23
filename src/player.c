#include "../include/player.h"
#include "../include/spriteHandler.h"

#define totSprites 3

void handlePlayerInputs(Player *player)
{
    if (player->keyInputs.keyState[SDLK_a])
    {
        player->render->flip = 1;
        player->physics->oldPos = vsum(player->physics->oldPos, vec2(0.5, 0.0));
        handlePlayerAnimation(player);
    }

    if (player->keyInputs.keyState[SDLK_d])
    {
        player->render->flip = 0;
        player->physics->oldPos = vsum(player->physics->oldPos, vec2(-0.5, 0.0));
        handlePlayerAnimation(player);
    }

    if (player->keyInputs.keyState[SDLK_SPACE])
    {
        player->physics->oldPos = vsum(player->physics->oldPos, vec2(0.0, -0.5));
    }
}

void handlePlayerAnimation(Player *player)
{
    if (player->render->imageExtents.x == player->render->imageExtents.w * (totSprites-1))  //   -1 to skip idle frame.
    {
        player->render->imageExtents.x = player->render->imageExtents.w;
    }
    else
    {
        player->render->imageExtents.x += player->render->imageExtents.w;
    }
}