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

#include "src/drw_platform.h"

#include "src/drw_config.h"
#include "src/drw_export.h"
#include "src/drw_point.h"
#include "src/drw_render.h"
#include "src/drw_render_gl.h"

//#include "src/r_hatch.h"

//#define DRW_BUILD_STANDALONE

#ifdef DRW_PLATFORM_IOS
//#error ios
#include "src/type/drw_type_ftgles.h"
#else
//#error macos
#include "src/type/drw_type_ftgl.h"
#endif

#include "src/type/drw_type_asteroids.h"
#include "src/type/drw_type_hershey.h"
#include "src/type/drw_type_hpvec.h"

//#include "src/d_glus.h"
#include "src/drw_point.h"
#include "src/drw_wobject_ops.h"

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
	sprintf(buf, "%d.%d", DRW_VERSION_MAJOR, DRW_VERSION_MINOR);
	return buf;
}

static inline char* drw_get_version_string_header(void)
{
	char* buf = (char*)calloc(256, sizeof(char));
	sprintf(buf, "%d.%d.%d", DRW_VERSION_MAJOR, DRW_VERSION_MINOR,
		DRW_VERSION_PATCH);
	return buf;
}




int    drw_lib_init(void);
int    drw_lib_deinit(void);
double drw_type_size_real_get(void);
void   drw_type_size_set(int sz);
int    drw_type_size_get(void);

#endif /* drw_h */
