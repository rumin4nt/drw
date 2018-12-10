

#include "drw_text_ftgl.h"

//#include <r4/r4.h>

#ifdef RPLATFORM_WIN
#include "stdafx.h"
#endif

#include "../../drw.h"

#ifndef RPLATFORM_WIN

#include <FTGL/ftgl.h>
#include <assert.h>

// LL#include "../ui/r_sizing.h"
//#include "../core/r_app.h"

// extern AppSettings app_settings;

FTGLfont* font		= NULL;
int       justification = 0;

void drw_text_init()
{

	// ummm guess we don't need this?
}

void drw_text_deinit()
{
	if (font)
	{
		ftglDestroyFont(font);
	}
	font = NULL;
}

void drw_text_size(int sz, int resolution)
{
	//todo - eliminate this from public and force all font size through bottleneck
	//	interface in drw.c
	//
	//	for now we just call it on behalf of the user (me)

	drw_text_size_set(sz);

	if (!font)
	{
		return;
	}

	if (sz == 0)
	{
		printf("Uhh wtf font size was 0\n");
		sz = 18;
	}
	// printf("Setting font size: %d %d\n", sz, resolution);

	ftglSetFontFaceSize(font, sz, resolution);
	// int res = ftglSetFontFaceSize(font, sz, resolution);
	// printf("res was %d\n", res );
}

int drw_text_ftgl_load(const char* path)
{
	assert(path);

	// if ( font )
	//{
	//	ftglDestroyFont(font);
	//}
	//font = ftglCreateTextureFont(path);
	font = ftglCreatePolygonFont(path);

	if (!font)
		return -1;

	// drw_text_size(R_UI_FONT_SIZE * app_settings.scale_retina );

	drw_text_size(D_FONT_SIZE, 144);
	return 0;
}

void drw_text_draw(const char* str)
{

	if (!font)
	{
		//	logging belongs to the application space
		// l_warning("Error, no font!\n");
		printf("Error, no font!\n");
		return;
	}
	// if ( !font )
	//{
	//	drw_text_load("data/ttf/terminus.ttf");
	//}

	if (drw_get_screenspace())
	{
		//ftglRenderFont(font, str, FTGL_RENDER_ALL);
		ftglRenderFont(font, str, FTGL_RENDER_SIDE);
	}
	else
	{
		drw_push();
		int w, h;
		drw_query_framebuffer(&w, &h);
		int    sz   = drw_text_get_size();
		double dpi  = drw_query_retina();
		double mult = h;
		double frac = 1.0 / mult;
		drw_scale_u(frac);
		//ftglRenderFont(font, str, FTGL_RENDER_ALL);
		ftglRenderFont(font, str, FTGL_RENDER_SIDE);

		drw_pop();
	}

	// printf( "%d\n", ftglGetFontFaceSize(font) );
}

void drw_text_get_bbox(const char* str, unsigned int num, float* data)
{
	ftglGetFontBBox(font, str, num, data);
}

double drw_text_get_width(const char* str)
{
	int  count = 0;
	bool done  = false;

	// could probably ditch done here and merely do
	// while() etc
	while (!done)
	{
		char c = str[count];
		if (c == '\0')
		{
			done = true;
			break;
		}
		count++;
	}

	float* bounds = calloc(6, sizeof(float));
	for (int j = 0; j < 6; j++)
	{
		bounds[j] = 0;
	}
	drw_text_get_bbox(str, count, bounds);
	double v = bounds[3] - bounds[0];
	free(bounds);
	return v;
}

#endif
