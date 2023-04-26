#include "../include/player.h"
#include "../include/spriteHandler.h"

#define totSprites 3
int latestJumpFromPlattform; // den här behövs för att annars kan man hoppa 3 gånger i rad
static float timeSinceLastJump = 0.f;
const float jumpCooldown = 0.1f;

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

    if(player->physics->recentCollision == 1) 
    {
        if (latestJumpFromPlattform ==1) player->amountOfJumpsLeft = 1;
        else player->amountOfJumpsLeft = 2;
    }

if (isKeyDown(&player->keyInputs,SDL_SCANCODE_SPACE) && timeSinceLastJump >= jumpCooldown)
{
    if (player->amountOfJumpsLeft > 0) 
    {
        vec2 velocity = vdiff(player->physics->pos, player->physics->oldPos);
        player->physics->oldPos = vsum(vsum(player->physics->oldPos, vsmul(vec2(0.f, -396.4f),dt)), vec2(0.f, velocity.y));
        if (player->physics->recentCollision == 1) latestJumpFromPlattform =1;
        else latestJumpFromPlattform =0;
        player->amountOfJumpsLeft--;
        timeSinceLastJump = 0.f;
    }
}

timeSinceLastJump += dt;
       
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