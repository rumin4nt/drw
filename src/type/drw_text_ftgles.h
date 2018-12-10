

#ifndef r_font_ftgles_h_
#define r_font_ftgles_h_

//#ifdef R4_ENABLE_FREETYPE

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef DRW_TEXT_PROVIDER_ENABLE_FTGLES
	void drw_text_ftgles_initialize(void);
#endif

#include <r4/src/r4_config.h>

	//	all this too shall one day be gone
	void drw_text_init(void);
	void drw_text_deinit(void);
	int  drw_text_ftgles_load(const char*);
	void drw_text_draw(const char*);
	void drw_text_size(int, int);
	void drw_text_get_bbox(const char*, unsigned int, float*);
	void drw_text_set_alignment(int horiz, int vert);

#ifdef __cplusplus
}
#endif

//#endif

#endif
