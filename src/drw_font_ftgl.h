

#ifndef drw_font_ftgl_h_
#define drw_font_ftgl_h_

#include <r4/src/r4_platform.h>

#ifndef RPLATFORM_WIN

typedef enum {
	RFontJustificationLeft,
	RFontJustificationCenter,
	RFontJustificationRight
} FontJustification;

void   drw_font_set_align(int);
void   drw_font_init(void);
void   drw_font_deinit(void);
int    drw_font_load(const char*);
void   drw_font_draw(const char*);
void   drw_font_size(int, int);
void   drw_font_get_bbox(const char*, int, float*);
double drw_font_get_width(const char*);

#endif

#endif
