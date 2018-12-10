//
//  drw_text_hpvec.c
//  drw
//
//  Created by vs on 4/27/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_text_hpvec.h"
#include "../../drw.h"

#include <hp/hp1345a.c>

#include <stdbool.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include <string.h>

/*

 	courtesy of http://phk.freebsd.dk/hacks/Wargames/


 */

#ifdef DRW_TEXT_PROVIDER_ENABLE_HPVEC

static int alignment_h = DRW_TEXT_ALIGN_H_CENTER;
static int alignment_v = DRW_TEXT_ALIGN_V_CENTER;

void drw_text_hpvec_set_alignment(int h, int v)
{
	if (h != -1)
		alignment_h = h;
	if (v != -1)
		alignment_v = v;
}

static void draw_hp_glyph(int idx)
{
	if (idx < 0)
		return;

	const int* points = hp1345a[idx];
	int	num    = hp1345a_sizes[idx];
	if (num == 0)
		return;

	RLine* l = r_line_create();
	int    j;
	for (j = 0; j < num - 1; j += 2)
	{

		r_line_add_point2f(l, points[j], points[j + 1]);
	}

	drw_rline(l);
	r_line_destroy(l);
}

void drw_text_hpvec_draw(const char* text)
{
	if (!text || 0 == strcmp("", text))
	{
		drw_log("Error, no text to draw!");
		return;
	}
	bool done = false;
	int  i    = 0;

	int offx = 0;
	int offy = 0;

	unsigned long len = strlen(text);

	switch (alignment_h)
	{
	case DRW_TEXT_ALIGN_H_LEFT:
		break;
	case DRW_TEXT_ALIGN_H_CENTER:
		offx += HPVEC_FONT_SIZE * len * -.5;
		if (len % 2 == 0)
			offx += HPVEC_FONT_SIZE * .25 * .5; //.3333333333;
		break;
	case DRW_TEXT_ALIGN_H_RIGHT:
		offx += HPVEC_FONT_SIZE * len * -1;

		break;
	default:
		break;
	}

	switch (alignment_v)
	{
	case DRW_TEXT_ALIGN_V_TOP:
		offy = -HPVEC_FONT_SIZE;
		break;
	case DRW_TEXT_ALIGN_V_CENTER:
		offy = HPVEC_FONT_SIZE * -.5;
		offy += HPVEC_FONT_SIZE * .25 * -.5 * .5; //.3333333333;

		break;
	case DRW_TEXT_ALIGN_V_BOTTOM:
		break;

	default:
		break;
	}

	drw_push();
	int w, h;

	if (!drw_get_screenspace())
	{
		drw_query_framebuffer(&w, &h);

		double sz = drw_text_size_get();

		double sc = 1. / (h / sz * 8);

		drw_scale_u(sc);
	}

	drw_translate(offx, offy, 0);

	while (!done)
	{

		char c = text[i];
		if (c == '\0')
		{
			done = true;
		}
		else
		{
			int	 idx  = 0 + c;
			static bool once = false;
			if (!once)
			{
				once = true;
				printf("index [%c] %d\n", c, idx);
			}
			draw_hp_glyph(idx);
			drw_translate_x(HPVEC_FONT_SIZE);
		}
		i++;
	}

	//	if ( !drw_get_screenspace() )
	////
	//{
	//	drw_pop();

	//}/
	drw_pop();
}
//static void draw_hp_glyph(int idx)

static HPGlyph* render_hp_glyph(unsigned long idx)
{
	if (idx < 0)
		return NULL;

	const int* points = hp1345a[idx];
	int	num    = hp1345a_sizes[idx];
	if (num == 0)
		return NULL;

	HPGlyph* glyph = calloc(1, sizeof(HPGlyph));
	glyph->num     = num;
	glyph->points  = points;

	return glyph;
}

void drw_text_hpvec_get_offset_scale(const char* text, double* ox, double* oy, double* scale)
{
	int offx = 0;
	int offy = 0;

	unsigned long len = strlen(text);

	switch (alignment_h)
	{
	case DRW_TEXT_ALIGN_H_LEFT:
		break;
	case DRW_TEXT_ALIGN_H_CENTER:
		offx += HPVEC_FONT_SIZE * len * -.5;
		if (len % 2 == 0)
			offx += HPVEC_FONT_SIZE * .25 * .5; //.3333333333;
		break;
	case DRW_TEXT_ALIGN_H_RIGHT:
		offx += HPVEC_FONT_SIZE * len * -1;

		break;
	default:
		break;
	}

	switch (alignment_v)
	{
	case DRW_TEXT_ALIGN_V_TOP:
		offy = -HPVEC_FONT_SIZE;
		break;
	case DRW_TEXT_ALIGN_V_CENTER:
		offy = HPVEC_FONT_SIZE * -.5;
		offy += HPVEC_FONT_SIZE * .25 * -.5 * .5; //.3333333333;

		break;
	case DRW_TEXT_ALIGN_V_BOTTOM:
		break;

	default:
		break;
	}

	drw_push();
	int w, h;
	*ox = offx;
	*oy = offy;

	if (!drw_get_screenspace())
	{
		drw_query_framebuffer(&w, &h);

		double sz = drw_text_size_get();

		double sc = 1. / (h / sz * 8);

		*scale = sc;
		//drw_scale_u(sc);
	}
	else
	{
		*scale = 1;
	}
}

HPGlyph** drw_text_hpvec_render(const char* text)
{
	unsigned long len = strlen(text);

	HPGlyph** glyphs = calloc(len, sizeof(HPGlyph));

	for (unsigned long i = 0; i < len; i++)
	{
		glyphs[i] = render_hp_glyph(text[i]);
	}

	return glyphs;
}

#endif
