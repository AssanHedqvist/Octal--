#include "../include/physicsObject.h"

void updatePosition(PhysicsObject* object, float dt) 
{
    object->velocity = vsum(object->velocity,vsmul(object->acceleration,dt));
    object->position = vsum(object->position,vsmul(object->velocity,dt));
}
