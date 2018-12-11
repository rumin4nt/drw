

#ifndef drw_type_ftgl_h_
#define drw_type_ftgl_h_

#include "../drw_config.h"
#include "../drw_platform.h"

#ifndef DRW_PLATFORM_WIN

#ifdef DRW_TYPE_PROVIDER_ENABLE_FTGL
void drw_type_ftgl_initialize(void);
#endif

//	this will all go away
void drw_type_ftgl_init(void);
void drw_type_ftgl_deinit(void);
int  drw_type_ftgl_load(const char*);
void drw_type_ftgl_draw(const char*);
void drw_type_ftgl_size(int, int);
void drw_type_ftgl_get_bbox(const char* str, unsigned long num, float* data);

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
