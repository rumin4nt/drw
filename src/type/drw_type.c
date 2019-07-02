//
//  drw_type.c
//  drw
//
//  Created by vs on 12/10/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_type.h"
//#ifdef DEBUG
#include "../drw_log.h"
//#endif

//#include "../drw_log.h"
#include "../drw_config.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef DRW_TYPE_PROVIDER_ENABLE_FTGLES
#include "drw_type_ftgles.h"
#endif

#ifndef DRW_PLATFORM_IOS
#include "drw_type_ftgl.h"
#endif

//#define DRW_TYPE_PROVIDER_ENABLE_ASTEROIDS

#include "drw_type_asteroids.h"
#ifdef DRW_TYPE_PROVIDER_ENABLE_HERSHEY
#include "drw_type_hershey.h"
#endif

#include "drw_type_hpvec.h"
#define TEXT_MAX 1024
#define PROVIDER_ID_MAX 32
#include <stdarg.h>
#include <string.h>

#include <drw/drw.h>

int		   drw_type_debug = 0;
static int	 align_x	= 0;
static int	 align_y	= 0;
static int	 type_provider  = -1;
static int	 num_providers  = 0;
drw_type_draw_fun* draw_funcs     = NULL;
drw_type_bbox_fun* bbox_funcs     = NULL;

#ifdef DRW_EXT_R4
drw_type_render_fun* render_funcs = NULL;
#endif

static int   _text_size;
const char** provider_ids = NULL;

static bool right_to_left = false;

void drw_type_set_righttoleft(bool v)
{
	right_to_left = v;
}

void drw_type_provider_select(signed index)
{
	if (index < 0 || index >= num_providers)
#ifdef DEBUG
		drw_log("Invalid font provider requested: %d [%d] \n", index, num_providers);
#else
		return;
#endif
	type_provider = index;
}

void drw_type_debug_set(int v)
{
	drw_type_debug = v;
}

void drw_type_init(void)
{

#ifdef DRW_TYPE_PROVIDER_ENABLE_FTGLES
	drw_type_ftgles_initialize();
#endif

#ifdef DRW_TYPE_PROVIDER_ENABLE_FTGL
	drw_type_ftgl_initialize();
#endif

#ifdef DRW_TYPE_PROVIDER_ENABLE_HERSHEY
	drw_type_hershey_initialize();
#endif

#ifdef DRW_TYPE_PROVIDER_ENABLE_HPVEC
	drw_type_hpvec_initialize();
#endif

#ifdef DRW_TYPE_PROVIDER_ENABLE_ASTEROIDS
	drw_type_asteroids_initialize();
#endif

	int n = drw_type_provider_count();
	if (n == 0) {
		drw_log("NO type providers...provided!");
		return;
	}

#ifdef RPLATFORM_IOS
	drw_type_provider_select(0);
#else
	drw_type_provider_select(0);

#endif
	
	drw_type_set_align(DRW_TYPE_ALIGN_H_RIGHT, DRW_TYPE_ALIGN_V_BOTTOM);
}

void drw_type_deinit(void)
{
	free(draw_funcs);
	free(bbox_funcs);
#ifdef DRW_EXT_R4
	if (render_funcs)
		free(render_funcs);
#endif
	free(provider_ids);
}

void drw_type_get_align(int* x, int* y)
{

	*x = align_x;
	*y = align_y;
}

void drw_type_set_align(int x, int y)
{
	int tx, ty = -1;

	if (x > -1 && x < DRW_TYPE_ALIGN_H_NONE) {
		align_x = x;
	}
#ifdef DEBUG
	else {
		drw_log("invalid horizontal type alignment requested!\n");
	}
#endif

	if (y > -1 && y < DRW_TYPE_ALIGN_V_NONE) {
		align_y = y;
	}
#ifdef DEBUG
	else {
		drw_log("invalid vertical type alignment requested!\n");
	}
#endif

	/*
#ifdef DEBUG
	if ( x < 0 || x > 2 || y < 0 || y > 2 )
	{
		printf("Invalid alignment selected!\n");
		return;
	}
#endif

	align_x = x;
	align_y = y;
*/
}

int drw_type_get_size(void)
{
	return _text_size;
}

void drw_type_set_size(int sz, int resolution)
{
#ifdef DRW_ENABLE_FTGLES
	drw_type_ftgles_size(sz, resolution);
#endif

#ifdef DRW_ENABLE_FTGL
	drw_type_ftgl_size(sz, resolution);
#endif
	_text_size = sz;
}
#ifdef DRW_EXT_R4

int drw_type_provider_register(const char* ident, drw_type_draw_fun draw, drw_type_bbox_fun bbox, drw_type_render_fun* render)
#else
int drw_type_provider_register(const char* ident, drw_type_draw_fun draw, drw_type_bbox_fun bbox)
#endif
{
	drw_log("registering provider: %s", ident);

	num_providers++;
	if (provider_ids == NULL) {
		provider_ids = calloc(num_providers, PROVIDER_ID_MAX * sizeof(char*));
		draw_funcs   = calloc(num_providers, sizeof(drw_type_draw_fun));
		bbox_funcs   = calloc(num_providers, sizeof(drw_type_bbox_fun));
#ifdef DRW_EXT_R4
		render_funcs = calloc(num_providers, sizeof(drw_type_render_fun));

#endif

	} else {
		provider_ids = realloc(provider_ids, num_providers * PROVIDER_ID_MAX * sizeof(char*));
		draw_funcs   = realloc(draw_funcs, num_providers * sizeof(drw_type_draw_fun));
		bbox_funcs   = realloc(bbox_funcs, num_providers * sizeof(drw_type_bbox_fun));
#ifdef DRW_EXT_R4

		render_funcs = realloc(render_funcs, num_providers * sizeof(drw_type_render_fun));
#endif
	}

	int idx		  = num_providers - 1;
	provider_ids[idx] = ident;
	draw_funcs[idx]   = draw;
	bbox_funcs[idx]   = bbox;
#ifdef DRW_EXT_R4
	render_funcs[idx] = render;
#endif

	return 0;
}

