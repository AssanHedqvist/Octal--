#ifndef PHYSICSOBJECT_H_INCLUDED
#define PHYSICSOBJECT_H_INCLUDED

#include "../include/vec2.h"

typedef struct PhysicsObject
{
    vec2 position;
    vec2 velocity;
    vec2 acceleration;
    unsigned int type; //  for example static objects (immovable objects) and dynamic objects (movable objects)
} PhysicsObject;

void updateEulerPos(PhysicsObject* obj, float dt);


#endif