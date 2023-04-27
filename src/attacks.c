#include "../include/attacks.h"
#include "../include/keyboard.h"
#include "../include/player.h"

void lightPunch(Player players[], int amountOfPlayers)
{
    if (isKeyDown(&players->keyInputs, SDL_SCANCODE_J))
    {
        for (int i = 1; i < 4; i++)
        {
            //int yDist = players[0].physics->pos.y - players[i].physics->pos.y;
            //int xDist = players[0].physics->pos.x - players[i].physics->pos.x;
            //int playerWidth = players->render->screenExtents.w;
            //int playerHeight = players->render->screenExtents.h;

            if ((players[0].physics->pos.x - players[i].physics->pos.x) >= -(players->render->screenExtents.w) && (players[0].physics->pos.x - players[i].physics->pos.x) <= 0 && (players[0].physics->pos.y - players[i].physics->pos.y) <= (players->render->screenExtents.h / 2) && (players[0].physics->pos.y - players[i].physics->pos.y) >= -(players->render->screenExtents.h / 2) && players[0].render->flip == 0) //   magic number has to be tweaked.
            {
                players[i].health += 10;
            }
            else if ((players[0].physics->pos.x - players[i].physics->pos.x) <= (players->render->screenExtents.w) && (players[0].physics->pos.x - players[i].physics->pos.x) >= 0 && (players[0].physics->pos.y - players[i].physics->pos.y) <= (players->render->screenExtents.h / 2) && (players[0].physics->pos.y - players[i].physics->pos.y) >= -(players->render->screenExtents.h / 2) && players[0].render->flip == 1)
            {
                players[i].health += 10;
            }
        }
        players->render->imageExtents.x = 96;
    }
}
