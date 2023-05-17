#include "../include/player.h"

#define JUMP_COOLDOWN 0.1f
#define DT (1.0/240.0f)
#define TOTAL_TIME_PASSED (1.0/60.0f)

void initPlayers(Player players[]) 
{
    players[0].health = 0;
    players[0].lives = 4;
    players[0].animationState = IDLE_0;
    players[0].amountOfJumpsLeft = 2;
    players[0].timeSinceHit = 0.f;
    players[0].timeSinceLastJump = 0.f;
    players[0].timeSinceLastPunch = 0.f;

    players[1].health = 0;
    players[1].lives = 4;
    players[1].animationState = IDLE_0;
    players[1].amountOfJumpsLeft = 2;
    players[1].timeSinceHit = 0.f;
    players[1].timeSinceLastJump = 0.f;
    players[1].timeSinceLastPunch = 0.f;

    players[2].health = 0;
    players[2].lives = 4;
    players[2].animationState = IDLE_0;
    players[2].amountOfJumpsLeft = 2;
    players[2].timeSinceHit = 0.f;
    players[2].timeSinceLastJump = 0.f;
    players[2].timeSinceLastPunch = 0.f;

    players[3].health = 0;
    players[3].lives = 4;
    players[3].animationState = IDLE_0;
    players[3].amountOfJumpsLeft = 2;
    players[3].timeSinceHit = 0.f;
    players[3].timeSinceLastJump = 0.f;
    players[3].timeSinceLastPunch = 0.f;
}

void handlePlayerInputsServer(Player player[4], KeyboardStates keyboardInputs[4], unsigned char playerFlip[4])
{
    for (int i = 0; i < 4; i++)
	{
        if(flagPhysicsGet(player[i].physics->flags, PHYSICS_ACTIVE)) 
        {
            if (flagPhysicsGet(player[i].physics->flags, DOWN) && player[i].timeSinceLastJump >= JUMP_COOLDOWN)
            {
                player[i].amountOfJumpsLeft = 2;
            }

            if (getKeyboardKey(&keyboardInputs[i], SDL_SCANCODE_B))
            {
                if(player[i].animationState < BLOCK_0 || BLOCK_16 < player[i].animationState) 
                {
                    player[i].animationState = BLOCK_0;
                }
            }
            else {

                if(BLOCK_0 <= player[i].animationState && player[i].animationState <= BLOCK_16) 
                {
                    player[i].animationState = IDLE_0;
                }

                if (getKeyboardKey(&keyboardInputs[i], SDL_SCANCODE_A))
                {
                    player[i].physics->oldPos = vsum(player[i].physics->oldPos, vsmul(vec2(75.0f, 0.f), DT));
                    playerFlip[i] = 1;
                }

                if (getKeyboardKey(&keyboardInputs[i], SDL_SCANCODE_D))
                {
                    player[i].physics->oldPos = vsum(player[i].physics->oldPos, vsmul(vec2(-75.0f, 0.f), DT));
                    playerFlip[i] = 0;
                }

                if (getKeyboardKey(&keyboardInputs[i], SDL_SCANCODE_SPACE) && player[i].timeSinceLastJump >= JUMP_COOLDOWN)
                {

                    if (player[i].amountOfJumpsLeft > 0)
                    {
                        player[i].animationState = JUMP_0;

                        player[i].physics->oldPos.y = player[i].physics->pos.y - 300.f * DT; 

                        player[i].amountOfJumpsLeft--;
                        player[i].timeSinceLastJump = 0.f;
                    }
                }
            }

            player[i].timeSinceLastJump += TOTAL_TIME_PASSED;
        }
       
    }
}

