//
//  drw_type_hpvec.c
//  drw
//
//  Created by vs on 4/27/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_type_hpvec.h"
//#include "../../drw.h"

#ifdef DRW_TYPE_PROVIDER_ENABLE_HPVEC
//#error fuck
#include <hp/hp1345a.c>

#include <stdbool.h>

//#ifdef DEBUG
#include "../drw_log.h"
//#include <stdio.h>

#include "../drw_config.h"
#include "../drw_render_gl.h"

//#endif

//#include "drw_type.h"

#include <string.h>

/*

 	courtesy of http://phk.freebsd.dk/hacks/Wargames/


 */

// 	???
#define HPVEC_GRID_SIZE 16
//static int alignment_h = DRW_TYPE_ALIGN_H_CENTER;
//static int alignment_v = DRW_TYPE_ALIGN_V_CENTER;

static double get_real_sz()
{
	double x = HPVEC_GRID_SIZE;
	//double   y = HPVEC_GRID_SIZE;
	unsigned w, h;

	if (!drw_screenspace_get()) {
		drw_screensize_get(&w, &h);
		x /= w;
		//y /= w;
		//		x *= .5;
		//		y *= .5;
	}
	return x;
}

void drw_type_hpvec_bbox(const char* text, unsigned long sz, float* data)
{
	double   x = HPVEC_GRID_SIZE * sz;
	double   y = HPVEC_GRID_SIZE;
	unsigned w, h;

	if (!drw_screenspace_get()) {
		drw_screensize_get(&w, &h);
		x /= w;
		y /= w;
		//		x *= .5;
		//		y *= .5;
	}
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = x;
	data[4] = y;
	data[5] = 0;
}

void drw_type_hpvec_draw(const char* text);

void drw_type_hpvec_initialize(void)
{
#ifdef DRW_EXT_R4
	drw_type_provider_register("hpvec", drw_type_hpvec_draw, drw_type_hpvec_bbox, drw_type_hpvec_render);

#else
	drw_type_provider_register("hpvec", drw_type_hpvec_draw, drw_type_hpvec_bbox);

#endif
}

/*
void drw_type_hpvec_set_alignment(int h, int v)
{
	if (h != -1)
		alignment_h = h;
	if (v != -1)
		alignment_v = v;
}
*/

static void draw_hp_glyph(int idx)
{
	if (idx < 0)
		return;

	const int* points = hp1345a[idx];
	int	num    = hp1345a_sizes[idx];
	if (num == 0)
		return;

	double sz = drw_query_dpi();
	sz *= .5;
	RLine* l = r_line_create();
	int    j;
	for (j = 0; j < num - 1; j += 2) {

        double x = points[j];
        double y = points[1+j];
		x *= sz;
		y *= sz;
		r_line_add_point2f(l, x, y);
	}

	drw_rline(l);
	r_line_destroy(l);
}

static void draw_debug(const char* text)
{
	float* dims = calloc(6, sizeof(float));

	drw_type_hpvec_bbox(text, strlen(text), dims);
	double wx = dims[3] - dims[0];
	double wy = dims[2] - dims[1];

	drw_rect(dims[0], dims[1], wx, wy);

	free(dims);
}

void drw_type_hpvec_draw(const char* text)
{
	if (!text || 0 == strcmp("", text)) {
#ifdef DEBUG
		drw_log("Error, no text to draw!");
#endif
		return;
	}
	if (drw_type_debug)
		draw_debug(text);

	bool done = false;
	int  i    = 0;

	int offx = 0;
	int offy = 0;

	unsigned long len = strlen(text);
	/*
	switch (alignment_h)
	{
	case DRW_TYPE_ALIGN_H_LEFT:
		break;
	case DRW_TYPE_ALIGN_H_CENTER:
		offx += HPVEC_FONT_SIZE * len * -.5;
		if (len % 2 == 0)
			offx += HPVEC_FONT_SIZE * .25 * .5; //.3333333333;
		break;
	case DRW_TYPE_ALIGN_H_RIGHT:
		offx += HPVEC_FONT_SIZE * len * -1;

		break;
	default:
		break;
	}

	switch (alignment_v)
	{
	case DRW_TYPE_ALIGN_V_TOP:
		offy = -HPVEC_FONT_SIZE;
		break;
	case DRW_TYPE_ALIGN_V_CENTER:
		offy = HPVEC_FONT_SIZE * -.5;
		offy += HPVEC_FONT_SIZE * .25 * -.5 * .5; //.3333333333;

		break;
	case DRW_TYPE_ALIGN_V_BOTTOM:
		break;

	default:
		break;
	}
*/
	drw_push();
	int w, h;

	if (!drw_screenspace_get()) {
		drw_query_framebuffer(&w, &h);

		double sz = drw_type_size_get();

		double sc = 1. / (h / sz * 8);
		sc *= .5;
		drw_scale_u(sc);
	}

	//drw_translate(offx, offy, 0);
	double sz = drw_query_dpi();
	sz *= .5;
	while (!done) {

		char c = text[i];
		if (c == '\0') {
			done = true;
		} else {
			int	 idx  = 0 + c;
			static bool once = false;
			if (!once) {
				once = true;
				printf("index [%c] %d\n", c, idx);
			}
			draw_hp_glyph(idx);
			drw_translate_x(HPVEC_FONT_SIZE * sz);
		}
		i++;
	}

	//	if ( !drw_screenspace_get() )
	////
	//{
	//	drw_pop();

	//}/
	drw_pop();
}
//static void draw_hp_glyph(int idx)

