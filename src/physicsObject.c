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

void updatePositions(PhysicsObject objects[], int length, float dt) 
{
    for (int i = 0; i < length; i++)
    {
        if(objects[i].type != STATIC) {
            // 0.9740037464253f is a magic value equal to 0.9^(1/4) because we take 4 substeps right now --Damien
            vec2 velocity = vprod(vdiff(objects[i].pos, objects[i].oldPos),vec2(0.9740037464253f,1.0f));

            objects[i].oldPos = objects[i].pos;

            objects[i].pos = vsum(objects[i].pos,vsum(velocity,vsmul(objects[i].acceleration,dt*dt)));
        }
    }
}

void aabbTest(PhysicsObject* obj, PhysicsObject* obj2) 
{   
    //  complicated code ahead!! (truly sorry) --Damien
    vec2 maxCorner = vsum(obj->pos,obj->extents);
    vec2 maxCorner2 = vsum(obj2->pos,obj2->extents);

    //  AABB intersection test
    if(!(maxCorner.x < obj2->pos.x || 
         obj->pos.x > maxCorner2.x ||
         maxCorner.y < obj2->pos.y ||
         obj->pos.y > maxCorner2.y)) 
    {
        //  MTD stands for minimum translation distance (vector)
        vec2 MTD = vdiff(vmin(maxCorner,maxCorner2),vmax(obj->pos,obj2->pos));
        MTD = MTD.x < MTD.y ? vec2(MTD.x, 0.f) : vec2(0.f, MTD.y);

        vec2 sign = vdiff(obj->pos,obj2->pos);
        MTD.x = sign.x >= 0.f ? MTD.x : -MTD.x;
        MTD.y = sign.y >= 0.f ? MTD.y : -MTD.y;

        if(obj->type == DYNAMIC && obj2->type == STATIC) 
        {
            obj->pos = vsum(obj->pos, MTD);
        }
        if(obj->type == DYNAMIC && obj2->type == DYNAMIC) 
        {
            obj->pos = vsum(obj->pos, vsmul(MTD,0.5f));
            obj2->pos = vsum(obj2->pos, vsmul(MTD,-0.5f));
        }
        if(obj->type == STATIC && obj2->type == DYNAMIC) 
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
        vec2 maxCorner = vsum(objects[i].pos,objects[i].extents);
        for (int j = 0; j < length; j++)
        {
            //  dont collide the same object with itself   
            if(j != i) //&& !((objects[i].type & PLAYER) == PLAYER && (objects[j].type & PLAYER) == PLAYER)
            {  
                vec2 maxCorner2 = vsum(objects[j].pos,objects[j].extents);

                //  AABB intersection test
                if(!(maxCorner.x < objects[j].pos.x || 
                     objects[i].pos.x > maxCorner2.x ||
                     maxCorner.y < objects[j].pos.y ||
                     objects[i].pos.y > maxCorner2.y)) 
                {
                    //  MTD stands for minimum translation distance (vector)
                    vec2 MTD = vdiff(vmin(maxCorner,maxCorner2),vmax(objects[i].pos,objects[j].pos));
                    MTD = MTD.x < MTD.y ? vec2(MTD.x, 0.f) : vec2(0.f, MTD.y);

                    vec2 sign = vdiff(objects[i].pos,objects[j].pos);
                    MTD.x = sign.x >= 0.f ? MTD.x : -MTD.x;
                    MTD.y = sign.y >= 0.f ? MTD.y : -MTD.y;

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
}