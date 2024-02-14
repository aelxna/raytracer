#include "debug.h"
#include <stdio.h>

void print_config(config_t *c) {
	printf("eye: (%f, %f, %f)\n", c->eye.x, c->eye.y, c->eye.z);
	printf("viewdir: (%f, %f, %f)\n", c->viewdir.x, c->viewdir.y, c->viewdir.z);
	printf("updir: (%f, %f, %f)\n", c->updir.x, c->updir.y, c->updir.z);
	printf("hfov: %f\n", c->hfov);
	printf("dimensions: %d x %d\n", c->width, c->height);
	printf("bkgcolor: (%f, %f, %f)\n", c->bkgcolor.x, c->bkgcolor.y, c->bkgcolor.z);
}

