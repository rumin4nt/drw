

#include <r4/src/r4_config.h>

#ifdef RPLATFORM_IOS

#include "drw_font_ftgles.h"

#include "../drw.h"
#include "FTGL/ftgles.h"

static FTPolygonFont* font  = 0;
static FTTextureFont* tfont = 0;
//static FTGLfont *font = NULL;
//void* font = 0;

void drw_font_init()
{

	//ummm guess we don't need this?
}

void drw_font_size(int sz, int garbage)
{
	if (!font)
	{
		printf("Can't set font size, font is NULL\n");
		return;
	}
	font->FaceSize(sz);
}

void drw_font_deinit()
{
	if (font)
	{
		//FTGL::ftglDestroyFont(font);
	}
	font = 0;
}

int drw_font_load(const char* path)
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

extern "C"
{
	void drw_font_draw(const char* str)
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
		font->Render(str);
		//tfont->Render(str);
	}
}

void drw_font_get_bbox(const char* str, int num, float* data)
{
	font->BBox(str, data[0], data[1], data[2], data[3], data[4], data[5]);

	//ftglGetFontBBox(font, str, num, data);
}

#endif
