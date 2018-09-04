

#ifndef drw_text_ftgl_h_
#define drw_text_ftgl_h_

#include "../drw_platform.h"

#ifndef DRW_PLATFORM_WIN

typedef enum
{
	RFontJustificationLeft,
	RFontJustificationCenter,
	RFontJustificationRight
} FontJustification;

void   drw_text_set_align(int);
void   drw_text_init(void);
void   drw_text_deinit(void);
int    drw_text_ftgl_load(const char*);
void   drw_text_draw(const char*);
void   drw_text_size(int, int);
void   drw_text_get_bbox(const char*, int, float*);
double drw_text_get_width(const char*);
int    drw_text_get_fontsize(void);

#endif

#endif
