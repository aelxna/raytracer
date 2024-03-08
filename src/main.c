#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "vec3.h"
#include "types.h"
#include "debug.h"

/*
 * Parses input file for configurations
 * fc: pointer to config file
 * c: pointer to config struct
 * Returns -1 if any values are not defined properly, 0 otherwise
 */
int generate_config(FILE *fc, config_t *c) {
	char ln[512];
	float vx, vy, vz;
	float ux, uy, uz;
	float br, bg, bb;
	float odr, odg, odb, osr, osg, osb, ka, kd, ks, n;
	float sx, sy, sz, sr;
	float lx, ly, lz, i;
	int lw;
	mtl_t mtlcolor;
	int vindex = 0, nindex = 0, tcindex = 0;
	// check if each mandatory config is present
	int initialized[7] = {0};
	// read each line and find the pattern (if any) that each line follows
	while (fgets(ln, 512, fc)) {
		       if (sscanf(ln, "imsize %d %d", &c->width, &c->height) == 2) { 
			initialized[0] = 1;
			initialized[1] = 1;
			continue;
		} else if (sscanf(ln, "eye %f %f %f", &c->eye.x, &c->eye.y, &c->eye.z) == 3) { 
			initialized[2] = 1;
			continue;
		} else if (sscanf(ln, "viewdir %f %f %f", &vx, &vy, &vz) == 3) { 
			c->viewdir = normalize(vec3_new(vx, vy, vz));
			initialized[3] = 1;
			continue;
		} else if (sscanf(ln, "updir %f %f %f", &ux, &uy, &uz) == 3) { 
			c->updir = normalize(vec3_new(ux, uy, uz));
			initialized[4] = 1;
			continue;
		} else if (sscanf(ln, "hfov %f", &c->hfov) == 1) { 
			initialized[5] = 1;
			continue;
		} else if (sscanf(ln, "bkgcolor %f %f %f", &br, &bg, &bb) == 3) {
			c->bkgcolor = rgb_new(br, bg, bb);
			initialized[6] = 1;
			continue;
		} else if (sscanf(ln, "mtlcolor %f %f %f %f %f %f %f %f %f %f", &odr, &odg, &odb, &osr, &osg, &osb, &ka, &kd, &ks, &n) == 10) {
			mtlcolor = mtl_new(
				rgb_new(odr, odg, odb),
				rgb_new(osr, osg, osb),
				ka, kd, ks, n
			);
			continue;
		} else if (sscanf(ln, "sphere %f %f %f %f", &sx, &sy, &sz, &sr) == 4) { 
			sphere_t *new_sphere = malloc(sizeof(sphere_t));
			*new_sphere = sphere_new(vec3_new(sx, sy, sz), sr, mtlcolor);
			shape_t *new_shape = malloc(sizeof(shape_t));
			new_shape->type = SPHERE;
			new_shape->data.s = new_sphere;
			new_shape->next = NULL;

			shape_t *curr = c->shape_head;
			if (curr == NULL) {
				c->shape_head = new_shape;
			} else {
				while (curr->next) {
					curr = curr->next;
				}
				curr->next = new_shape;
			}
			continue;
		} else if (sscanf(ln, "light %f %f %f %d %f", &lx, &ly, &lz, &lw, &i) == 5) { 
			light_t *new_light = malloc(sizeof(light_t));
			*new_light = light_new(vec3_new(lx, ly, lz), lw, fmin(1.0, fmax(0.0, i)));
			light_t *curr = c->light_head;
			if (curr == NULL) {
				c->light_head = new_light;
			} else {
				while (curr->next) {
					curr = curr->next;
				}
				curr->next = new_light;
			}
			continue;
		}
	}

	int ret_val = 0;
	// handle invalid/incomplete input
	if (!initialized[0] || !initialized[1] || (c->width <= 0 || c->height <= 0)) {
		printf("Please specify a valid width and height using 'imsize {width} {height}'\n");
		ret_val = -1;
	}
	if (!initialized[2]) {
		printf("Please specify an eye position using 'eye {x} {y} {z}'\n");
		ret_val = -1;
	}
	if (!initialized[3]) {
		printf("Please specify a view direction using 'viewdir {x} {y} {z}'\n");
		ret_val = -1;
	}
	if (!initialized[4]) {
		printf("Please specify an up direction using 'updir {x} {y} {z}'\n");
		ret_val = -1;
	}
	if (!initialized[5] || (c->hfov <= 0.0)) {
		printf("Please specify a positive hfov with 'hfov {x}'\n");
		ret_val = -1;
	}
	if (!initialized[6]) {
		printf("Please specify a background color with 'bkgcolor {r} {g} {b}'\n");
		ret_val = -1;
	}

	if (initialized[3] && initialized[4] && (vec3_equal(c->viewdir, c->updir) || vec3_equal(c->viewdir, vec3_scale(-1.0, c->updir)))) {
		printf("Error: updir and viewdir are co-linear\n");
		ret_val = -1;
	}

	return ret_val;
}

