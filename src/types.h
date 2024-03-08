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

/*
* Sphere containing its center point, radius, material color 
*/
typedef struct {
	vec3_t center;
	float radius;
	mtl_t mtl;
} sphere_t;

typedef struct {
	int vertices[3];
	int normals[3];
	vec3_t snorm;
	float d;
	int texcoords[3];
	mtl_t mtl;
} triangle_t;

/*
* Creates a new sphere_t given a center point, radius, and color
*/
sphere_t sphere_new(vec3_t c, float r, mtl_t m);

/*
* Creates a new triangle_t given vertices, normals, texture coords (optional), and mtlcolor
*/
triangle_t triangle_new(int *v, int *n, vec3_t sn, int *tc, mtl_t m);

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

typedef struct Texture texture_t;

struct Texture {
	int width, height;
	vec3_t *img;
	texture_t *next;
};

// Used to store configuration options
typedef struct {
	vec3_t eye;
	vec3_t viewdir;
	vec3_t updir;
	float hfov;
	int width;
	int height;
	vec3_t bkgcolor;
	shape_t *shape_head;
	light_t *light_head;
	texture_t *texture_head;
	vec3_t vertices[4096];
	vec3_t normals[4096];
	float texcoords[2][4096];
} config_t;

typedef enum {
	SPHERE,
	TRIANGLE,
	NONE
} objtype_t;

void free_config(config_t *c);

typedef struct Shape shape_t;

struct Shape {
	objtype_t type;
	union {
		sphere_t s;
		triangle_t t;
	} data;
	shape_t *next;
};

typedef struct {
	objtype_t type;
	void *shape;
	float t;
} trace_t;

#endif

