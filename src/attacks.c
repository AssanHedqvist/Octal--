#include "../include/attacks.h"
#include "../include/keyboard.h"
#include "../include/player.h"

void lightPunch(Player players[], int amountOfPlayers)
{
    if (isKeyDown(&players->keyInputs, SDL_SCANCODE_J))
    {
        for (int i = 0; i < 4; i++)
        {
            //  could change this to a aabb test potentially
            if ((players[0].physics->pos.x - players[i].physics->pos.x) >  40) //   magic number has to be tweaked.
            {
                players[i].health += 10;
            }
        }
        players->render->imageExtents.x = 96;
    }
}