void handlePlayerInputsClient(Player *player, KeyboardStates *keyboardInputs, SoundEffect soundEffect)
{
    if(flagPhysicsGet(player->physics->flags,PHYSICS_ACTIVE)) 
    {
        if (flagPhysicsGet(player->physics->flags, DOWN) && player->timeSinceLastJump >= JUMP_COOLDOWN)
        {
            if (player->amountOfJumpsLeft < 2)
            {
                Mix_PlayChannel(-1,soundEffect.landOnGround,0);
            }
            player->amountOfJumpsLeft = 2;
        }
    
        if (getKeyboardKey(keyboardInputs, SDL_SCANCODE_B))
        {
            if(player->animationState < BLOCK_0 || BLOCK_16 < player->animationState) 
            {
                player->animationState = BLOCK_0;
            }
            // should we add this??
            // Mix_PlayChannel(-1,soundEffect.block,0);
        }
        else {
             
            if(BLOCK_0 <= player->animationState && player->animationState <= BLOCK_16) 
            {
                player->animationState = IDLE_0;
            }
    
            if (getKeyboardKey(keyboardInputs, SDL_SCANCODE_A))
            {
                player->render->flags |= FLIP;
                player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(100.0f, 0.f), DT));  
            }
    
            if (getKeyboardKey(keyboardInputs, SDL_SCANCODE_D))
            {
                player->render->flags &= ~FLIP;
                player->physics->oldPos = vsum(player->physics->oldPos, vsmul(vec2(-100.0f, 0.f), DT));
            }
    
            if (getKeyboardKey(keyboardInputs, SDL_SCANCODE_SPACE) && player->timeSinceLastJump >= JUMP_COOLDOWN)
            {
            
                if (player->amountOfJumpsLeft > 0)
                {
                    player->animationState = JUMP_0;
    
                    player->physics->oldPos.y = player->physics->pos.y - 450.f * DT; 
    
                    Mix_PlayChannel(-1, soundEffect.jump, 0);
    
                    player->amountOfJumpsLeft--;
                    player->timeSinceLastJump = 0.f;
                }
            }
        }
    
        player->timeSinceLastBlock += TOTAL_TIME_PASSED;
        player->timeSinceLastJump += TOTAL_TIME_PASSED;
    }
}

void handlePlayerLivesServer(Player player[4])
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
    }
}

void handlePlayerLivesClient(Player player[4], SoundEffect soundEffect)
{
    for (int i = 0; i < 4; i++)
    {
        if (player[i].physics->pos.y < 0)
        {
            Mix_PlayChannel(-1, soundEffect.death, 0);
            player[i].lives -= 1;
            player[i].health = 0; 
            player[i].physics->oldPos = vec2(150.f + i * 166.6666718f, 450);
            player[i].physics->pos = vec2(150.f + i * 166.6666718f, 450);
        }
    }
}

void handlePlayerAnimationServer(Player player[4])
{
    for (int i = 0; i < 4; i++)
    {
        switch (player[i].animationState)
        {
        case IDLE_0 ... IDLE_16: 
            if (!flagPhysicsGet(player[i].physics->flags, DOWN))
            {
                player[i].animationState = JUMP_0;
            }
            else if(player[i].animationState == IDLE_16) 
            {
                player[i].animationState = IDLE_0;
            }
            else if (fabsf(player[i].physics->pos.x - player[i].physics->oldPos.x) > 0.3f)
            {
                player[i].animationState = RUN_0;
            }
            break;
        case RUN_0 ... RUN_16: 
            if (!flagPhysicsGet(player[i].physics->flags, DOWN))
            {
                player[i].animationState = JUMP_0;
            }
            else if(player[i].animationState == RUN_16) 
            {
                player[i].animationState = RUN_0;
            }
            else if (fabs(player[i].physics->pos.x - player[i].physics->oldPos.x) < 0.3f)
            {
                player[i].animationState = IDLE_0;
            }
            break;
        case JUMP_0 ... JUMP_16:
            if (!flagPhysicsGet(player[i].physics->flags, DOWN))
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
            if(player[i].animationState == PUNCH_16) 
            {
                player[i].animationState = IDLE_0;
            }
            break;
        case BLOCK_0 ... BLOCK_16:
            if(player[i].animationState == BLOCK_16) 
            {
                player[i].animationState = BLOCK_15;
            }
            break;
        default:
            break;
        }
        player[i].animationState+=1;
    }
}

void handlePlayerAnimationClient(Player player[4])
{
    for (int i = 0; i < 4; i++)
    {
        switch (player[i].animationState)
        {
        case IDLE_0 ... IDLE_16:
            if (!flagPhysicsGet(player[i].physics->flags, DOWN))
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
            if (!flagPhysicsGet(player[i].physics->flags, DOWN))
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
            if (!flagPhysicsGet(player[i].physics->flags, DOWN))
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
        case BLOCK_0 ... BLOCK_16:
            if(player[i].animationState == BLOCK_16) {
                player[i].animationState = BLOCK_15;
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
        //  Don't ask XD --Damien
        player[i].render->imageExtents.y = ((player[i].animationState / 17) << 8);
        player[i].render->imageExtents.x = (((player[i].animationState % 17) - 1) << 7);
    }
}