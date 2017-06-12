#include "lmath_full.h"

mat4 rotate_axis(vec3 pt, vec3 dir, float angle)
{
	return
		glm::translate(mat4(), pt) *
		glm::rotate(mat4(), angle, dir) *
		glm::translate(mat4(), -pt);
}

vec3 mul(mat4 m, vec3 v)
{
	vec4 vRes = m * vec4(v, 1);
	return vec3(vRes.x, vRes.y, vRes.z) / vRes.w;
}
