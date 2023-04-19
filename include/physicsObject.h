#ifndef PHYSICSOBJECT_H_INCLUDED
#define PHYSICSOBJECT_H_INCLUDED

#include "../include/vec2.h"

typedef enum type {
    STATIC = 0b0000,
    DYNAMIC = 0b0001,
    PLAYER = 0b1000
} Type;

typedef struct PhysicsObject
{
    
    vec2 acceleration;
    vec2 pos;   //  lower left corner of sprite
    vec2 oldPos;
    vec2 extents;   //  width and height of sprite
    Type type; //  for example static objects (immovable objects) and dynamic objects (movable objects)
} PhysicsObject;

void updatePosition(PhysicsObject* obj, float dt);

void updatePositions(PhysicsObject objects[], int length, float dt);

void aabbTest(PhysicsObject* obj, PhysicsObject* obj2);

void constraintSolve(PhysicsObject objects[], int length);


#endif