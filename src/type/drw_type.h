//
//  drw_type.h
//  drw
//
//  Created by vs on 12/10/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_type_h
#define drw_type_h

#define DRW_TYPE_PROVIDER_ENABLE_ASTEROIDS

#include "drw_type_asteroids.h"
#include "drw_type_hershey.h"
#include "drw_type_hpvec.h"

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

typedef void (*drw_type_draw_fun)(const char*);
typedef void (*drw_type_bbox_fun)(const char*, unsigned long, float*);


extern int drw_type_debug;
void drw_type_init(void);
void drw_type_deinit(void);

void drw_type_debug_set(int v);

void   drw_type_draw(const char* text, ...);
void   drw_type_get_bbox(const char*, unsigned long sz, float*);
double drw_type_get_width(const char*);
int    drw_type_get_size(void);
void   drw_type_set_align(int w , int h);
void   drw_type_set_size(int x , int y);

void drw_type_load_ttf(const char* path);

int  drw_type_provider_register(const char* ident, drw_type_draw_fun render, drw_type_bbox_fun bbox);
int  drw_type_provider_count(void);
void drw_type_provider_select(signed index);

#endif /* drw_type_h */
