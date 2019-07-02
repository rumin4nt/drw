//
//  drw_type_hpvec.h
//  drw
//
//  Created by vs on 4/27/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_type_hpvec_h
#define drw_type_hpvec_h

//#include "drw.h"
#include "drw_type.h"

#ifdef DRW_TYPE_PROVIDER_ENABLE_HPVEC

void drw_type_hpvec_initialize(void);

#define HPVEC_FONT_SIZE 16

typedef struct HPGlyph
{
	unsigned long num;
	const int*    points;
} HPGlyph;

//void drw_type_hpvec_set_alignment(int h, int v);
void drw_type_hpvec_draw(const char* text);

HPGlyph** drw_type_hpvec_glyph(const char* text);
//void drw_type_hpvec_get_offset_scale(const char* text, double* ox, double* oy, double* sc);

//#include "../drw_ext.h"

#ifdef DRW_EXT_R4
void* drw_type_hpvec_render(const char* text);

#endif
#endif

#endif /* drw_type_hpvec_h */
