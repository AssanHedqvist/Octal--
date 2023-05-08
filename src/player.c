#include "../include/player.h"

#define JUMP_COOLDOWN 0.1f

void handlePlayerInputs(Player *player, const float dt, KeyboardStates *keyboardInputs /*, SoundEffect soundEffect*/)
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
        if (player->amountOfJumpsLeft < 2)
        {
            // Mix_PlayChannel(-1,soundEffect.landOnGround,0);
        }
        player->amountOfJumpsLeft = 2;
    }

    if (isKeyDown(keyboardInputs, SDL_SCANCODE_SPACE) && player->timeSinceLastJump >= JUMP_COOLDOWN)
    {

        if (player->amountOfJumpsLeft > 0)
        {
            player->animationState = JUMP;

            player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(0.f, -450.f), dt));

            // Mix_PlayChannel(-1, soundEffect.jump, 0);

            // vec2 velocity = vdiff(player->physics->pos, player->physics->oldPos);

            //  may have to do it so that it only adds velocity.y when it is negative (try bouncing off a player then jumping off the platform)
            // player->physics->oldPos = vsum(vsum(player->physics->oldPos, vsmul(vec2(0.f, -396.4f), dt)), vec2(0.f, velocity.y));

            player->amountOfJumpsLeft--;
            player->timeSinceLastJump = 0.f;
        }
    }
    player->timeSinceLastJump += (1.0f / 60.f);
}

void handlePlayerLives(Player player[4])
{
    for (int i = 0; i < 4; i++)
    {
        if (player[i].physics->pos.y < 0 || player[i].health > 100)
        {
            player[i].lives -= 1;
            player[i].health = 0;
            player[i].physics->oldPos = vec2(150.f + i * 166.6666718f, 450);
            player[i].physics->pos = vec2(150.f + i * 166.6666718f, 450);
        }

        //  this is function is a server side function now need to figure out how to play music on client side --Damien
        /*
        Mix_PlayChannel(-1, soundEffect.death, 0);
        player->lives -= 1;
        player->physics->oldPos = vec2(384, 450);
        player->physics->pos = vec2(384, 450);
        */
    }
}

void handlePlayerAnimation(Player player[4])
{
    for (int i = 0; i < 4; i++)
    {
        switch (player[i].animationState)
        {
        case IDLE:
            if (player[i].render->imageExtents.y == player[i].render->imageExtents.h * PUNCH &&
                player[i].render->imageExtents.x < 1920)
            {
                player[i].render->imageExtents.x += 128;
                player[i].animationState = PUNCH;
                printf("borde räkna upp x? %d %d\n", player[i].render->imageExtents.x, player[i].render->imageExtents.y);
            }

            else if (!flagSet(player[i].physics->flags, DOWN))
            {
                player[i].animationState = JUMP;
                player[i].render->imageExtents.x += player[i].render->imageExtents.w;
            }
            else if (player[i].render->imageExtents.y != player[i].render->imageExtents.h * IDLE)
            {
                player[i].render->imageExtents.y = player[i].render->imageExtents.h * IDLE;
                player[i].render->imageExtents.x = 0;
            }
            else if (player[i].render->imageExtents.x >= 1920) //   1920 is entire spritesheet - one sprite
            {
                player[i].render->imageExtents.x = 0;
            }
            else
            {
                player[i].render->imageExtents.x += player[i].render->imageExtents.w;
            }

            break;

        case RUN:
            if (player[i].render->imageExtents.y == player[i].render->imageExtents.h * PUNCH &&
                player[i].render->imageExtents.x < 1920)
            {
                player[i].render->imageExtents.x += player[i].render->imageExtents.w;
                printf("borde räkna upp x? %d %d\n", player[i].render->imageExtents.x, player[i].render->imageExtents.y);
            }
            else if (player[i].render->imageExtents.y != player[i].render->imageExtents.h * RUN)
            {
                player[i].render->imageExtents.y = player[i].render->imageExtents.h * RUN;
                player[i].render->imageExtents.x = 0;
            }
            if (!flagSet(player[i].physics->flags, DOWN))
            {
                player[i].animationState = JUMP;
                player[i].render->imageExtents.y = 512; //   not sure why this has to be set here as well.
            }
            else if (player[i].render->imageExtents.x >= 1920 && fabs(player[i].physics->pos.x - player[i].physics->oldPos.x) > 0.3f)
            {
                player[i].render->imageExtents.x = 0;
            }
            else if (fabs(player[i].physics->pos.x - player[i].physics->oldPos.x) > 0.3f)
            {
                player[i].render->imageExtents.x += player[i].render->imageExtents.w;
            }
            else
            {
                player[i].animationState = IDLE;
            }
            break;
        case JUMP:
            if (player[i].render->imageExtents.y == player[i].render->imageExtents.h * PUNCH &&
                player[i].render->imageExtents.x < 1920)
            {
                printf("%d %d\n", player[i].render->imageExtents.x, player[i].render->imageExtents.y);
                player[i].render->imageExtents.x += 128;
            }
            else if (player[i].render->imageExtents.y != player[i].render->imageExtents.h * JUMP)
            {
                player[i].render->imageExtents.y = player[i].render->imageExtents.h * JUMP;
                player[i].render->imageExtents.x = 0;
            }
            if (player[i].render->imageExtents.x >= 1920)
            {
                player[i].render->imageExtents.x = 0;
            }
            else if (!flagSet(player[i].physics->flags, DOWN))
            {
                player[i].render->imageExtents.x += player[i].render->imageExtents.w;
            }
            else
            {
                player[i].animationState = IDLE;
            }
            break;
        case PUNCH:
            if (player[i].render->imageExtents.h != player[i].render->imageExtents.h * PUNCH)
            {
                player[i].render->imageExtents.y = player[i].render->imageExtents.h * PUNCH;
                player[i].render->imageExtents.x = 0;
                //  player[i].animationState = IDLE;
            }
            else
            {
                printf("in the ELSE\n");
                player[i].animationState = IDLE;
            }
            printf("%d %d\n", player[i].render->imageExtents.x, player[i].render->imageExtents.y);
            // printf("animstate %d\n", player[i].animationState);

        default:
            break;
        }
    }
}

void handlePlayerAnimationServer(Player player[4])
{
    for (int i = 0; i < 4; i++)
    {
        switch (player[i].animationState)
        {
        case IDLE:
            if (!flagSet(player[i].physics->flags, DOWN))
            {
                player[i].animationState = JUMP;
            }
            break;
        case RUN:
            if (!flagSet(player[i].physics->flags, DOWN))
            {
                player[i].animationState = JUMP;
            }
            if (fabs(player[i].physics->pos.x - player[i].physics->oldPos.x) > 0.3f)
            {
                player[i].animationState = RUN;
            }
            else
            {
                player[i].animationState = IDLE;
            }
            break;
        case JUMP:
            if (!flagSet(player[i].physics->flags, DOWN))
            {
                player[i].animationState = JUMP;
            }
            else
            {
                player[i].animationState = IDLE;
            }
            break;
        case PUNCH:
            player[i].animationState = PUNCH;

        default:
            break;
        }
    }
}
