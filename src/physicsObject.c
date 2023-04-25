#include "../include/physicsObject.h"
#include <math.h>

void updatePosition(PhysicsObject* obj, float dt) 
{
    if(obj->type != STATIC) {
        vec2 velocity = vdiff(obj->pos, obj->oldPos);

        obj->oldPos = obj->pos;

        obj->pos = vsum(obj->pos,vsum(velocity,vsmul(obj->acceleration,dt*dt)));
    }
}

void updatePositions(PhysicsObject objects[], int length, const float dt) 
{
    for (int i = 0; i < length; i++)
    {
        if(objects[i].type != STATIC) {
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
        //  MTD stands for minimum translation distance (vector)
        vec2 MTD = vdiff(vmin(maxCorner1,maxCorner2),vmax(obj1->pos,obj2->pos));
        MTD = MTD.x < MTD.y ? vec2(MTD.x, 0.f) : vec2(0.f, MTD.y);

        MTD.x = obj2->pos.x <= obj1->pos.x ? MTD.x : -MTD.x;
        MTD.y = obj2->pos.y <= obj1->pos.y ? MTD.y : -MTD.y;

        if(obj1->type == DYNAMIC && obj2->type == STATIC) 
        {
            obj1->pos = vsum(obj1->pos, MTD);
        }
        if(obj1->type == DYNAMIC && obj2->type == DYNAMIC) 
        {
            obj1->pos = vsum(obj1->pos, vsmul(MTD,0.5f));
            obj2->pos = vsum(obj2->pos, vsmul(MTD,-0.5f));
        }
        if(obj1->type == STATIC && obj2->type == DYNAMIC) 
        {
            obj2->pos = vdiff(obj2->pos, MTD);
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
                //  MTD stands for minimum translation distance (vector)
                vec2 MTD = vdiff(vmin(maxCorner1,maxCorner2),vmax(objects[i].pos,objects[j].pos));

                MTD = MTD.x < MTD.y ? vec2(MTD.x, 0.f) : vec2(0.f, MTD.y);

                MTD.x = objects[j].pos.x <= objects[i].pos.x ? MTD.x : -MTD.x;
                MTD.y = objects[j].pos.y <= objects[i].pos.y ? MTD.y : -MTD.y;

                if(MTD.y < 0.0f) {
                    objects[j].recentCollision = 1;
                }
                else if(MTD.y > 0.0f) {
                    objects[i].recentCollision = 1;
                }

                //  push objects out of each other
                if(objects[i].type == DYNAMIC && objects[j].type == STATIC) 
                {
                    objects[i].pos = vsum(objects[i].pos, MTD);
                }
                if(objects[i].type == DYNAMIC && objects[j].type == DYNAMIC) 
                {
                    objects[i].pos = vsum(objects[i].pos, vsmul(MTD,0.5f));
                    objects[j].pos = vsum(objects[j].pos, vsmul(MTD,-0.5f));
                }
                if(objects[i].type == STATIC && objects[j].type == DYNAMIC) 
                {
                    objects[j].pos = vdiff(objects[j].pos, MTD);
                }  
            }
        }
    }
}