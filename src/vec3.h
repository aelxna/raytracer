#ifndef VECTHREE_H
#define VECTHREE_H

// Represents a vector with x, y, and z values
typedef struct {
	float x, y, z;
} vec3_t;

/*
* Create new vec3_t struct given x, y, and z
*/
vec3_t vec3_new(float x, float y, float z);

/*
* Add two vec3_t's together
*/
vec3_t vec3_add(vec3_t u, vec3_t v);

/*
* Multiply a vec3_t by a scalar s
*/
vec3_t vec3_scale(float s, vec3_t v);

/*
* Compute the dot product between two vec3_t's
*/
float dot(vec3_t u, vec3_t v);

/*
* Compute the cross product between two vec3_t's
*/
vec3_t cross(vec3_t u, vec3_t v);

/*
* Calculate the length of a vec3_t
*/
float vec3_len(vec3_t v);

/*
* Transform a vec3_t into a unit vector
*/
vec3_t normalize(vec3_t v);

/*
* Check if two vec3_t's are equal
*/
int vec3_equal(vec3_t u, vec3_t v);

/*
* Clamp a vec3 so that all values are between 0 and 1
*/
vec3_t rgb_clamp(vec3_t c);

vec3_t rgb_new(float r, float g, float b);

#endif

