#ifndef lmathfull_h
#define lmathfull_h

#include <glm/gtc/matrix_transform.hpp>
#include "lmath.h"

mat4 rotate_axis(vec3 pt, vec3 dir, float angle);
vec3 mul(mat4 m, vec3 v);

#endif