static HPGlyph* render_hp_glyph(unsigned long idx)
{
	//if (idx < 0)
	//	return NULL;

	const int* points = hp1345a[idx];
	int	num    = hp1345a_sizes[idx];
	if (num == 0)
		return NULL;

	HPGlyph* glyph = calloc(1, sizeof(HPGlyph));
	glyph->num     = num;
	glyph->points  = points;

	return glyph;
}

/*
void drw_type_hpvec_get_offset_scale(const char* text, double* ox, double* oy, double* scale)
{
	int offx = 0;
	int offy = 0;

	unsigned long len = strlen(text);

	switch (alignment_h) {
	case DRW_TYPE_ALIGN_H_LEFT:
		break;
	case DRW_TYPE_ALIGN_H_CENTER:
		offx += HPVEC_FONT_SIZE * len * -.5;
		if (len % 2 == 0)
			offx += HPVEC_FONT_SIZE * .25 * .5; //.3333333333;
		break;
	case DRW_TYPE_ALIGN_H_RIGHT:
		offx += HPVEC_FONT_SIZE * len * -1;

		break;
	default:
		break;
	}

	switch (alignment_v) {
	case DRW_TYPE_ALIGN_V_TOP:
		offy = -HPVEC_FONT_SIZE;
		break;
	case DRW_TYPE_ALIGN_V_CENTER:
		offy = HPVEC_FONT_SIZE * -.5;
		offy += HPVEC_FONT_SIZE * .25 * -.5 * .5; //.3333333333;

		break;
	case DRW_TYPE_ALIGN_V_BOTTOM:
		break;

	default:
		break;
	}

	drw_push();
	int w, h;
	*ox = offx;
	*oy = offy;

	if (!drw_screenspace_get()) {
		drw_query_framebuffer(&w, &h);

		double sz = drw_type_size_get();

		double sc = 1. / (h / sz * 8);

		*scale = sc;
		//drw_scale_u(sc);
	} else {
		*scale = 1;
	}
}
*/

HPGlyph** drw_type_hpvec_glyph(const char* text)
{
	unsigned long len = strlen(text);

	HPGlyph** glyphs = calloc(len, sizeof(HPGlyph*));

	for (unsigned long i = 0; i < len; i++) {
		glyphs[i] = render_hp_glyph(text[i]);
	}

	return glyphs;
}

#ifdef DRW_EXT_R4

//----------------------------------------------------------------
#pragma mark rline rendering

static RLine* render_hp_glyph_as_rline(int idx)
{
	if (idx < 0)
		return NULL;

	const int* points = hp1345a[idx];
	int	num    = hp1345a_sizes[idx];
	if (num == 0)
		return NULL;

	//double real = get_real_sz();

	RLine* l = r_line_create();
	int    j;
	for (j = 0; j < num - 1; j += 2) {

		r_line_add_point2f(l, points[j] , points[j + 1] );
	}

	return l;
}

void* drw_type_hpvec_render(const char* text)
{
	RObject* obj = r_object_create();

	unsigned long n = strlen(text);
	double real = get_real_sz();
	real /= 8;
	for (int i = 0; i < n; i++) {
		char c = text[i];

		RLine* line = render_hp_glyph_as_rline(text[i]);
		if (!line)
			continue;
		
		r_line_scale(line, real, real);
		r_line_translate(line, i * real * HPVEC_GRID_SIZE, 0);
		r_object_add_line(obj, line);
	}
	
	//r_object_scale(obj, real, real);
	//r_object_translate(obj, i * real, 0);
	return obj;
}
#endif

#endif
