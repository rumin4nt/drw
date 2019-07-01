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
#include "../contrib/fonts/asteroids_font.h"
#include "ctype.h"
#include <stdio.h>

void drw_type_asteroids_initialize(void)
{
#ifdef DRW_EXT_R4
	drw_type_provider_register("asteroids", drw_type_asteroids_draw, drw_type_asteroids_bbox, NULL);
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
}

void drw_type_asteroids_draw(const char* text)
{
	int n = strlen(text);
	for ( size_t i = 0; i < n; i++ )
	{
		
		const char c = text[i];
		char up = toupper(c);
		int i = (int)up;
		//i -= 128;
		asteroids_char_t ch = asteroids_font[i];
		
		printf("ch : %d\n", i );
		bool done = false;
		int j = 0;
		bool alt = true;
		while (!done )
		{
			uint8_t v = ch.points[j];
			
			char x = v >> 4;
			char y = v >> 0;
			
			if ( v == FONT_LAST )
				done = true;
		
			alt = !alt;
			j++;
			if( j == 8)
				done = true;
		}
		
		
	}
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
