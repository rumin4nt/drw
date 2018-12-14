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

#ifdef DRW_PLATFORM_IOS
#include "drw_type_ftgles.h"
#else
#include "drw_type_ftgl.h"
#endif

#define TEXT_MAX 1024
#define PROVIDER_ID_MAX 32
#include <stdarg.h>
#include <string.h>

#include <drw/drw.h>

static int	 align_x       = 0;
static int	 align_y       = 0;
static int	 type_provider = -1;
static int	 num_providers = 0;
drw_type_draw_fun* draw_funcs    = NULL;
drw_type_bbox_fun* bbox_funcs    = NULL;
static int	 _text_size;
const char**       provider_ids = NULL;

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

void drw_type_init(void)
{
#ifdef DRW_TYPE_PROVIDER_ENABLE_HPVEC
	drw_type_hpvec_initialize();
#endif

#ifdef DRW_TYPE_PROVIDER_ENABLE_FTGLES
	drw_type_ftgles_initialize();
#endif

#ifdef DRW_TYPE_PROVIDER_ENABLE_FTGL
	drw_type_ftgl_initialize();
#endif

	int n = drw_type_provider_count();
	if (n > 0)
		drw_type_provider_select(n - 1);

	drw_type_set_align(DRW_TYPE_ALIGN_H_RIGHT, DRW_TYPE_ALIGN_V_BOTTOM);
}

void drw_type_deinit(void)
{
	free(draw_funcs);
	free(bbox_funcs);
	free(provider_ids);
}

void drw_type_get_align(int* x, int* y)
{
	*x = align_x;
	*y = align_y;
}

void drw_type_set_align(int x, int y)
{
	align_x = x;
	align_y = y;
}
int drw_type_get_size(void)
{
	return _text_size;
}

void drw_type_set_size(int sz, int resolution)
{
#ifdef DRW_ENABLE_FTGLES

	drw_type_ftgles_size(sz, resolution);
#else
	drw_type_ftgl_size(sz, resolution);

#endif
	_text_size = sz;
}

int drw_type_provider_register(const char* ident, drw_type_draw_fun render, drw_type_bbox_fun bbox)
{
	drw_log("registering provider: %s", ident);

	num_providers++;
	if (provider_ids == NULL)
	{
		provider_ids = calloc(num_providers, PROVIDER_ID_MAX * sizeof(char));
		draw_funcs   = calloc(num_providers, sizeof(draw_funcs));
		bbox_funcs   = calloc(num_providers, sizeof(bbox_funcs));
	}
	else
	{
		provider_ids = realloc(provider_ids, num_providers * PROVIDER_ID_MAX * sizeof(char));
		draw_funcs   = realloc(draw_funcs, num_providers * sizeof(draw_funcs));
		bbox_funcs   = realloc(bbox_funcs, num_providers * sizeof(bbox_funcs));
	}

	int idx		  = num_providers - 1;
	provider_ids[idx] = ident;
	draw_funcs[idx]   = render;
	bbox_funcs[idx]   = bbox;
	return 0;
}

void drw_type_get_bbox(const char* text, unsigned long sz, float* bounds)
{
	if (type_provider == -1)
	{
		drw_log("NO type provider specified, return.");
		return;
	}
	if (num_providers == 0)
	{
		drw_log("NO providers registered, return.");
		return;
	}

	drw_type_bbox_fun fun = bbox_funcs[type_provider];
	(*fun)(text, sz, bounds);

	/*switch (type_provider) {
		case <#constant#>:
			<#statements#>
			break;

		default:
			break;
	}*/
}

void drw_type_draw(const char* format, ...)
{
	if (type_provider == -1)

	{
		drw_log("NO type provider specified, return.");
	}
	if (num_providers == 0)
	{
		drw_log("NO providers registered, return.");
	}
	//if (_text_provider_type == DRW_TYPE_PROVIDER_NONE)
	//	return;

	char buf[TEXT_MAX];
	sprintf(buf, "%s", format);
	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	float bounds[6];
	drw_type_get_bbox(buf, strlen(buf), bounds);
	//if ( align_x)
	double wx = bounds[3] - bounds[0];
	double wy = bounds[4] - bounds[1];
	double wz = bounds[5] - bounds[2];

	drw_type_draw_fun fun = *draw_funcs[type_provider];
	double		  tx = 0, ty = 0;

	switch (align_x)
	{
	case DRW_TYPE_ALIGN_H_LEFT:
		tx = wx * -1;
		break;
	case DRW_TYPE_ALIGN_H_CENTER:
		tx = wx * -.5;
		break;
	case DRW_TYPE_ALIGN_H_RIGHT:
		//tx = 0;
		break;
	default:
		break;
	}

	switch (align_y)
	{
	case DRW_TYPE_ALIGN_V_TOP:
		//ty = wy * -1;
		break;
	case DRW_TYPE_ALIGN_V_CENTER:
		ty = wy * -.5;
		break;
	case DRW_TYPE_ALIGN_V_BOTTOM:
		ty = wy * -1;
		break;
	default:
		break;
	}
	drw_push();
	drw_translate2f(tx, ty);
	fun(buf);
	drw_pop();

	//free(buf);
}

void drw_type_load_ttf(const char* path)
{
#ifdef DRW_PLATFORM_IOS
	drw_type_ftgles_load(path);
#else
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
