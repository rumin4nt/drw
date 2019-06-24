//
//  drw_type_asteroids.c
//  drw
//
//  Created by vs on 4/27/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_type_asteroids.h"

#ifdef DRW_TYPE_PROVIDER_ENABLE_ASTEROIDS

#include <stdlib.h>

void drw_type_asteroids_initialize(void)
{
	drw_type_provider_register("asteroids", drw_type_asteroids_draw, drw_type_asteroids_bbox, NULL);
}

void drw_type_asteroids_init(void)
{

	//ummm guess we don't need this?
}

void drw_type_asteroids_bbox(const char* text, unsigned long idk, float* address)
{
}

void drw_type_asteroids_draw(const char* text)
{
}

void drw_type_asteroids(const char* text)
{

	//const uint8_t * const pts = a

	/*
	const uint8_t * const pts = asteroids_font[c - ' '].points;
	int next_moveto = 1;

	for(int i = 0 ; i < 8 ; i++)
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
			moveto(x + dx, y + dy);
		else
			lineto(x + dx, y + dy);

		next_moveto = 0;
	}

	return 12 * size;
	*/
}

#endif
