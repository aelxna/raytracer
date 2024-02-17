#include <stdio.h>
#include <stdlib.h>
#include "types.h"

mtl_t mtl_new(vec3_t od, vec3_t os, float ka, float kd, float ks, float n) {
	mtl_t m = {
		.diffuse = od, 
		.specular = os, 
		.ka = ka, 
		.kd = kd, 
		.ks = ks, 
		.n = n
	};
	return m;
}

void rgb_str(char *str, vec3_t *c) {
	sprintf(str, "%3d %3d %3d", (uint32_t)(c->x * 255), (uint32_t)(c->y * 255), (uint32_t)(c->z * 255));
}

sphere_t sphere_new(vec3_t c, float r, mtl_t m) {
	sphere_t s = {
		.center = c,
		.radius = r,
		.mtl = m,
		.next = NULL
	};
	return s;
}

ray3_t ray3_new(vec3_t o, vec3_t d) {
	ray3_t r = {
		.origin = o,
		.dir = d
	};
	return r;
}

light_t light_new(vec3_t p, int w, float i) {
	light_t l = { 
		.pos = p, 
		.w = w, 
		.i = i, 
		.next = NULL 
	};
	return l;
}

void free_spheres(sphere_t *head) {
	while (head) {
		sphere_t *next = head->next;
		free(head);
		head = next;
	}
}