/*
 * Finds the distance between the ray origin and the closest intersection pt
 * r: ray
 * s: head of sphere list
 * skip: ptr to sphere that should be skipped in calculations
 * returns trace_t containing intersecting sphere and distance t
 */
trace_t trace_ray(ray3_t *r, shape_t *s, void *skip) {
	// find closest sphere that can be displayed at that pixel and set the color accordingly
	trace_t ret = {
		.type = NONE;
		.t = -1.0;
	}
	void *si = NULL;
	while (s) {
		switch (s->type) {
			case SPHERE:
				sphere_t *sphere = &s->data.s;
				if ((void *)sphere != skip) {
					float b = 2.0 * dot(r->dir, sub(r->origin, sphere->center));
					float constant = sq_mag(sub(r->origin, sphere->center)) - (sphere->radius * sphere->radius);
					
					float t1 = -1.0, t2 = -1.0;
					float discrim = (b * b) - (4.0 * constant);
					if (discrim >= 0) {
						t1 = (-b + sqrtf(discrim)) / 2.0;
						t2 = (-b - sqrtf(discrim)) / 2.0;
					}
					
					// pick t that is closest to origin and still positive
					if ((t1 > 0) && (t1 <= t2)) {
						if ((t1 < ret.t) || (ret.t < 0)) {
							ret.t = t1;
							si = (void *)sphere;
							ret.type = SPHERE;
						}
					} else if (t2 > 0) {
						if ((t2 < ret.t) || (ret.t < 0)) {
							ret.t = t2;
							si = (void *)sphere;
							ret.type = SPHERE;
						}
					}
				}
				break;
			case TRIANGLE:
				triangle_t *tri = &s->data.t;
				if ((void *)tri != skip) {

				}
				break;
			case NONE:
				break;
		}
		s = s->next;
	}
	
	ret.shape = si;
	return ret;
}

/*
* Writes the ppm image to pixel array
* pixels: array of pixels in row major order
* c: struct containing configuration
*/
void generate_image(vec3_t *pixels, config_t *c) {
	int total = c->width * c->height;
	// world coordinates
	vec3_t w = normalize(negate(c->viewdir));
	vec3_t u = normalize(cross(c->viewdir, c->updir));
	vec3_t v = normalize(cross(u, c->viewdir));
	
	// width and height of viewing window
	float aspect = (float)(c->width) / (float)(c->height);
	float d = 5.0;
	float hfov_rad = c->hfov * M_PI / 180.0;
	float width = 2 * d * tan(0.5 * hfov_rad);
	float height = width / aspect;
	
	// corners of viewing window
	vec3_t center = add(c->eye, scale(d, c->viewdir));
	vec3_t ul = add(add(center, scale((-0.5 * width), u)), scale(( 0.5 * height), v));
	vec3_t ur = add(add(center, scale(( 0.5 * width), u)), scale(( 0.5 * height), v));
	vec3_t ll = add(add(center, scale((-0.5 * width), u)), scale((-0.5 * height), v));
	vec3_t lr = add(add(center, scale(( 0.5 * width), u)), scale((-0.5 * height), v));

	vec3_t dx = scale((1.0 / (c->width - 1)), sub(ur, ul));
	vec3_t dy = scale((1.0 / (c->height - 1)), sub(ll, ul));

	for (int i = 0; i < total; i++) {
		int x = i % c->width;
		int y = i / c->width;
		
		// create ray pointing to that pixel
		vec3_t loc = add(ul, add(scale(x, dx), scale(y, dy)));
		ray3_t r = ray3_new(c->eye, normalize(sub(loc, c->eye)));
	
		trace_t tr = trace_ray(&r, c->shape_head, NULL);
		
		switch (tr.type) {
			case SPHERE:
				sphere_t *s = (sphere_t *)(tr.shape);
				// set up lighting equation variables
				vec3_t illum = scale(s->mtl.ka, s->mtl.diffuse);
				vec3_t p = add(r.origin, scale(tr.t, r.dir));
				vec3_t normal = normalize(sub(p, s->center));
				vec3_t vi = normalize(negate(r.dir));
				vec3_t l, h;

				light_t *cl = c->light_head;
				while (cl) { // iterates through light_t linked list
					if (cl->w) { // point light
						l = normalize(sub(cl->pos, p));	
					} else { // directional light
						l = normalize(negate(cl->pos));
					}
					h = normalize(add(l, vi));
					
					// calculate diffuse and specular components
					vec3_t df = scale((s->mtl.kd * fmax(0.0, dot(normal, l))), s->mtl.diffuse);
					vec3_t sp = scale((s->mtl.ks * fmax(0.0, pow(dot(normal, h), s->mtl.n))), s->mtl.specular);
					
					vec3_t li = scale(cl->i, add(df, sp));
					
					// check if shadow
					float shadow = 1.0;
					ray3_t sr = ray3_new(p, l);
					
					// trace ray between p and light
					trace_t st = trace_ray(&sr, c->shape_head, (void *)s);
					
					if (st.t > 0) { // found intersection
						if (cl->w) { // point light
							// check if the intersection is past the light (no shadow) or not (yes shadow)
							float light_dist = len(l);
							if (light_dist >= st.t) {
								shadow = 0.0;
							}
						} else {
							shadow = 0.0;
						}
					} 

					illum = add(illum, scale(shadow, li));
					
					cl = cl->next;
				}
				// clamp down to 1
				pixels[i] = rgb_clamp(illum);
				break;
			case TRIANGLE:
				triangle *t = (triangle_t *)(tr.shape);
				break;
			case NONE:
				pixels[i] = c->bkgcolor;
				break;
		}
	}
}

