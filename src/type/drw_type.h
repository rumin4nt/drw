//
//  drw_type.h
//  drw
//
//  Created by vs on 12/10/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_type_h
#define drw_type_h

#define DRW_TEXT_PROVIDER_ENABLE_ASTEROIDS

#include "drw_type_asteroids.h"
#include "drw_type_hershey.h"
#include "drw_type_hpvec.h"

enum
{
	DRW_TEXT_PROVIDER_FTGL,
	DRW_TEXT_PROVIDER_HPVEC,
	DRW_TEXT_PROVIDER_HERSHEY,
	DRW_TEXT_PROVIDER_ASTEROIDS,
	DRW_TEXT_PROVIDER_NONE
};

void   drw_type_get_bbox(const char*, unsigned int, float*);
double drw_type_get_width(const char*);
int    drw_type_get_fontsize(void);
void   drw_type_set_align(int, int);

int drw_type_register_provider(void);

#endif /* drw_type_h */
