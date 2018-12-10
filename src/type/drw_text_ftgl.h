

#ifndef drw_text_ftgl_h_
#define drw_text_ftgl_h_

#include "../drw_platform.h"

#ifndef DRW_PLATFORM_WIN

#define DRW_TEXT_PROVIDER_ENABLE_FTGL



#ifdef DRW_TEXT_PROVIDER_ENABLE_FTGL
void drw_text_ftgl_initialize(void);
#endif

//	this will all go away
void   drw_text_init(void);
void   drw_text_deinit(void);
int    drw_text_ftgl_load(const char*);
void   drw_text_draw(const char*);
void   drw_text_size(int, int);




/*
 typedef enum
 {
 RFontJustificationLeft,
 RFontJustificationCenter,
 RFontJustificationRight
 } FontJustification;
 */


#endif

#endif
