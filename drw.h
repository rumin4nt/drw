//
//  drw.h
//  r4
//
//  Created by Andrew Macfarlane on 4/8/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef drw_h
#define drw_h

#define DRW_VERSION_MAJOR 0
#define DRW_VERSION_MINOR 0
#define DRW_VERSION_PATCH 1

#include <r4/src/r4_platform.h>

#include "src/drw_render.h"
//#include "src/r_hatch.h"

#define D_FONT_SIZE 18

enum DRWAlignmentHorizontal
{
	DRW_FONT_ALIGN_H_LEFT,
	DRW_FONT_ALIGN_H_CENTER,
	DRW_FONT_ALIGN_H_RIGHT,
	DRW_FONT_ALIGN_H_NONE
};

enum DRWAlignmentVertical
{
	DRW_FONT_ALIGN_V_TOP,
	DRW_FONT_ALIGN_V_CENTER,
	DRW_FONT_ALIGN_V_BOTTOM,
	DRW_FONT_ALIGN_V_NONE
};

#ifdef RPLATFORM_IOS
#include "src/drw_font_ftgles.h"
#else
#include "src/drw_font_ftgl.h"
#endif

#include "src/drw_font_asteroids.h"
#include "src/drw_font_hershey.h"
#include "src/drw_font_hpvec.h"

//#include "src/d_glus.h"
#include "src/drw_wobject_ops.h"
#include "src/drw_point.h"

#include "src/drw_primitives.h"

#ifdef DEBUG
#include "stdio.h"
#endif

#include "stdlib.h"

int		    drw_check_version_match(const char* str);
int		    drw_check_compat_match(const char* str);
char*		    drw_get_version_string(void);
void		    drw_print_version(void);
char*		    drw_get_compat_string(void);
static inline char* drw_get_compat_string_header(void)
{
	char* buf = (char*)calloc(256, sizeof(char));
	sprintf(buf, "%d_%d", DRW_VERSION_MAJOR, DRW_VERSION_MINOR);
	return buf;
}

static inline char* drw_get_version_string_header(void)
{
	char* buf = (char*)calloc(256, sizeof(char));
	sprintf(buf, "%d_%d_%d", DRW_VERSION_MAJOR, DRW_VERSION_MINOR,
		DRW_VERSION_PATCH);
	return buf;
}
#endif /* drw_h */
