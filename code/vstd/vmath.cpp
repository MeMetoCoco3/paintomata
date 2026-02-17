#include "vmath.h"
#include "math.h"
#include "vtypes.h"

constexpr auto PI = 3.14159265359f;




f32 Signf32(f32 val)
{
    if (val == 0)
    {
	return 0;
    }

    return val > 0.0f ? 1.0f: -1.0f;
}

vec2 Vec2xScalar(vec2 v, f32 s)
{
    return {v.x * s, v.y * s};
}
vec3 Vec3xScalar(vec3 v, f32 s)
{
    return {v.x * s, v.y * s, v.z * s};
}
vec2 Vec2Add(vec2 v1, vec2 v2)
{
    return {v1.x+v2.x, v1.y+v2.y};
}

vec2 Vec2AddScalar(vec2 v, f32 s)
{
    return {v.x+s, v.y+s};
}

vec2 Vec2Subs(vec2 v1, vec2 v2)
{
    return {v1.x-v2.x, v1.y-v2.y};
}

f32 Minf32(f32 a, f32 b)
{
    return a < b ? a : b;
}

f32 Maxf32(f32 a, f32 b)
{
    return a > b ? a : b;
}

vec3 Vec3Cross(vec3 v1, vec3 v2)
{
    return {
        (v1.y * v2.z) - (v1.z * v2.y),
        (v1.z * v2.x) - (v1.x * v2.z),
        (v1.x * v2.y) - (v1.y * v2.x),
    };
}

vec2 Vec2Normalize(vec2 v)
{
    float mag = Vec2Length(v); 
    return mag == 0 ? vec2{0, 0} : vec2{v.x /mag, v.y/mag};
}

vec3 Vec3Normalize(vec3 v)
{
    float mag = Vec3Length(v); 
    return mag == 0 ? vec3{0, 0, 0} : vec3{v.x /mag, v.y/mag, v.z/mag};
}


vec2 Vec2Dir(vec2 v)
{
    float mag = Vec2Length(v); 
    return mag == 0 ? vec2{0, 0} : vec2{v.x /mag, v.y/mag};
}

f32 Clampf32(f32 v, f32 min, f32 max)
{
    return Minf32(Maxf32(v, min), max);
}

f32 Absf32(f32 val)
{
    return val > 0 ? val : -val;
}

vec2 Vec2Unit(vec2 v)
{
    f32 magnitude = Vec2Length(v);
    return {v.x/magnitude, v.y/magnitude};
}

f32 EaseOutCubic(f32 number)
{
    return f32(sin((number * PI) / 2.0f));}


f32 Lerp(f32 start, f32 end, f32 amount)
{
    return start + amount*(end-start);
}

f32 Dot(vec2 v1, vec2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

f32 Vec2Length(vec2 v)
{
    return sqrtf(powf(v.x, 2) + powf(v.y, 2)); 
}

f32 Vec3Length(vec3 v)
{
    return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2)); 
}


f32 Angle2Vectors(vec2 v1, vec2 v2)
{
    f32 dot = Dot(v1, v2);
    f32 mag1 = Vec2Length(v1);
    f32 mag2 = Vec2Length(v2);


    f32 cos = dot / (mag1 * mag2);

    if (cos >  1) cos =  1;
    if (cos < -1) cos = -1;
    
    return acosf(cos);
}

