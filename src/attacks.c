#include "../include/attacks.h"
#include "../include/keyboard.h"
#include "../include/player.h"
#include <math.h>

#define PUNCH_COOLDOWN 0.13333333333333333333333333333333f

//  server shall handle this function later when that is done change keyboardStates to a array instead of pointer
void lightPunch(Player players[], int amountOfPlayers, KeyboardStates *keyboardInputs, SoundEffect soundEffect)
{
    if (isKeyDown(keyboardInputs, SDL_SCANCODE_J) && players[0].timeSinceLastPunch >= PUNCH_COOLDOWN)
    {
        players[0].animationState = PUNCH;
        Mix_PlayChannel(-1, soundEffect.punch,0);
        for (int i = 1; i < 4; i++)
        {
            //   I'll keep those here to clarify the math for myself. Will delete in a week or so.

            // int yDist = players[0].physics->pos.y - players[i].physics->pos.y;
            // int xDist = players[0].physics->pos.x - players[i].physics->pos.x;
            // int playerWidth = players->render->screenExtents.w;
            // int playerHeight = players->render->screenExtents.h;

            //   check if player to the right
            if ((players[0].physics->pos.x - players[i].physics->pos.x) +
                        (players->render->screenExtents.w * 0.2) >=
                    -(players->render->screenExtents.w) &&
                (players[0].physics->pos.x - players[i].physics->pos.x) <= 0 &&
                (players[0].physics->pos.y - players[i].physics->pos.y) <= (players->render->screenExtents.h / 2) &&
                (players[0].physics->pos.y - players[i].physics->pos.y) >= -(players->render->screenExtents.h / 2) &&
                players[0].render->flip == 0)
            {
                players[i].health += 10;
                players[i].recentlyHit++;
                if (players[i].timeSinceHit < 0.5)
                {
                    if (players[i].recentlyHit >= 3)
                    {
                        players[i].physics->oldPos.x -= pow(1.03,players[i].health);
                    }
                }
                else
                {
                    players[i].recentlyHit = 0;
                }
                players[i].timeSinceHit = 0.0f;
            }

            //   Check if player to the left
            else if ((players[0].physics->pos.x - players[i].physics->pos.x) -
                             (players->render->screenExtents.w * 0.2) <=
                         (players->render->screenExtents.w) &&
                     (players[0].physics->pos.x - players[i].physics->pos.x) >= 0 &&
                     (players[0].physics->pos.y - players[i].physics->pos.y) <= (players->render->screenExtents.h / 2) &&
                     (players[0].physics->pos.y - players[i].physics->pos.y) >= -(players->render->screenExtents.h / 2) &&
                     players[0].render->flip == 1)
            {
                players[i].health += 10;
                players[i].recentlyHit++;
                if (players[i].timeSinceHit < 0.5)
                {
                    if (players[i].recentlyHit >= 3)
                    {
                        players[i].physics->oldPos.x += pow(1.03,players[i].health);
                    }
                }
                else
                {
                    players[i].recentlyHit = 0;
                }
                players[i].timeSinceHit = 0.0f;
            }
        }
        players->render->imageExtents.x = 96;
        players[0].timeSinceLastPunch = 0.0f;
    }
    for (int i = 0; i < 4; i++)
    {
        // printf("player %d time: %f hitting: %d\n", i, players[i].timeSinceHit, players[i].recentlyHit);
        players[i].timeSinceHit += 1.0f / (60.0f);
    }
    players[0].timeSinceLastPunch += 1.0f / (60.0f);
}

void lightPunchServer(Player players[4], unsigned char playerFlip[4], KeyboardStates keyboardInputs[4]) 
{
    vec2 minCorner1;
    vec2 maxCorner1;
    vec2 minCorner2;
    vec2 maxCorner2;
    float sign = 1.f;
    for (int i = 0; i < 4; i++)
    {
        if (isKeyDown(&keyboardInputs[i], SDL_SCANCODE_J) && players[i].timeSinceLastPunch >= PUNCH_COOLDOWN) 
        {   
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
                if(j != i) 
                {
                    minCorner2 = players[j].physics->pos;
                    maxCorner2 = vsum(players[j].physics->pos,players[j].physics->extents);

                    if(minCorner1.x <= maxCorner2.x &&
                       minCorner1.y <= maxCorner2.y &&
                       minCorner2.x <= maxCorner1.x &&
                       minCorner2.y <= maxCorner1.y ) 
                    {
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
            players[i].timeSinceLastPunch = 0.0f;
        }
        players[i].timeSinceLastPunch += 1.0f / (60.0f);
    }
    for (int i = 0; i < 4; i++)
    {
        players[i].timeSinceHit += 1.0f / (60.0f);
    }
}

  
