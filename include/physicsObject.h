#ifndef PHYSICSOBJECT_H_INCLUDED
#define PHYSICSOBJECT_H_INCLUDED

#include "../include/vec2.h"

typedef enum {
    STATIC = 0b0001,
    DYNAMIC = 0b0011,
    NON_PLAYER = 0b0100,
    PLAYER = 0b1100
} Type;

typedef enum {
    NONE =  0b0000,
    UP =    0b0001,
    RIGHT = 0b0011,
    DOWN =  0b0010,
    LEFT =  0b0110
} CollisionType;

//  Verlet integration
typedef struct PhysicsObject
{
    vec2 acceleration;
    vec2 pos;   //  lower left corner of sprite
    vec2 oldPos;
    vec2 extents;   //  width and height of sprite
    unsigned char type; //  for example static objects (immovable objects) and dynamic objects (movable objects)
    unsigned char recentCollision;
} PhysicsObject;

void updatePosition(PhysicsObject* obj, float dt);

void updatePositions(PhysicsObject objects[], int length, const float dt);

void aabbTest(PhysicsObject* obj, PhysicsObject* obj2);

void constraintSolve(PhysicsObject objects[], int length);


#endif