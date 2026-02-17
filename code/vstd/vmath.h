#pragma once

#ifndef VMATH
#define VMATH

#include "vtypes.h"



vec2 Vec2Subs(vec2 v1, vec2 v2);
vec2 Vec2Add(vec2 v1, vec2 v2);
vec2 Vec2AddScalar(vec2 v, f32 s);
vec2 Vec2xScalar(vec2 v, f32 s);
vec3 Vec3xScalar(vec3 v, f32 s);
f32 Signf32(f32 val);
vec2 Vec2Normalize(vec2 v);
vec3 Vec3Normalize(vec3 v);
vec3 Vec3Cross(vec3 v1, vec3 v2);
vec2 Vec2Dir(vec2 v);
f32 Clampf32(f32 v, f32 min, f32 max);
f32 Dot(vec2 v1, vec2 v2);
f32 Angle2Vectors(vec2 v1, vec2 v2);
f32 Lerp(f32 start, f32 end, f32 amount);
f32 EaseOutCubic(f32 number);
f32 Maxf32(f32 a, f32 b);
f32 Minf32(f32 a, f32 b);
f32 Absf32(f32 val);
f32 Vec2Length(vec2 v);
f32 Vec3Length(vec3 v);
vec2 Vec2Unit(vec2 v);

#endif