/*
* Write contents of pixel array to file
* buf: output file
* img: pixel array in row major order
* c: struct containing configuration
*/
void write_image(FILE *buf, vec3_t *img, config_t *c) {
	int total = c->width * c->height;
	int i = 0;
	// 4 pixels per row
	for (; i < total; i += 4) {
		// allocate memory for formatted strings
		char strs[4][12];
		for (int j = 0; j < 4; j++) {
			rgb_str(strs[j], img + i + j);
		}
		fprintf(buf, "%s  %s  %s  %s\n",
		  		strs[0], strs[1], strs[2], strs[3]);
		if (ferror(buf)) {
			return;
		}
	}
	for (; i < total; i++) {
	  	char str[12];
		rgb_str(str, img + i);
		fprintf(buf, "%s  ", str);
		if (ferror(buf)) {
			return;
		}
	}
}

int main(int argc, char** argv) {
	// throw error if program isn't called with one parameter
	if (argc != 2) {
		printf("Please use a text file containing the following format as a parameter:\n");
		printf("imsize {width} {height}\n");
		return 1;
	}

	FILE *f = fopen(argv[1], "r"); 
	if (!f) {
		perror("Error opening file");
		return 1;
	}
	
	config_t *conf = malloc(sizeof(config_t));
	
	// read config file and fill in conf accordingly
	if (generate_config(f, conf) == -1) {
		fclose(f);
		free_config(conf);
		return 1;
	}
	if (fclose(f) == EOF) {
		perror("Error closing description file");
		free_config(conf);
		return 1;
	}
	
	// print out config to confirm that everything is correct
	// print_config(conf);

	vec3_t *pixels = malloc(conf->width * conf->height * sizeof(vec3_t));

	// calculate the rgb values for the pixels
	generate_image(pixels, conf);
	
	// add .ppm to the end of the filename and open new file
	char img[strlen(argv[1]) + 5];
	strcpy(img, argv[1]);
	strcat(img, ".ppm");
	FILE *buf = fopen(img, "w");
	if (!buf) {
		perror("Could not open output file");
		free(pixels);
		free_config(conf);
		return 1;
	}
	
	// write header to output file
	fprintf(buf, "P3\n# Created by Elena's PPM image generation machine v1.3\n%d %d\n255\n", conf->width, conf->height);
	if (ferror(buf)) {
		printf("Error writing to file %s\n", img);
		fclose(buf);
		free(pixels);
		free_config(conf);
		return 1;
	}
	
	// write pixel array to buffer
	write_image(buf, pixels, conf);
	if (ferror(buf)) {
		printf("Error writing to file %s\n", img);
		fclose(buf);
		free(pixels);
		free_config(conf);
		return 1;
	}
	
	free(pixels);

	int ret_val = 0;
	if (fclose(buf) == EOF) {
		perror ("Error closing output file");
		ret_val = 1;
	}
	
	free_config(conf);

	return ret_val;
}

