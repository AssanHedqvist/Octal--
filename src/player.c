#include "../include/player.h"

#define JUMP_COOLDOWN 0.1f

void handlePlayerInputs(Player *player, const float dt, KeyboardStates *keyboardInputs /*, SoundEffect soundEffect*/)
{
    if (isKeyDown(keyboardInputs, SDL_SCANCODE_A))
    {
        // player->render->flip = 1;
        player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(100.0f, 0.f), dt));
        
    }

    if (isKeyDown(keyboardInputs, SDL_SCANCODE_D))
    {
        // player->render->flip = 0;
        player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(-100.0f, 0.f), dt));
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
            player->animationState = JUMP_0;

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
            }
            else
            {
                player[i].animationState = IDLE;
            }
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

void handlePlayerAnimationAlt(Player player[4]) 
{
    for (int i = 0; i < 4; i++)
    {
        switch (player[i].animationState)
        {
        case IDLE_0:
        case IDLE_1: 
        case IDLE_2: 
        case IDLE_3: 
        case IDLE_4: 
        case IDLE_5: 
        case IDLE_6: 
        case IDLE_7: 
        case IDLE_8: 
        case IDLE_9: 
        case IDLE_10: 
        case IDLE_11: 
        case IDLE_12: 
        case IDLE_13: 
        case IDLE_14: 
        case IDLE_15: 
        case IDLE_16: 
            player[i].render->imageExtents.y = 0;
            player[i].render->imageExtents.x = (player[i].animationState - IDLE_1) * 128;
            break;
        case RUN_0:
        case RUN_1: 
        case RUN_2: 
        case RUN_3: 
        case RUN_4: 
        case RUN_5: 
        case RUN_6: 
        case RUN_7: 
        case RUN_8: 
        case RUN_9: 
        case RUN_10: 
        case RUN_11: 
        case RUN_12: 
        case RUN_13: 
        case RUN_14: 
        case RUN_15:
        case RUN_16: 
            player[i].render->imageExtents.y = 256;
            player[i].render->imageExtents.x = (player[i].animationState - RUN_1) * 128;
            break;
        case JUMP_0:
        case JUMP_1: 
        case JUMP_2: 
        case JUMP_3: 
        case JUMP_4: 
        case JUMP_5: 
        case JUMP_6: 
        case JUMP_7: 
        case JUMP_8: 
        case JUMP_9: 
        case JUMP_10: 
        case JUMP_11: 
        case JUMP_12: 
        case JUMP_13: 
        case JUMP_14: 
        case JUMP_15:
        case JUMP_16: 
            player[i].render->imageExtents.y = 512;
            player[i].render->imageExtents.x = (player[i].animationState - JUMP_1) * 128;
            break;
        case PUNCH_0:
        case PUNCH_1: 
        case PUNCH_2: 
        case PUNCH_3: 
        case PUNCH_4: 
        case PUNCH_5: 
        case PUNCH_6: 
        case PUNCH_7: 
        case PUNCH_8: 
        case PUNCH_9: 
        case PUNCH_10: 
        case PUNCH_11: 
        case PUNCH_12: 
        case PUNCH_13: 
        case PUNCH_14: 
        case PUNCH_15:  
        case PUNCH_16:
            player[i].render->imageExtents.y = 768;
            player[i].render->imageExtents.x = (player[i].animationState - PUNCH_1) * 128;
            break;
        default:
            break;
        }
    }
}

void handlePlayerAnimationServerAlt(Player player[4])
{
    for (int i = 0; i < 4; i++)
    {
        switch (player[i].animationState)
        {
        case IDLE_0:
        case IDLE_1: 
        case IDLE_2: 
        case IDLE_3: 
        case IDLE_4: 
        case IDLE_5: 
        case IDLE_6: 
        case IDLE_7: 
        case IDLE_8: 
        case IDLE_9: 
        case IDLE_10: 
        case IDLE_11: 
        case IDLE_12: 
        case IDLE_13: 
        case IDLE_14: 
        case IDLE_15: 
        case IDLE_16: 
            if (!flagSet(player[i].physics->flags, DOWN))
            {
                player[i].animationState = JUMP_0;
            }
            else if(player[i].animationState == IDLE_16) {
                player[i].animationState = IDLE_0;
            }
            else if (fabsf(player[i].physics->pos.x - player[i].physics->oldPos.x) > 0.3f)
            {
                player[i].animationState = RUN_0;
            }
            break;
        case RUN_0:
        case RUN_1: 
        case RUN_2: 
        case RUN_3: 
        case RUN_4: 
        case RUN_5: 
        case RUN_6: 
        case RUN_7: 
        case RUN_8: 
        case RUN_9: 
        case RUN_10: 
        case RUN_11: 
        case RUN_12: 
        case RUN_13: 
        case RUN_14: 
        case RUN_15:
        case RUN_16: 
            if (!flagSet(player[i].physics->flags, DOWN))
            {
                player[i].animationState = JUMP_0;
            }
            else if(player[i].animationState == RUN_16) {
                player[i].animationState = RUN_0;
            }
            else if (fabs(player[i].physics->pos.x - player[i].physics->oldPos.x) < 0.3f)
            {
                player[i].animationState = IDLE_0;
            }
            break;
        case JUMP_0:
        case JUMP_1: 
        case JUMP_2: 
        case JUMP_3: 
        case JUMP_4: 
        case JUMP_5: 
        case JUMP_6: 
        case JUMP_7: 
        case JUMP_8: 
        case JUMP_9: 
        case JUMP_10: 
        case JUMP_11: 
        case JUMP_12: 
        case JUMP_13: 
        case JUMP_14: 
        case JUMP_15:
        case JUMP_16: 
            if (!flagSet(player[i].physics->flags, DOWN))
            {
                if(player[i].animationState == JUMP_16) 
                {
                    player[i].animationState = JUMP_0;
                }
            }
            else
            {
                player[i].animationState = IDLE_0;
            }
            break;
        case PUNCH_0:
        case PUNCH_1: 
        case PUNCH_2: 
        case PUNCH_3: 
        case PUNCH_4: 
        case PUNCH_5: 
        case PUNCH_6: 
        case PUNCH_7: 
        case PUNCH_8: 
        case PUNCH_9: 
        case PUNCH_10: 
        case PUNCH_11: 
        case PUNCH_12: 
        case PUNCH_13: 
        case PUNCH_14: 
        case PUNCH_15:  
        case PUNCH_16:
            if(player[i].animationState == PUNCH_16) {
                player[i].animationState = IDLE_0;
            }
            break;
        default:
            break;
        }
        player[i].animationState+=1;
    }
}
