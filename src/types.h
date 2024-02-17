#ifndef ASCII_TYPES_H
#define ASCII_TYPES_H

#include <inttypes.h>
#include "vec3.h"

typedef uint8_t u8;

typedef struct {
	vec3_t diffuse, specular;
	float ka, kd, ks, n;
} mtl_t;

mtl_t mtl_new(vec3_t od, vec3_t os, float ka, float kd, float ks, float n);

/*
* Converts the given color into a string stored in str
* Assumes that str has 12 bytes allocated before calling. 
* str: pointer to where string will be stored
* c: color to be converted to a string
*/ 
void rgb_str(char *str, vec3_t *c);

typedef struct Sphere sphere_t;

/*
* Sphere containing its center point, radius, material color, 
* and a pointer to the next sphere in a linked list
*/
struct Sphere {
	vec3_t center;
	float radius;
	mtl_t mtl;
	sphere_t *next;
};

/*
* Creates a new sphere_t given a center point, radius, and color
*/
sphere_t sphere_new(vec3_t c, float r, mtl_t m);

// Represents a ray with an origin point and a direction
typedef struct {
	vec3_t origin, dir;
} ray3_t;

/*
* Creates a new ray3_t given an origin point and a direction
*/
ray3_t ray3_new(vec3_t o, vec3_t d);

typedef struct Light light_t;

struct Light {
	vec3_t pos;
	int w;
	float i;
	light_t *next;
};

light_t light_new(vec3_t p, int w, float i);

// Used to store configuration options
typedef struct {
	vec3_t eye;
	vec3_t viewdir;
	vec3_t updir;
	float hfov;
	int width;
	int height;
	vec3_t bkgcolor;
	sphere_t *sphere_head;
	light_t *light_head;
} config_t;

/*
* Free spheres from linked list given pointer to head
*/
void free_spheres(sphere_t *head);

typedef struct {
	sphere_t *s;
	float t;
} trace_t;

#endif

