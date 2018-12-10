

#ifndef drw_type_ftgl_h_
#define drw_type_ftgl_h_

#include "../drw_config.h"
#include "../drw_platform.h"

#ifndef DRW_PLATFORM_WIN

#ifdef DRW_TEXT_PROVIDER_ENABLE_FTGL
void drw_type_ftgl_initialize(void);
#endif

//	this will all go away
void drw_type_init(void);
void drw_type_deinit(void);
int  drw_type_ftgl_load(const char*);
void drw_type_draw(const char*);
void drw_type_size(int, int);

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
