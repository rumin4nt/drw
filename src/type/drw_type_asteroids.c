//
//  drw_type_asteroids.c
//  drw
//
//  Created by vs on 4/27/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_type_asteroids.h"

#ifdef DRW_TYPE_PROVIDER_ENABLE_ASTEROIDS

#include "ctype.h"
#include <fonts/asteroids_font.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DRW_EXT_R4
void* drw_type_asteroids_render(const char* text);
#endif

void drw_type_asteroids_initialize(void)
{
#ifdef DRW_EXT_R4
	drw_type_provider_register("asteroids", drw_type_asteroids_draw, drw_type_asteroids_bbox, drw_type_asteroids_render);
#else
	drw_type_provider_register("asteroids", drw_type_asteroids_draw, drw_type_asteroids_bbox);

#endif
}

void drw_type_asteroids_init(void)
{

	//ummm guess we don't need this?
}

void drw_type_asteroids_bbox(const char* text, unsigned long idk, float* address)
{
	
	address[0] = 0;
	address[1] = 0;
	address[2] = 0;
	address[3] = idk*12;
	address[4] = 16;
	address[5] = 0;
	

}

#define SIZE 1
#include <drw/drw.h>

static void ast_char(char c, int ox, int oy)
{
	const uint8_t* const pts	 = asteroids_font[c - ' '].points;
	int		     next_moveto = 1;
	int		     size	= 1;
	int		     x		 = ox;
	int		     y		 = oy;
	for (int i = 0; i < 8; i++)
	{
		uint8_t delta = pts[i];
		if (delta == FONT_LAST)
			break;
		if (delta == FONT_UP)
		{
			next_moveto = 1;
			continue;
		}

		unsigned dx = ((delta >> 4) & 0xF) * size;
		unsigned dy = ((delta >> 0) & 0xF) * size;

		if (next_moveto)
		{

			x = ox + dx;
			y = oy + dy;
			//moveto(x + dx, y + dy);
		}
		else
		{
			double nx = ox + dx;
			double ny = oy + dy;
			drw_line(x, y, nx, ny);
			x = ox + dx;
			y = oy + dy;
		}
		next_moveto = 0;
	}
}

static void ast_char_render(char c, int ox, int oy, RObject* obj)
{
	const uint8_t* const pts	 = asteroids_font[c - ' '].points;
	int		     next_moveto = 1;
	int		     size	= 1;
	int		     x		 = ox;
	int		     y		 = oy;

	//printf("drawing char: %c\n", c);

	//RLine* line = r_line_create();
	RLine* line = NULL;

	for (int i = 0; i < 8; i++)
	{

		uint8_t delta = pts[i];
		if (delta == FONT_LAST)
		{
			//	we do have one potential blank, ' '
			if (line)
				r_object_add_line(obj, line);
			line = NULL;
			break;
		}
		if (delta == FONT_UP)
		{
			next_moveto = 1;
			r_object_add_line(obj, line);
			line = NULL;
			continue;
		}

		unsigned dx = ((delta >> 4) & 0xF) * size;
		unsigned dy = ((delta >> 0) & 0xF) * size;

		if (next_moveto)
		{

			x = ox + dx;
			y = oy + dy;

			if (!line)
			{
				line = r_line_create();
			}
			r_line_add_point2f(line, x, y);
		}
		else
		{
			double nx = ox + dx;
			double ny = oy + dy;
			//hilarious mistake
			//r_line_add_point2f(line, x, y);
			//r_line_add_point2f(line, ox+dx, oy+dy);

			//drw_line(x, y, nx, ny);
			//if ( !line )
			//	line = r_line_create();
			//r_line_add_point2f(line, x, y);
			r_line_add_point2f(line, nx, ny);
			//x = ox + dx;
			//y = oy + dy;
		}
		next_moveto = 0;
	}

	if (line)
	{
		r_object_add_line(obj, line);
	}
}

static double get_real_sz()
{
	double x = 12; //asteroids constant
	//double   y = HPVEC_GRID_SIZE;
	unsigned w, h;

	if (!drw_screenspace_get())
	{
		drw_screensize_get(&w, &h);
		x /= w;
		x *= .5;
	}
	return x;
}

void drw_type_asteroids_draw(const char* text)
{
	double dpi = drw_query_dpi();
	//drw_push();
	
	
	//if ( dpi != 1 )
	//	drw_scale_u(dpi);
	
	
	
	//drw_scale_u(.01 * .5);
	uint8_t n = strlen(text);

	for (size_t i = 0; i < n; i++)
	{
		int    y     = 0;
		double x     = i * 12;
		char   upper = toupper(text[i]);
		ast_char(upper, x, y);
	}
	//drw_pop();
}

void* drw_type_asteroids_render(const char* text)
{
	RObject*      obj = r_object_create();
	unsigned long n   = strlen(text);

	for (size_t i = 0; i < n; i++)
	{
		int    y     = 0;
		double x     = i * 12;
		char   upper = toupper(text[i]);

		ast_char_render(upper, x, y, obj);
		drw_push();
		drw_scale_u(.01);
		ast_char(upper, x, y);
		drw_pop();
	}

	double r = get_real_sz();
	r_object_scale(obj, r, r);
	return obj;
}

#endif
