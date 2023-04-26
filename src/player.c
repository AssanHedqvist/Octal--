#include "../include/player.h"
#include "../include/spriteHandler.h"

#define totSprites 3

void handlePlayerInputs(Player *player, const float dt)
{
    if (isKeyDown(&player->keyInputs,SDL_SCANCODE_A))
    {
        player->render->flip = 1;
        player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(100.0f, 0.f),dt));
    }

    if (isKeyDown(&player->keyInputs,SDL_SCANCODE_D))
    {
        player->render->flip = 0;
        player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(-100.0f, 0.f),dt));
    }

    if (isKeyDown(&player->keyInputs,SDL_SCANCODE_SPACE))
    {
        if(player->physics->recentCollision == 1) 
        {
            player->amountOfJumpsLeft = 2;
        }
        if (player->amountOfJumpsLeft > 0) 
        {
            player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(0.f, -196.4f),dt));
            player->amountOfJumpsLeft--;
        }
       
    }
}

void handlePlayerAnimation(Player *player)
{
    if (fabs(player->physics->pos.x - player->physics->oldPos.x) > 0.5)
    {
        if (player->render->imageExtents.x == player->render->imageExtents.w * (totSprites-1))
        {
            player->render->imageExtents.x = player->render->imageExtents.w;
        }
        else
        {
            player->render->imageExtents.x += player->render->screenExtents.w;
        }
    }
    else
    {
        player->render->imageExtents.x = 0;
    }
}