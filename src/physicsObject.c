#include "../include/physicsObject.h"
#include <math.h>

void updatePosition(PhysicsObject* obj, float dt) 
{
    if(flagSet(obj->flags, DYNAMIC)) {
        vec2 velocity = vdiff(obj->pos, obj->oldPos);

        obj->oldPos = obj->pos;

        obj->pos = vsum(obj->pos,vsum(velocity,vsmul(obj->acceleration,dt*dt)));
    }
}

void updatePositions(PhysicsObject objects[], int length, const float dt) 
{
    for (int i = 0; i < length; i++)
    {
        if(flagSet(objects[i].flags, DYNAMIC)) {
            // 0.9740037464253f is a magic value equal to 0.9^(1/4) because we take 4 substeps right now --Damien
            
            vec2 velocity = vdiff(objects[i].pos, objects[i].oldPos);
            
            velocity.x *= 0.9740037464253f;

            objects[i].oldPos = objects[i].pos;

            objects[i].pos = vsum(objects[i].pos,vsum(velocity,vsmul(objects[i].acceleration,dt*dt)));
        }
    }
}

void aabbTest(PhysicsObject* obj1, PhysicsObject* obj2) 
{   
    //  complicated code ahead!! (truly sorry) --Damien
    vec2 maxCorner1 = vsum(obj1->pos,obj1->extents);
    vec2 maxCorner2 = vsum(obj2->pos,obj2->extents);

    //  AABB intersection test
    if(obj1->pos.x <= maxCorner2.x &&
       obj1->pos.y <= maxCorner2.y &&
       obj2->pos.x <= maxCorner1.x &&
       obj2->pos.y <= maxCorner1.y )
    {   
        //  pushVector stands for minimum translation distance (vector)
        vec2 pushVector = vdiff(vmin(maxCorner1,maxCorner2),vmax(obj1->pos,obj2->pos));
        pushVector = pushVector.x < pushVector.y ? vec2(pushVector.x, 0.f) : vec2(0.f, pushVector.y);

        pushVector.x = obj2->pos.x <= obj1->pos.x ? pushVector.x : -pushVector.x;
        pushVector.y = obj2->pos.y <= obj1->pos.y ? pushVector.y : -pushVector.y;

        if(flagSet(obj1->flags, DYNAMIC) && !flagSet(obj2->flags, DYNAMIC)) 
        {
            obj1->pos = vsum(obj1->pos, pushVector);
        }
        if(flagSet(obj1->flags, DYNAMIC) && flagSet(obj2->flags, DYNAMIC)) 
        {
            obj1->pos = vsum(obj1->pos, vsmul(pushVector,0.5f));
            obj2->pos = vsum(obj2->pos, vsmul(pushVector,-0.5f));
        }
        if(!flagSet(obj1->flags, DYNAMIC) && flagSet(obj2->flags, DYNAMIC)) 
        {
            obj2->pos = vdiff(obj2->pos, pushVector);
        }  
    }
}

void constraintSolve(PhysicsObject objects[], int length) 
{   
    //  complicated code ahead!! (truly sorry) --Damien
    for (int i = 0; i < length; i++)
    {
        vec2 maxCorner1 = vsum(objects[i].pos,objects[i].extents);
        for (int j = i + 1; j < length; j++)
        {
            vec2 maxCorner2 = vsum(objects[j].pos,objects[j].extents);

            //  AABB intersection test (the objects pos is minCorner)
            if(objects[i].pos.x <= maxCorner2.x &&
               objects[i].pos.y <= maxCorner2.y &&
               objects[j].pos.x <= maxCorner1.x && 
               objects[j].pos.y <= maxCorner1.y) 
            {
                //  pushVector
                vec2 pushVector = vdiff(vmin(maxCorner1,maxCorner2),vmax(objects[i].pos,objects[j].pos));

                pushVector = pushVector.x <= pushVector.y ? vec2(pushVector.x, 0.f) : vec2(0.f, pushVector.y);

                pushVector.x = objects[j].pos.x <= objects[i].pos.x ? pushVector.x : -pushVector.x;
                pushVector.y = objects[j].pos.y <= objects[i].pos.y ? pushVector.y : -pushVector.y;

                if(pushVector.x < 0.f) 
                {
                    objects[j].flags |= LEFT;
                    objects[i].flags |= RIGHT;
                }
                if (pushVector.x > 0.f) 
                {
                    objects[j].flags |= RIGHT;
                    objects[i].flags |= LEFT;
                }

                if(pushVector.y < 0.0f) {
                    objects[j].flags |= DOWN;
                    objects[i].flags |= UP;
                }
                if(pushVector.y > 0.0f) {
                    objects[j].flags |= UP;
                    objects[i].flags |= DOWN;
                }

                //  push objects out of each other
                if(flagSet(objects[i].flags, DYNAMIC) && !flagSet(objects[j].flags, DYNAMIC)) 
                {
                    objects[i].pos = vsum(objects[i].pos, pushVector);
                }
                if(flagSet(objects[i].flags, DYNAMIC) && flagSet(objects[j].flags, DYNAMIC)) 
                {
                    objects[i].pos = vsum(objects[i].pos, vsmul(pushVector,0.5f));
                    objects[j].pos = vsum(objects[j].pos, vsmul(pushVector,-0.5f));
                }
                if(!flagSet(objects[i].flags, DYNAMIC) && flagSet(objects[j].flags, DYNAMIC)) 
                {
                    objects[j].pos = vdiff(objects[j].pos, pushVector);
                }  
            }
        }
    }
}


void boundarySolve(PhysicsObject objects[], int length) {
    for (int i = 0; i < length; i++)
    {
        if(flagSet(objects[i].flags, PLAYER)) 
        {
            // collision detection with windows boundries
            if (objects[i].pos.x <= 0.f)
            {
                // if you collide reset the position so u dont go out of the screen
                objects[i].pos.x = 0.f;
            }
            if (objects[i].pos.y <= 0.f)
            {
                objects[i].pos.y = 0.f;
            }
            // subtract the width of the sprite.
            if (objects[i].pos.x + objects[i].extents.x >= 800.0f)
            {
                objects[i].pos.x = 800.0f - objects[i].extents.x;
            }
            // subtract the height of the sprite.
            if (objects[i].pos.y + objects[i].extents.y >= 600.0f)
            {
                objects[i].pos.y = 600.0f - objects[i].extents.y;
            }
        }
    }
}