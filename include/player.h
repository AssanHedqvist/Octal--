#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../include/renderObject.h"
#include "../include/physicsObject.h"
#include "../include/keyboard.h"
#include "../include/sounds.h"


typedef struct
{
    RenderObject *render;
    PhysicsObject *physics;
    unsigned char amountOfJumpsLeft;
    unsigned short health;
    unsigned char lives;
    unsigned char recentlyHit;
    unsigned char animationState;

    float timeSinceLastJump;
    float timeSinceLastPunch;
    float timeSinceHit;
    //  add more
} Player;

//   numbers represent rows in spritesheet
typedef enum {
    IDLE = 0,
    RUN = 1,
    JUMP = 2,
    PUNCH = 3    
} animationState;

//   numbers represent rows in spritesheet
typedef enum {
IDLE_0       = 0,
IDLE_1       = 1,
IDLE_2       = 2,
IDLE_3       = 3,
IDLE_4       = 4,
IDLE_5       = 5,
IDLE_6       = 6,
IDLE_7       = 7,
IDLE_8       = 8,
IDLE_9       = 9,
IDLE_10      = 10,
IDLE_11      = 11,
IDLE_12      = 12,
IDLE_13      = 13,
IDLE_14      = 14,
IDLE_15      = 15,
IDLE_16      = 16,
RUN_0        = 17,
RUN_1        = 18,
RUN_2        = 19,
RUN_3        = 20,
RUN_4        = 21,
RUN_5        = 22,
RUN_6        = 23,
RUN_7        = 24,
RUN_8        = 25,
RUN_9        = 26,
RUN_10       = 27,
RUN_11       = 28,
RUN_12       = 29,
RUN_13       = 30,
RUN_14       = 31,
RUN_15       = 32,
RUN_16       = 33,
JUMP_0       = 34,
JUMP_1       = 35,
JUMP_2       = 36,
JUMP_3       = 37,
JUMP_4       = 38,
JUMP_5       = 39,
JUMP_6       = 40,
JUMP_7       = 41,
JUMP_8       = 42,
JUMP_9       = 43,
JUMP_10      = 44,
JUMP_11      = 45,
JUMP_12      = 46,
JUMP_13      = 47,
JUMP_14      = 48,
JUMP_15      = 49,
JUMP_16      = 50,
PUNCH_0      = 51,
PUNCH_1      = 52,
PUNCH_2      = 53,
PUNCH_3      = 54,
PUNCH_4      = 55,
PUNCH_5      = 56,
PUNCH_6      = 57,
PUNCH_7      = 58,
PUNCH_8      = 59,
PUNCH_9      = 60,
PUNCH_10     = 61,
PUNCH_11     = 62,
PUNCH_12     = 63,
PUNCH_13     = 64,
PUNCH_14     = 65,
PUNCH_15     = 66,
PUNCH_16     = 67,
} animationStateAlt;

void handlePlayerInputs(Player *player, float dt, KeyboardStates *keyboardInputs /*, SoundEffect soundEffect*/);

void handlePlayerLives(Player player[4]);

void handlePlayerAnimation(Player player[4]);

void handlePlayerAnimationServer(Player player[4]);

void handlePlayerAnimationAlt(Player player[4]);

void handlePlayerAnimationServerAlt(Player player[4]);

#endif