void drw_type_get_bbox(const char* text, unsigned long sz, float* bounds)
{
	if (type_provider < -1) {
		drw_log("NO type provider specified, return.");
		return;
	}

	if (num_providers == 0) {
		drw_log("NO providers registered, return.");
		return;
	}

	if (type_provider >= num_providers) {
		drw_log("Requested out of range type provider.\n");
		return;
	}

	drw_type_bbox_fun fun = bbox_funcs[type_provider];
	(*fun)(text, sz, bounds);
}

void drw_type_draw(const char* format, ...)
{
	if (type_provider == -1) {
		drw_log("NO type provider specified, return.");
	}

	if (num_providers == 0) {
		drw_log("NO providers registered, return.");
	}

	char buf[TEXT_MAX];
	memset(buf, 0, sizeof(strlen(buf)));
	sprintf(buf, "%s", format);
	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	float bounds[6];
	bounds[0] = bounds[1] = bounds[2] = bounds[3] = bounds[4] = bounds[5] = -1;
	drw_type_get_bbox(buf, strlen(buf), bounds);
	//if ( align_x)
	double wx = bounds[3] - bounds[0];
	double wy = bounds[4] - bounds[1];
	//double wz = bounds[5] - bounds[2];

	drw_type_draw_fun fun = draw_funcs[type_provider];
	double		  tx = 0, ty = 0;

	//
	switch (align_x) {
	case DRW_TYPE_ALIGN_H_LEFT:
		tx = wx * -1;
		break;
	case DRW_TYPE_ALIGN_H_CENTER:
		tx = wx * -.5;
		break;
	case DRW_TYPE_ALIGN_H_RIGHT:
		tx = wx * 0;
		break;
	default:
		break;
	}

	switch (align_y) {
	case DRW_TYPE_ALIGN_V_TOP:
		ty = wy * -.1;
		break;
	case DRW_TYPE_ALIGN_V_CENTER:
		ty = wy * -.5;
		break;
	case DRW_TYPE_ALIGN_V_BOTTOM:
		ty = wy * 0;
		break;
	default:
		break;
	}

	drw_push();
	drw_translate2f(tx, ty);
	fun(buf);
	drw_pop();
}

void drw_type_draw_fixed(const char* text)
{
	if (type_provider == -1) {
		drw_log("NO type provider specified, return.");
	}

	if (num_providers == 0) {
		drw_log("NO providers registered, return.");
	}

	float bounds[6];
	bounds[0] = bounds[1] = bounds[2] = bounds[3] = bounds[4] = bounds[5] = -1;
	drw_type_get_bbox(text, strlen(text), bounds);
	//if ( align_x)
	double wx = bounds[3] - bounds[0];
	double wy = bounds[4] - bounds[1];
	//double wz = bounds[5] - bounds[2];

	drw_type_draw_fun fun = draw_funcs[type_provider];
	double		  tx = 0, ty = 0;

	//
	switch (align_x) {
	case DRW_TYPE_ALIGN_H_LEFT:
		tx = wx * -1;
		break;
	case DRW_TYPE_ALIGN_H_CENTER:
		tx = wx * -.5;
		break;
	case DRW_TYPE_ALIGN_H_RIGHT:
		tx = wx * 0;
		break;
	default:
		break;
	}

	switch (align_y) {
	case DRW_TYPE_ALIGN_V_TOP:
		ty = wy * -.1;
		break;
	case DRW_TYPE_ALIGN_V_CENTER:
		ty = wy * -.5;
		break;
	case DRW_TYPE_ALIGN_V_BOTTOM:
		ty = wy * 0;
		break;
	default:
		break;
	}

	drw_push();
	drw_translate2f(tx, ty);
	fun(text);
	drw_pop();
}

void drw_type_load_ttf(const char* path)
{
#ifdef DRW_TYPE_PROVIDER_ENABLE_FTGLES

	drw_type_ftgles_load(path);
#endif

#ifdef DRW_TYPE_PROVIDER_ENABLE_FTGL
	drw_type_ftgl_load(path);
#endif
}

double drw_type_get_width(const char* w)
{
	//#ifdef DRW_PLATFORM_IOS
	//	return drw_type_ftgl_get

	//	OOPS
	return -1;
}

int drw_type_provider_count(void)
{
	return num_providers;
}

#ifdef DRW_EXT_R4

void* drw_type_render(const char* text)
{
	if (type_provider < -1) {
		drw_log("NO type provider specified, return.");
		return NULL;
	}

	if (num_providers == 0) {
		drw_log("NO providers registered, return.");
		return NULL;
	}

	if (type_provider >= num_providers) {
		drw_log("Requested out of range type provider.\n");
		return NULL;
	}

	drw_type_render_fun fun      = render_funcs[type_provider];
	if ( !fun )
	{
		printf("No rendering method provided.\n");
		return NULL;
	}
	void*		    rendered = (*fun)(text);
	return rendered;
}

#endif
