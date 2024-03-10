#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"

extern vec3_t vertices[];
extern vec3_t normals[];
extern float texcoords[][2];

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

vec3_t texture_lookup(texture_t *tx, float u, float v) {
	double intpart;
	// nearest neighbor
	int x = roundf(modf(u, &intpart) * (tx->width - 1));
	int y = roundf(modf(v, &intpart) * (tx->height - 1));
	return tx->img[x + (y * tx->width)];
}

void rgb_str(char *str, vec3_t *c) {
	sprintf(str, "%3d %3d %3d", (uint32_t)(c->x * 255), (uint32_t)(c->y * 255), (uint32_t)(c->z * 255));
}

sphere_t sphere_new(vec3_t c, float r, texture_t *tx, mtl_t m) {
	sphere_t s = {
		.center = c,
		.radius = r,
		.texture = tx,
		.mtl = m
	};
	return s;
}

triangle_t triangle_new(int *v, int *n, int *tc, texture_t *tx, mtl_t m) {
	triangle_t t = {
		.mtl = m,
		.texture = tx
	};
	for (int i = 0; i < 3; i++) {
		t.vertices[i] = v[i] - 1;
		if (n) { // vertex normals being used
			t.normals[i] = n[i] - 1;
		} else {
			t.normals[i] = -1;
		}
		if (tc) { // texture coords provided
			t.texcoords[i] = tc[i] - 1;
		} else {
			t.texcoords[i] = -1;
		}
	}
	// calculate e1, e2, surface normal, and d
	t.e1 = sub(vertices[t.vertices[1]], vertices[t.vertices[0]]);
	t.e2 = sub(vertices[t.vertices[2]], vertices[t.vertices[0]]);
	t.snorm = normalize(cross(t.e1, t.e2));
	t.d = -(dot(t.snorm, vertices[t.vertices[1]]));
	return t;
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

void free_shapes(shape_t *head) {
	while (head) {
		shape_t *next = head->next;
		free(head);
		head = next;
	}
}

void free_lights(light_t *head) {
	while (head) {
		light_t *next = head->next;
		free(head);
		head = next;
	}
}

void free_textures(texture_t *head) {
	while (head) {
		free(head->img);
		texture_t *next = head->next;
		free(head);
		head = next;
	}
}

void free_config(config_t *c) {
	if (c) {
		free_shapes(c->shape_head);
		free_lights(c->light_head);
		free_textures(c->texture_head);
		free(c);
	}
}
