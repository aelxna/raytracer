#include "vec3.h"
#include <math.h>

vec3_t vec3_new(float x, float y, float z) {
	vec3_t v = {
		.x = x,
		.y = y,
		.z = z
	};
	return v;
}

vec3_t add(vec3_t u, vec3_t v) {
	vec3_t a = {
		.x = u.x + v.x,
		.y = u.y + v.y,
		.z = u.z + v.z
	};
	return a;
}

vec3_t sub(vec3_t u, vec3_t v) {
	vec3_t a = {
		.x = u.x - v.x,
		.y = u.y - v.y,
		.z = u.z - v.z
	};
	return a;
}

vec3_t scale(float s, vec3_t v) {
	vec3_t a = {
		.x = s * v.x,
		.y = s * v.y,
		.z = s * v.z
	};
	return a;
}

vec3_t negate(vec3_t v) {
	vec3_t a = {
		.x = -v.x,
		.y = -v.y,
		.z = -v.z
	};
	return a;
}

float dot(vec3_t u, vec3_t v) {
	return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

vec3_t cross(vec3_t u, vec3_t v) {
	vec3_t a = {
		.x = (u.y * v.z) - (u.z * v.y),
		.y = (u.z * v.x) - (u.x * v.z),
		.z = (u.x * v.y) - (u.y * v.x)
	};
	return a;
}

float sq_mag(vec3_t v) {
	return dot(v, v);
}

float len(vec3_t v) {
	return sqrtf(sq_mag(v));
}

vec3_t normalize(vec3_t v) {
	return scale((1.0 / len(v)), v);
}

int equal(vec3_t u, vec3_t v) {
	return (
		(u.x == v.x) &&
		(u.y == v.y) &&
		(u.z == v.z)
	);
}

vec3_t rgb_clamp(vec3_t c) {
		 if (c.x < 0) { c.x = 0.0; }
	else if (c.x > 1) { c.x = 1.0; }
		 if (c.y < 0) { c.y = 0.0; }
	else if (c.y > 1) { c.y = 1.0; }
		 if (c.z < 0) { c.z = 0.0; }
	else if (c.z > 1) { c.z = 1.0; }
	return c;
}

vec3_t rgb_new(float r, float g, float b) {
	vec3_t c = {
		.x = r,
		.y = g,
		.z = b
	};
	return rgb_clamp(c);
}

