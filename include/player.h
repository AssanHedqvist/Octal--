#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../include/renderObject.h"
#include "../include/physicsObject.h"
#include "../include/keyboard.h"
#include "../include/sounds.h"

typedef struct {
    RenderObject *render;
    PhysicsObject *physics;
    unsigned char amountOfJumpsLeft;
    unsigned short health;
    char lives;
    unsigned char recentlyHit;
    unsigned char animationState;

    float timeSinceLastJump;
    float timeSinceLastPunch;
    float timeSinceHit;
    float timeBlockHeld;
    float timeSinceLastBlock;
    //  add more
} Player;

void initPlayers(Player players[]);

void handlePlayerInputsServer(Player player[4], KeyboardStates keyboardInputs[4], unsigned char playerFlip[4]);

void handlePlayerInputsClient(Player *player, KeyboardStates *keyboardInputs, SoundEffect soundEffect);

void handlePlayerLivesServer(Player player[4]);

void handlePlayerLivesClient(Player player[4], SoundEffect soundEffect);

void handlePlayerAnimationServer(Player player[4]);

void handlePlayerAnimationClient(Player player[4]);

//  client only function
void updatePlayerRenderWithAnimation(Player player[4]);

/*
    Every _0 animation isn't really an animation it is just there to make the code work properly
*/
typedef enum {
    IDLE_0 ,
    IDLE_1 ,
    IDLE_2 ,
    IDLE_3 ,
    IDLE_4 ,
    IDLE_5 ,
    IDLE_6 ,
    IDLE_7 ,
    IDLE_8 ,
    IDLE_9 ,
    IDLE_10,
    IDLE_11,
    IDLE_12,
    IDLE_13,
    IDLE_14,
    IDLE_15,
    IDLE_16,
    RUN_0  ,
    RUN_1  ,
    RUN_2  ,
    RUN_3  ,
    RUN_4  ,
    RUN_5  ,
    RUN_6  ,
    RUN_7  ,
    RUN_8  ,
    RUN_9  ,
    RUN_10 ,
    RUN_11 ,
    RUN_12 ,
    RUN_13 ,
    RUN_14 ,
    RUN_15 ,
    RUN_16 ,
    JUMP_0 ,
    JUMP_1 ,
    JUMP_2 ,
    JUMP_3 ,
    JUMP_4 ,
    JUMP_5 ,
    JUMP_6 ,
    JUMP_7 ,
    JUMP_8 ,
    JUMP_9 ,
    JUMP_10,
    JUMP_11,
    JUMP_12,
    JUMP_13,
    JUMP_14,
    JUMP_15,
    JUMP_16,
    PUNCH_0,
    PUNCH_1,
    PUNCH_2,
    PUNCH_3,
    PUNCH_4,
    PUNCH_5,
    PUNCH_6,
    PUNCH_7,
    PUNCH_8,
    PUNCH_9,
    PUNCH_10,
    PUNCH_11,
    PUNCH_12,
    PUNCH_13,
    PUNCH_14,
    PUNCH_15,
    PUNCH_16,
    BLOCK_0,
    BLOCK_1,
    BLOCK_2,
    BLOCK_3,
    BLOCK_4,
    BLOCK_5,
    BLOCK_6,
    BLOCK_7,
    BLOCK_8,
    BLOCK_9,
    BLOCK_10,
    BLOCK_11,
    BLOCK_12,
    BLOCK_13,
    BLOCK_14,
    BLOCK_15,
    BLOCK_16,
} animationState;

#endif