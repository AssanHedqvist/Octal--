#include "../include/attacks.h"
#include "../include/keyboard.h"
#include "../include/player.h"
#include <math.h>

#define PUNCH_COOLDOWN 0.26833333333333333333333333333333f


void lightPunchServer(Player players[4], unsigned char playerFlip[4], KeyboardStates keyboardInputs[4]) 
{
    vec2 minCorner1;
    vec2 maxCorner1;
    vec2 minCorner2;
    vec2 maxCorner2;
    float sign = 1.f;
    for (int i = 0; i < 4; i++)
    {
        if (getKeyboardKey(&keyboardInputs[i], SDL_SCANCODE_J) && 
            !getKeyboardKey(&keyboardInputs[i], SDL_SCANCODE_B) &&
            players[i].timeSinceLastPunch >= PUNCH_COOLDOWN &&
            flagPhysicsGet(players[i].physics->flags, PHYSICS_ACTIVE)
            ) 
        {   
            players[i].timeSinceLastPunch = 0.0f;
            players[i].animationState = PUNCH_0;
            

            if(playerFlip[i] == 1) {
                minCorner1 = vsum(players[i].physics->pos, vec2(-players[i].physics->extents.x,0.f));
                maxCorner1 = vsum(players[i].physics->pos, vec2(0.f,players[i].physics->extents.y));
                sign = 1.0f;
            }
            else 
            {
                minCorner1 = vsum(players[i].physics->pos, vec2(players[i].physics->extents.x,0.f));
                maxCorner1 = vsum(players[i].physics->pos, vec2(players[i].physics->extents.x*2.0f,players[i].physics->extents.y));
                sign = -1.0f;
            }


            for (int j = 0; j < 4; j++)
            {
                if(j != i && flagPhysicsGet(players[j].physics->flags, PHYSICS_ACTIVE)) 
                {
                    minCorner2 = players[j].physics->pos;
                    maxCorner2 = vsum(players[j].physics->pos,players[j].physics->extents);

                    if(minCorner1.x <= maxCorner2.x &&
                       minCorner1.y <= maxCorner2.y &&
                       minCorner2.x <= maxCorner1.x &&
                       minCorner2.y <= maxCorner1.y) 
                    {
                        if(players[j].animationState >= BLOCK_10 && players[j].animationState <= BLOCK_16) 
                        {
                            players[i].timeSinceLastPunch = -PUNCH_COOLDOWN;
                        }
                        else {
                            players[j].health += 10;
                            players[j].recentlyHit++;
                            if (players[j].timeSinceHit < 0.5f)
                            {
                                if (players[j].recentlyHit >= 3)
                                {
                                    players[j].recentlyHit = 0;
                                    players[j].physics->oldPos.x += sign * pow(200.2f,players[i].health);
                                }
                            }
                            else
                            {
                                players[j].recentlyHit = 0;
                            }
                            players[j].timeSinceHit = 0.0f;  
                        }
                    }
                }
            }
            
        }
        players[i].timeSinceLastPunch += 1.0f / (60.0f);
    }
    for (int i = 0; i < 4; i++)
    {
        players[i].timeSinceHit += 1.0f / (60.0f);
    }
}

void lightPunchClient(Player players[4], KeyboardStates* keyboardInputs, unsigned char clientIndex, SoundEffect soundEffect) 
{
    vec2 minCorner1;
    vec2 maxCorner1;
    vec2 minCorner2;
    vec2 maxCorner2;
    float sign = 1.f;
    
    if (getKeyboardKey(keyboardInputs, SDL_SCANCODE_J) &&  
       !getKeyboardKey(keyboardInputs, SDL_SCANCODE_B) &&
       players[clientIndex].timeSinceLastPunch >= PUNCH_COOLDOWN &&
       flagPhysicsGet(players[clientIndex].physics->flags, PHYSICS_ACTIVE)
       ) 
    {   
        players[clientIndex].timeSinceLastPunch = 0.0f;
        players[clientIndex].animationState = PUNCH_0;

        if(flagRenderGet(players[clientIndex].render->flags, FLIP) == 1) {
            minCorner1 = vsum(players[clientIndex].physics->pos, vec2(-players[clientIndex].physics->extents.x,0.f));
            maxCorner1 = vsum(players[clientIndex].physics->pos, vec2(0.f,players[clientIndex].physics->extents.y));
            sign = 1.0f;
        }
        else 
        {
            minCorner1 = vsum(players[clientIndex].physics->pos, vec2(players[clientIndex].physics->extents.x,0.f));
            maxCorner1 = vsum(players[clientIndex].physics->pos, vec2(players[clientIndex].physics->extents.x*2.0f,players[clientIndex].physics->extents.y));
            sign = -1.0f;
        }
        for (int j = 0; j < 4; j++)
        {
            if(j != clientIndex && flagPhysicsGet(players[j].physics->flags, PHYSICS_ACTIVE)) 
            {
                minCorner2 = players[j].physics->pos;
                maxCorner2 = vsum(players[j].physics->pos,players[j].physics->extents);
                
                if(minCorner1.x <= maxCorner2.x &&
                   minCorner1.y <= maxCorner2.y &&
                   minCorner2.x <= maxCorner1.x &&
                   minCorner2.y <= maxCorner1.y) 
                {
                    if(players[j].animationState >= BLOCK_10 && players[j].animationState <= BLOCK_16) 
                    {
                        players[clientIndex].timeSinceLastPunch = -PUNCH_COOLDOWN;
                    }
                    else {
                        players[j].health += 10;
                        if(players[j].health > 100) 
                        {
                            Mix_PlayChannel(-1, soundEffect.death, 0);
                            players[j].lives -= 1;
                            players[j].health = 0; 
                            players[j].physics->oldPos = vec2(150.f + j * 166.6666718f, 450);
                            players[j].physics->pos = vec2(150.f + j * 166.6666718f, 450);
                        }
                        players[j].recentlyHit++;
                        if (players[j].timeSinceHit < 0.5f)
                        {
                            if (players[j].recentlyHit >= 3)
                            {
                                players[j].recentlyHit = 0;
                                players[j].physics->oldPos.x += sign * pow(200.2f,players[clientIndex].health);
                            }
                        }
                        else
                        {
                            players[j].recentlyHit = 0;
                        }
                        players[j].timeSinceHit = 0.0f;      
                    }
                }
            }
        }
        Mix_PlayChannel(-1, soundEffect.punch,0);
    }
    players[clientIndex].timeSinceLastPunch += 1.0f / (60.0f);
    for (int i = 0; i < 4; i++)
    {
        players[i].timeSinceHit += 1.0f / (60.0f);
    }
}
