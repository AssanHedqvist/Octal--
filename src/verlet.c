#include "../include/verlet.h"


void updateVerletPos(Verlet* obj, float dt) 
{
    vec2 velocity = vdiff(obj->posNow,obj->posOld);

    obj->posOld = obj->posNow;

    obj->posNow = vsum(vsum(obj->posNow,velocity),vsmul(obj->acceleration,dt*dt));

    //obj->acceleration = vec2(0.f,0.f);
}

//  not needed i think --Damien
void accelerate(Verlet* obj, vec2 acc) 
{
    obj->acceleration = vsum(obj->acceleration, acc);
}