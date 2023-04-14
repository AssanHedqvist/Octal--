#ifndef INTEGRATION_H_INCLUDED
#define INTEGRATION_H_INCLUDED

#include "../include/vec2.h"

/* Source
    Verlet Integration: https://en.wikipedia.org/wiki/Verlet_integration
    Writing a Physics Engine from scratch (0:0 to 1:52): https://www.youtube.com/watch?v=lS_qeBy3aQI
*/

typedef struct Verlet 
{
    vec2 posNow;
    vec2 posOld;
    vec2 acceleration;
} Verlet;

typedef struct SemiEuler 
{
    vec2 position;
    vec2 velocity;
    vec2 acceleration;
} SemiEuler;

void updateVerletPos(Verlet* obj, float dt);

void updateEulerPos(SemiEuler* obj, float dt);

void accelerate(Verlet* obj, vec2 acc);

#endif