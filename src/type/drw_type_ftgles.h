

#ifndef r_font_ftgles_h_
#define r_font_ftgles_h_

//#ifdef R4_ENABLE_FREETYPE
#include "../drw_config.h"

#ifdef DRW_PLATFORM_IOS

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef DRW_TYPE_PROVIDER_ENABLE_FTGLES
	void drw_type_ftgles_initialize(void);
#endif

#include <r4/src/r4_config.h>

	//	all this too shall one day be gone
	void drw_type_ftgles_init(void);
	void drw_type_ftgles_deinit(void);
	int  drw_type_ftgles_load(const char*);
	void drw_type_ftgles_draw(const char*);
	void drw_type_ftgles_size(int, int);
	void drw_type_ftgles_bbox(const char*, unsigned long, float*);
	void drw_type_ftgles_set_alignment(int horiz, int vert);

#ifdef __cplusplus
}
#endif

#endif

#endif
