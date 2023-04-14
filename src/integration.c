#include "../include/integration.h"


void updateVerletPos(Verlet* obj, float dt) 
{
    vec2 velocity = vdiff(obj->posNow,obj->posOld);

    obj->posOld = obj->posNow;

    obj->posNow = vsum(vsum(obj->posNow,velocity),vsmul(obj->acceleration,dt*dt));

    //obj->acceleration = vec2(0.f,0.f);
}

void updateEulerPos(SemiEuler* obj, float dt) 
{
    obj->velocity = vsum(obj->velocity,vsmul(obj->acceleration,dt));
    obj->position = vsum(obj->position,vsmul(obj->velocity,dt));
}

//  not needed i think --Damien
void accelerate(Verlet* obj, vec2 acc) 
{
    obj->acceleration = vsum(obj->acceleration, acc);
}