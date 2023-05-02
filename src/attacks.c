#include "../include/attacks.h"
#include "../include/keyboard.h"
#include "../include/player.h"

//  server shall handle this function later when that is done change keyboardStates to a array instead of pointer 
void lightPunch(Player players[], int amountOfPlayers, KeyboardStates*  keyboardInputs)
{
    if (isKeyDown(keyboardInputs, SDL_SCANCODE_J))
    {
        for (int i = 1; i < 4; i++)
        {
            //   I'll keep those here to clarify the math for myself. Will delete in a week or so.
            //int yDist = players[0].physics->pos.y - players[i].physics->pos.y;
            //int xDist = players[0].physics->pos.x - players[i].physics->pos.x;
            //int playerWidth = players->render->screenExtents.w;
            //int playerHeight = players->render->screenExtents.h;

            if ((players[0].physics->pos.x - players[i].physics->pos.x) + (players->render->screenExtents.w * 0.2) >= -(players->render->screenExtents.w) && (players[0].physics->pos.x - players[i].physics->pos.x) <= 0 && (players[0].physics->pos.y - players[i].physics->pos.y) <= (players->render->screenExtents.h / 2) && (players[0].physics->pos.y - players[i].physics->pos.y) >= -(players->render->screenExtents.h / 2) && players[0].render->flip == 0) //   magic number has to be tweaked.
            {
                players[i].health += 10;
            }
            else if ((players[0].physics->pos.x - players[i].physics->pos.x) - (players->render->screenExtents.w * 0.2) <= (players->render->screenExtents.w) && (players[0].physics->pos.x - players[i].physics->pos.x) >= 0 && (players[0].physics->pos.y - players[i].physics->pos.y) <= (players->render->screenExtents.h / 2) && (players[0].physics->pos.y - players[i].physics->pos.y) >= -(players->render->screenExtents.h / 2) && players[0].render->flip == 1)
            {
                players[i].health += 10;
            }

            /*if ((players[0].physics->pos.x - players[i].physics->pos.x) >= -(players->render->screenExtents.w) && (players[0].physics->pos.x - players[i].physics->pos.x) <= 0 && (players[0].physics->pos.y - players[i].physics->pos.y) <= (players->render->screenExtents.h / 2) && (players[0].physics->pos.y - players[i].physics->pos.y) >= -(players->render->screenExtents.h / 2) && players[0].render->flip == 0) //   magic number has to be tweaked.
            {
                players[i].health += 10;
            }
            else if ((players[0].physics->pos.x - players[i].physics->pos.x) <= (players->render->screenExtents.w) && (players[0].physics->pos.x - players[i].physics->pos.x) >= 0 && (players[0].physics->pos.y - players[i].physics->pos.y) <= (players->render->screenExtents.h / 2) && (players[0].physics->pos.y - players[i].physics->pos.y) >= -(players->render->screenExtents.h / 2) && players[0].render->flip == 1)
            {
                players[i].health += 10;
            }*/
        }
        players->render->imageExtents.x = 96;
    }
}
