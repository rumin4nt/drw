

extern "C" {
#include <drw/drw.h>
}

#include "../drw_config.h"

#ifdef DRW_PLATFORM_IOS

#define DRW_TYPE_PROVIDER_ENABLE_FTGLES

#ifdef DRW_TYPE_PROVIDER_ENABLE_FTGLES


#include "drw_type_ftgles.h"
//#include "../drw_log.h"
#include "FTGL/ftgles.h"

static FTPolygonFont* font  = 0;
static FTTextureFont* tfont = 0;
//static FTGLfont *font = NULL;
//void* font = 0;

void drw_type_ftgles_initialize(void)
{
#ifdef DRW_EXT_R4
	drw_type_provider_register("ftgles", drw_type_ftgles_draw, drw_type_ftgles_bbox, NULL);

#else
	drw_type_provider_register("ftgles", drw_type_ftgles_draw, drw_type_ftgles_bbox);
#endif
	
}

void drw_type_ftgles_init()
{

	//ummm guess we don't need this?
}

void drw_type_ftgles_size(int sz, int garbage)
{
	if (!font)
	{
		printf("Can't set font size, font is NULL\n");
		return;
	}
	double dp = drw_query_dpi();
	font->FaceSize(sz * dp);
}

void drw_type_ftgles_deinit()
{
	if (font)
	{
		//FTGL::ftglDestroyFont(font);
	}
	font = 0;
}

int drw_type_ftgles_load(const char* path)
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
	double d = drw_query_dpi();
	font->FaceSize(D_FONT_SIZE * d);
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
void drw_type_ftgles_draw(const char* str)
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
		 if ( drw_screenspace_get() )
		 {
		 ftglRenderFont(font, str, FTGL_RENDER_ALL);

		 }else{
		 drw_push();
		 int w, h;
		 drw_query_framebuffer(&w, &h);
		 int sz = drw_type_get_size();
		 double dpi = drw_query_dpi();
		 double mult = h;
		 double frac = 1.0 / mult;
		 drw_scale_u(frac);
		 ftglRenderFont(font, str, FTGL_RENDER_ALL);

		 drw_pop();
		 }
		 */
	if (drw_screenspace_get())
	{
		font->Render(str);
	}
	else
	{

		drw_push();
		int w, h;
		drw_query_framebuffer(&w, &h);
		int    sz   = drw_type_get_size();
		double dpi  = drw_query_dpi();
		double mult = h;
		double frac = 1.0 / mult;
		drw_scale_u(frac);
		font->Render(str);

		drw_pop();
	}
	//tfont->Render(str);
}
//}

void drw_type_ftgles_bbox(const char* str, unsigned long num, float* data)
{
	if (!font)
	{
		printf("Error, bbox called on an unloaded font!\n");
		return;
	}
	font->BBox(str, data[0], data[1], data[2], data[3], data[4], data[5]);
	double sz = drw_query_dpi();
	
//	if ( sz == 1 )
//		return;
//	
//	data[0] *= sz;
//	data[1] *= sz;
//	data[2] *= sz;
//	data[3] *= sz;
//	data[4] *= sz;
//	data[5] *= sz;
	
	
	
	//ftglGetFontBBox(font, str, num, data);
}

#endif
#endif
