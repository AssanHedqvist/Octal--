#include "../include/physicsObject.h"
#include <math.h>

void initPhysicsObjects(PhysicsObject objects[]) 
{
     //  (platform)
    objects[0].acceleration = vec2(0.f, 0.f);
    objects[0].pos = vec2(100, 247);
    objects[0].oldPos = objects[0].pos;
    objects[0].extents = vec2(600, 53);
    objects[0].flags = PHYSICS_ACTIVE;

    //  (player 0)
    objects[1].acceleration = vec2(0.f, -982.0f);
    objects[1].pos = vec2(150, 536);
    objects[1].oldPos = objects[1].pos;
    objects[1].extents = vec2(32, 64);
    objects[1].flags = (DYNAMIC | PLAYER);
    //  (player 1)
    objects[2].acceleration = vec2(0.f, -982.0f);
    objects[2].pos = vec2(316.6666666666666666666666666666f, 536);
    objects[2].oldPos = objects[2].pos;
    objects[2].extents = vec2(32, 64);
    objects[2].flags = (DYNAMIC | PLAYER);

    //  (player 2)
    objects[3].acceleration = vec2(0.f, -982.0f);
    objects[3].pos = vec2(483.33333333333333333333333333334f, 536);
    objects[3].oldPos = objects[3].pos;
    objects[3].extents = vec2(32, 64);
    objects[3].flags = (DYNAMIC | PLAYER);

    //  (player 3)
    objects[4].acceleration = vec2(0.f, -982.0f);
    objects[4].pos = vec2(650, 536);
    objects[4].oldPos = objects[4].pos;
    objects[4].extents = vec2(32, 64);
    objects[4].flags = (DYNAMIC | PLAYER);
}

void updatePositions(PhysicsObject objects[], int length, const float dt) 
{
    for (int i = 0; i < length; i++)
    {

        if(flagPhysicsGet(objects[i].flags, DYNAMIC) && flagPhysicsGet(objects[i].flags, PHYSICS_ACTIVE)) {
            // 0.9740037464253f is a magic value equal to 0.9^(1/4) because we take 4 substeps right now --Damien
            
            vec2 velocity = vdiff(objects[i].pos, objects[i].oldPos);
            
            velocity.x *= 0.9740037464253f;

            objects[i].oldPos = objects[i].pos;

            objects[i].pos = vsum(objects[i].pos,vsum(velocity,vsmul(objects[i].acceleration,dt*dt)));
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
               objects[j].pos.y <= maxCorner1.y &&
             !(flagPhysicsGet(objects[i].flags, PLAYER) && 
               flagPhysicsGet(objects[j].flags, PLAYER)) && 
               flagPhysicsGet(objects[i].flags, PHYSICS_ACTIVE) && 
               flagPhysicsGet(objects[j].flags, PHYSICS_ACTIVE)) 
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
                if(flagPhysicsGet(objects[i].flags, DYNAMIC) && !flagPhysicsGet(objects[j].flags, DYNAMIC)) 
                {
                    objects[i].pos = vsum(objects[i].pos, pushVector);
                }
                if(flagPhysicsGet(objects[i].flags, DYNAMIC) && flagPhysicsGet(objects[j].flags, DYNAMIC)) 
                {
                    objects[i].pos = vsum(objects[i].pos, vsmul(pushVector,0.5f));
                    objects[j].pos = vsum(objects[j].pos, vsmul(pushVector,-0.5f));
                }
                if(!flagPhysicsGet(objects[i].flags, DYNAMIC) && flagPhysicsGet(objects[j].flags, DYNAMIC)) 
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
        if(flagPhysicsGet(objects[i].flags, PLAYER) && flagPhysicsGet(objects[i].flags, PHYSICS_ACTIVE)) 
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