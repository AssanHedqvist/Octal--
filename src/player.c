#include "../include/player.h"

#define JUMP_COOLDOWN 0.1f

void handlePlayerInputsServer(Player *player, const float dt, KeyboardStates *keyboardInputs /*, SoundEffect soundEffect*/)
{
    if (getKeyboardKey(keyboardInputs, SDL_SCANCODE_A))
    {
        // player->render->flip = 1;
        player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(100.0f, 0.f), dt));
        
    }

    if (getKeyboardKey(keyboardInputs, SDL_SCANCODE_D))
    {
        // player->render->flip = 0;
        player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(-100.0f, 0.f), dt));
    }

    if (flagGet(player->physics->flags, DOWN) && player->timeSinceLastJump >= JUMP_COOLDOWN)
    {
        if (player->amountOfJumpsLeft < 2)
        {
            // Mix_PlayChannel(-1,soundEffect.landOnGround,0);
        }
        player->amountOfJumpsLeft = 2;
    }

    if (getKeyboardKey(keyboardInputs, SDL_SCANCODE_SPACE) && player->timeSinceLastJump >= JUMP_COOLDOWN)
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


void handlePlayerAnimationClient(Player player[4], unsigned char thisComputersPlayerIndex)
{
    for (int i = 0; i < 4; i++)
    {
        switch (player[i].animationState)
        {
        case IDLE_0 ... IDLE_16:
            if (!flagGet(player[i].physics->flags, DOWN))
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
        case RUN_0 ... RUN_16:
            if (!flagGet(player[i].physics->flags, DOWN))
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
        case JUMP_0 ... JUMP_16:
            if (!flagGet(player[i].physics->flags, DOWN))
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
        case PUNCH_0 ... PUNCH_16:
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

void handlePlayerAnimationServer(Player player[4])
{
    for (int i = 0; i < 4; i++)
    {
        switch (player[i].animationState)
        {
        case IDLE_0 ... IDLE_16: 
            if (!flagGet(player[i].physics->flags, DOWN))
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
        case RUN_0 ... RUN_16: 
            if (!flagGet(player[i].physics->flags, DOWN))
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
        case JUMP_0 ... JUMP_16:
            if (!flagGet(player[i].physics->flags, DOWN))
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
        case PUNCH_0 ... PUNCH_16:
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

void updatePlayerRenderWithAnimation(Player player[4]) 
{
    for (int i = 0; i < 4; i++)
    {
        switch (player[i].animationState)
        {
        case IDLE_0 ... IDLE_16: 
            player[i].render->imageExtents.y = 0;
            player[i].render->imageExtents.x = (player[i].animationState - IDLE_1) * 128;
            break;
        case RUN_0 ... RUN_16:  
            player[i].render->imageExtents.y = 256;
            player[i].render->imageExtents.x = (player[i].animationState - RUN_1) * 128;
            break;
        case JUMP_0 ... JUMP_16:
            player[i].render->imageExtents.y = 512;
            player[i].render->imageExtents.x = (player[i].animationState - JUMP_1) * 128;
            break;
        case PUNCH_0 ... PUNCH_16:
            player[i].render->imageExtents.y = 768;
            player[i].render->imageExtents.x = (player[i].animationState - PUNCH_1) * 128;
            break;
        default:
            break;
        }
    }
}