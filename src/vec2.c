#include "../include/vec2.h" 
#include <math.h>

//  a+=b
void vpadd(vec2* pA, vec2 b) 
{
    pA->x += b.x;
    pA->y += b.y;
}

//  c = a + b
vec2 vsum(vec2 a, vec2 b) 
{
    return (vec2){a.x+b.x,a.y+b.y};
}

//  a-=b
void vpsub(vec2* pA, vec2 b) 
{
    pA->x -= b.x;
    pA->y -= b.y;
}

//  c = a - b
vec2 vdiff(vec2 a, vec2 b) 
{
    return (vec2){a.x-b.x,a.y-b.y};
}

//  c = {a.x * t, a.y * t}
vec2 vsmul(vec2 a, float t) 
{
    return (vec2){a.x*t,a.y*t};
}

//  c = a * b
vec2 vprod(vec2 a, vec2 b) 
{
    return (vec2){a.x*b.x,a.y*b.y};
}

//  c = a / b
vec2 vquo(vec2 a, vec2 b) 
{
    return (vec2){a.x/b.x,a.y/b.y};
}

//  dot product 
float dot(vec2 a, vec2 b) 
{
    return (a.x * b.x) + (a.y * b.y);
}

//  c = a/|a|
vec2 normalize(vec2 a) 
{
    float length = 1.0f/sqrtf((a.x * a.x) + (a.y * a.y));

    return (vec2){a.x*length, a.y*length};
}

