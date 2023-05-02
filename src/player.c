#include "../include/player.h"

#define totSprites 3
#define JUMP_COOLDOWN 0.1f

void handlePlayerInputs(Player *player, const float dt, KeyboardStates* keyboardInputs)
{
    if (isKeyDown(keyboardInputs, SDL_SCANCODE_A))
    {
        player->render->flip = 1;
        player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(100.0f, 0.f), dt));
    }

    if (isKeyDown(keyboardInputs, SDL_SCANCODE_D))
    {
        player->render->flip = 0;
        player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(-100.0f, 0.f), dt));
    }

    if (player->physics->recentCollision == 1 && player->timeSinceLastJump >= JUMP_COOLDOWN)
    {
        player->amountOfJumpsLeft = 2;
    }
    
    if (isKeyDown(keyboardInputs, SDL_SCANCODE_SPACE) && player->timeSinceLastJump >= JUMP_COOLDOWN)
    {
       
        if (player->amountOfJumpsLeft > 0)
        {

            player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(0.f, -450.f), dt));
            //vec2 velocity = vdiff(player->physics->pos, player->physics->oldPos);

            //  may have to do it so that it only adds velocity.y when it is negative (try bouncing off a player then jumping off the platform)
            //player->physics->oldPos = vsum(vsum(player->physics->oldPos, vsmul(vec2(0.f, -396.4f), dt)), vec2(0.f, velocity.y));

            player->amountOfJumpsLeft--;
            player->timeSinceLastJump = 0.f;
        }
    }
    player->timeSinceLastJump += (1.0f/60.f);
}

void handlePlayerLives(Player *player)
{
    if (player->physics->pos.y < 0)
    {
        player->lives -= 1;
    }
}

void handlePlayerAnimation(Player *player)
{
    if (fabs(player->physics->pos.x - player->physics->oldPos.x) > 0.5)
    {
        if (player->render->imageExtents.x == player->render->imageExtents.w * (totSprites - 1))
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

void boundarySolve(Player players[4]) {
    for (int i = 0; i < 4; i++)
    {
        // collision detection with windows boundries
        if (players[i].physics->pos.x <= 0.f)
        {
            // if you collide reset the position so u dont go out of the screen
            players[i].physics->pos.x = 0.f;
        }
        if (players[i].physics->pos.y <= 0.f)
        {
            players[i].physics->pos.y = 0.f;
        }
        // subtract the width of the sprite.
        if (players[i].physics->pos.x + players[i].physics->extents.x >= 800.0f)
        {
            players[i].physics->pos.x = 800.0f - players[i].physics->extents.x;
        }
        // subtract the height of the sprite.
        if (players[i].physics->pos.y + players[i].physics->extents.y >= 600.0f)
        {
            players[i].physics->pos.y = 600.0f - players[i].physics->extents.y;
        }
    }
}