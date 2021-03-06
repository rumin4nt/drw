//
//  drw_type.h
//  drw
//
//  Created by vs on 12/10/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_type_h
#define drw_type_h

#include <stdbool.h>

enum DRWAlignmentHorizontal
{
	DRW_TYPE_ALIGN_H_LEFT,
	DRW_TYPE_ALIGN_H_CENTER,
	DRW_TYPE_ALIGN_H_RIGHT,
	DRW_TYPE_ALIGN_H_NONE
};

enum DRWAlignmentVertical
{
	DRW_TYPE_ALIGN_V_TOP,
	DRW_TYPE_ALIGN_V_CENTER,
	DRW_TYPE_ALIGN_V_BOTTOM,
	DRW_TYPE_ALIGN_V_NONE
};

enum
{
	DRW_TYPE_PROVIDER_FTGL,
	DRW_TYPE_PROVIDER_HPVEC,
	DRW_TYPE_PROVIDER_HERSHEY,
	DRW_TYPE_PROVIDER_ASTEROIDS,
	DRW_TYPE_PROVIDER_NONE
};

#define DRW_EXT_R4

#include <stdbool.h>
//#include "../drw_ext.h"
#include "../drw_config.h"
#ifdef DRW_PLATFORM_IOS
#include <TargetConditionals.h>
#define DRW_TYPE_PROVIDER_ENABLE_FTGLES

#ifndef TARGET_IPHONE_SIMULATOR
#define DRW_TYPE_PROVIDER_ENABLE_FTGLES
#endif

#else
#define DRW_TYPE_PROVIDER_ENABLE_FTGL
#endif

//#ifndef DRW_PLATFORM_IOS
#define DRW_TYPE_PROVIDER_ENABLE_HPVEC
//#endif

#define DRW_TYPE_PROVIDER_ENABLE_ASTEROIDS

#ifndef DRW_PLATFORM_LINUX
//#ifndef DRW_PLATFORM_IOS
#define DRW_TYPE_PROVIDER_ENABLE_HERSHEY

//#endif
#endif

typedef void (*drw_type_draw_fun)(const char*);
typedef void (*drw_type_bbox_fun)(const char*, unsigned long, float*);

#ifdef DRW_EXT_R4

typedef void* (*drw_type_render_fun)(const char*);
#endif

extern int drw_type_debug;
void       drw_type_init(void);
void       drw_type_deinit(void);

void drw_type_debug_set(int v);

void   drw_type_draw(const char* text, ...);
void   drw_type_draw_fixed(const char* text);
void   drw_type_draw_bbox(const char* text);

void   drw_type_get_bbox(const char*, unsigned long sz, float*);
double drw_type_get_width(const char*);
int    drw_type_get_size(void);
void   drw_type_get_default(double* x, double* y);

void drw_type_set_align(int w, int h);
void drw_type_set_size(int x, int y);
void drw_type_set_righttoleft(bool v);

void drw_type_load_ttf(const char* path);

#ifdef DRW_EXT_R4

int drw_type_provider_register(const char* ident, drw_type_draw_fun draw, drw_type_bbox_fun bbox, drw_type_render_fun render);
#else
int drw_type_provider_register(const char* ident, drw_type_draw_fun draw, drw_type_bbox_fun bbox);
#endif

int  drw_type_provider_count(void);
void drw_type_provider_select(signed index);

double drw_type_size_real_get(void);
void   drw_type_size_set(int sz);
int    drw_type_size_get(void);

#ifdef DRW_EXT_R4

void* drw_type_render(const char* text);

#endif

#endif /* drw_type_h */
