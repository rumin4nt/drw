

#ifndef d_font_ftgl_h_
#define d_font_ftgl_h_

#include <r4/src/r4_platform.h>

#ifndef RPLATFORM_WIN

typedef enum {
	RFontJustificationLeft,
	RFontJustificationCenter,
	RFontJustificationRight
} FontJustification;

void   d_font_set_align(int);
void   d_font_init(void);
void   d_font_deinit(void);
int    d_font_load(const char*);
void   d_font_draw(const char*);
void   d_font_size(int, int);
void   d_font_get_bbox(const char*, int, float*);
double d_font_get_width(const char*);

#endif

#endif
