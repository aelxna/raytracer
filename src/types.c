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
		.mtl = m
	};
	return s;
}

triangle_t triangle_new(int *v, int *n, vec3_t sn, int *tc, mtl_t m) {
	triangle_t t = {
		.snorm = sn,
		.mtl = m
	};
	for (int i = 0; i < 3; i++) {
		t.vertices[i] = v[i];
		if (n) {
			t.normals[i] = n[i];
		} else {
			t.normals[i] = -1;
		}
		if (tc) {
			t.texcoords[i] = tc[i];
		} else {
			t.texcoords[i] = -1;
		}
	}
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
		switch (head->type) {
			case SPHERE:
				free(head->data.s);
				break;
			case TRIANGLE:
				free(head->data.t);
				break;
			case NONE:
				break;
		}
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

void free_config(config_t *c) {
	if (c) {
		free_shapes(c->shape_head);
		free_lights(c->light_head);
		free(c);
	}
}
