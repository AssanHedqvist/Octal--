#ifndef PHYSICSOBJECT_H_INCLUDED
#define PHYSICSOBJECT_H_INCLUDED

#include "../include/vec2.h"

//  Verlet integration
typedef struct
{
    vec2 acceleration;
    vec2 pos;   //  lower left corner of sprite
    vec2 oldPos;
    vec2 extents;   //  width and height of sprite
    unsigned char flags; //  flags about physicsObject
} PhysicsObject;

typedef enum {
    DYNAMIC = 0b00000010,
    PLAYER = 0b00000100,
    PHYSICS_ACTIVE = 0b00001000,
    UP =    0b00010000,
    RIGHT = 0b00100000,
    DOWN =  0b01000000,
    LEFT =  0b10000000
} physicsFlags;

static inline unsigned char flagPhysicsGet(unsigned char objFlags, unsigned char physicsFlagToCheck) 
{
    return ((objFlags & physicsFlagToCheck) > 0);
}

void initPhysicsObjects(PhysicsObject objects[]);

void updatePositions(PhysicsObject objects[], int length, const float dt);

void constraintSolve(PhysicsObject objects[], int length);

void boundarySolve(PhysicsObject objects[], int length);


#endif