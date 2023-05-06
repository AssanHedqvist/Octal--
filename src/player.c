#include "../include/player.h"

#define totSprites 3
#define JUMP_COOLDOWN 0.1f

void handlePlayerInputs(Player *player, const float dt, KeyboardStates *keyboardInputs, SoundEffect soundEffect)
{
    if (isKeyDown(keyboardInputs, SDL_SCANCODE_A))
    {
        // player->render->flip = 1;
        player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(100.0f, 0.f), dt));
        player->animationState = RUN;
    }

    if (isKeyDown(keyboardInputs, SDL_SCANCODE_D))
    {
        // player->render->flip = 0;
        player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(-100.0f, 0.f), dt));
        player->animationState = RUN;
    }

    if (flagSet(player->physics->flags, DOWN) && player->timeSinceLastJump >= JUMP_COOLDOWN)
    {
        if (player->amountOfJumpsLeft <2)
        {
            Mix_PlayChannel(-1,soundEffect.landOnGround,0);
        }
        player->amountOfJumpsLeft = 2;
    }

    if (isKeyDown(keyboardInputs, SDL_SCANCODE_SPACE) && player->timeSinceLastJump >= JUMP_COOLDOWN)
    {

        if (player->amountOfJumpsLeft > 0)
        {
            player->animationState = JUMP;

            player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(0.f, -450.f), dt));
            Mix_PlayChannel(-1, soundEffect.jump, 0);
            // vec2 velocity = vdiff(player->physics->pos, player->physics->oldPos);

            //  may have to do it so that it only adds velocity.y when it is negative (try bouncing off a player then jumping off the platform)
            // player->physics->oldPos = vsum(vsum(player->physics->oldPos, vsmul(vec2(0.f, -396.4f), dt)), vec2(0.f, velocity.y));

            player->amountOfJumpsLeft--;
            player->timeSinceLastJump = 0.f;
        }
    }
    player->timeSinceLastJump += (1.0f / 60.f);
}

void handlePlayerLives(Player *player, SoundEffect soundEffect)
{
    if (player->physics->pos.y < 0)
    {
        Mix_PlayChannel(-1, soundEffect.death, 0);
        player->lives -= 1;
        player->physics->oldPos = vec2(384, 450);
        player->physics->pos = vec2(384, 450);
    }
}

void handlePlayerAnimation(Player *player)
{
    switch (player->animationState)
    {
    case IDLE:
        player->render->imageExtents.y = 0;
        if (player->render->imageExtents.x >= 1920) //   1920 is entire spritesheet - one sprite
        {
            player->render->imageExtents.x = 0;
        }
        else if (!flagSet(player->physics->flags, DOWN))
        {
            player->animationState = JUMP;
            player->render->imageExtents.x += player->render->imageExtents.w;
        }
        else
        {

            player->render->imageExtents.x += player->render->imageExtents.w;
        }

        break;

    case RUN:
        player->render->imageExtents.y = 256;
        if (!flagSet(player->physics->flags, DOWN))
        {
            player->animationState = JUMP;
            player->render->imageExtents.y = 512; //   not sure why this has to be set here as well.
        }
        else if (player->render->imageExtents.x >= 1920 && fabs(player->physics->pos.x - player->physics->oldPos.x) > 0.2)
        {
            player->render->imageExtents.x = 0;
        }
        else if (fabs(player->physics->pos.x - player->physics->oldPos.x) > 0.3)
        {
            player->render->imageExtents.x += player->render->imageExtents.w;
        }
        else
        {
            player->animationState = IDLE;
        }
        break;
    case JUMP:
        player->render->imageExtents.y = 512;
        if (player->render->imageExtents.x >= 1920)
        {
            player->render->imageExtents.x = 0;
        }
        else if (!flagSet(player->physics->flags, DOWN))
        {
            player->render->imageExtents.x += player->render->imageExtents.w;
        }
        else
        {
            player->animationState = IDLE;
        }
        break;

    default:
        break;
    }
}
