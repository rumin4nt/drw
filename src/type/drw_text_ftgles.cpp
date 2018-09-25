

extern "C"
{
#include <drw/drw.h>
}

#include "../drw_config.h"

#ifdef DRW_PLATFORM_IOS

#include "drw_text_ftgles.h"

#include "FTGL/ftgles.h"

static FTPolygonFont* font  = 0;
static FTTextureFont* tfont = 0;
//static FTGLfont *font = NULL;
//void* font = 0;

void drw_text_init()
{

	//ummm guess we don't need this?
}

void drw_text_size(int sz, int garbage)
{
	if (!font)
	{
		printf("Can't set font size, font is NULL\n");
		return;
	}
	font->FaceSize(sz);
}

void drw_text_deinit()
{
	if (font)
	{
		//FTGL::ftglDestroyFont(font);
	}
	font = 0;
}

int drw_text_ftgles_load(const char* path)
{
	printf("loading ftgles font %s.\n", path);
	if (font)
	{
		//ftglDestroyFont(font);
	}

	if (!path)
	{
		printf("Can't load font with an invalid path.\n");
		return 1;
	}
	//if(!font)
	//  return -1;

	font = new FTPolygonFont(path);
	//font = new FTTextureFont(path);

	//if ( font->err != 0 ){

	//  return 1;

	//   }
	if (!font)
	{
		printf("Font failed or did not load.\n");
		return -7;
	}
	font->FaceSize(D_FONT_SIZE);
	//tfont->FaceSize(72);

	//font = ftglCreatePolygonFont(path);
	//FTGLPixmapFont font("Arial.ttf");
	//ftglSetFontFaceSize(font, 72, 72);
	//ftglRenderFont(font, "Hello World!", FTGL_RENDER_ALL);

	//ftglDestroyFont(font);

	return 0;
}

//extern "C"
//{
void drw_text_draw(const char* str)
{
	if (!str)
	{
		printf("Tried to draw a NULL string\n");
		return;
	}
	if (!font)
	{
		printf("Error, font not loaded!\n");
		return;
	}

	/*
		 if ( drw_get_screenspace() )
		 {
		 ftglRenderFont(font, str, FTGL_RENDER_ALL);

		 }else{
		 drw_push();
		 int w, h;
		 drw_query_framebuffer(&w, &h);
		 int sz = drw_text_get_size();
		 double dpi = drw_query_retina();
		 double mult = h;
		 double frac = 1.0 / mult;
		 drw_scale_u(frac);
		 ftglRenderFont(font, str, FTGL_RENDER_ALL);

		 drw_pop();
		 }
		 */
	if (drw_get_screenspace())
	{
		font->Render(str);
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
		font->Render(str);

		drw_pop();
	}
	//tfont->Render(str);
}
//}

void drw_text_get_bbox(const char* str, int num, float* data)
{
	font->BBox(str, data[0], data[1], data[2], data[3], data[4], data[5]);

	//ftglGetFontBBox(font, str, num, data);
}

#endif