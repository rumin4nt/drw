//
//  drw_font_hpvec.c
//  drw
//
//  Created by vs on 4/27/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_font_hpvec.h"
#include "../../drw.h"

#include <hp/hp1345a.c>

#include <stdbool.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include <string.h>



static int alignment_h = DRW_FONT_ALIGN_H_CENTER;
static int alignment_v = DRW_FONT_ALIGN_V_CENTER;

void drw_font_hpvec_set_alignment(int h, int v)
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

	const unsigned long long renderLineSize = num * 2;

	GLfloat* arr = calloc(renderLineSize, sizeof(GLfloat));
	int      j;
	for (j = 0; j < num - 1; j += 2)
	{

		arr[j]     = points[j];
		arr[j + 1] = points[j + 1];
	}

	glVertexPointer(2, GL_FLOAT, 0, arr);
	glDrawArrays(GL_LINE_STRIP, 0, num);

	free(arr);
}

void drw_font_hpvec_draw(const char* text)
{
	bool done = false;
	int  i    = 0;

	int offx = 0;
	int offy = 0;

	unsigned long len = strlen(text);

	switch (alignment_h)
	{
	case DRW_FONT_ALIGN_H_LEFT:
		break;
	case DRW_FONT_ALIGN_H_CENTER:
		offx += HPVEC_FONT_SIZE * len * -.5;
		if (len % 2 == 0)
			offx += HPVEC_FONT_SIZE * .25 * .5; //.3333333333;
		break;
	case DRW_FONT_ALIGN_H_RIGHT:
		offx += HPVEC_FONT_SIZE * len * -1;

		break;
	default:
		break;
	}

	switch (alignment_v)
	{
	case DRW_FONT_ALIGN_V_TOP:
		offy = -HPVEC_FONT_SIZE;
		break;
	case DRW_FONT_ALIGN_V_CENTER:
		offy = HPVEC_FONT_SIZE * -.5;
		offy += HPVEC_FONT_SIZE * .25 * -.5 * .5; //.3333333333;

		break;
	case DRW_FONT_ALIGN_V_BOTTOM:
		break;

	default:
		break;
	}

	drw_push();
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

	drw_pop();
}
