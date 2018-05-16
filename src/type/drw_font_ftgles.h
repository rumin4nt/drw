

#ifndef r_font_ftgles_h_
#define r_font_ftgles_h_

//#ifdef R4_ENABLE_FREETYPE

#ifdef __cplusplus
extern "C"
{
#endif

#include <r4/src/r4_config.h>

	void drw_font_init(void);
	void drw_font_deinit(void);
	int  drw_font_load(const char*);
	void drw_font_draw(const char*);
	void drw_font_size(int, int);
	void drw_font_get_bbox(const char*, int, float*);
	void drw_font_set_alignment(int horiz, int vert);

#ifdef __cplusplus
}
#endif

//#endif

#endif
