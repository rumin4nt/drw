//
//  drw_type_asteroids.h
//  drw
//
//  Created by vs on 4/27/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_type_asteroids_h
#define drw_type_asteroids_h

//#include "../drw_config.h"
#include "drw_type.h"

#ifdef DRW_TYPE_PROVIDER_ENABLE_ASTEROIDS

void drw_type_asteroids_initialize(void);

void drw_type_asteroids(const char* text);
void drw_type_asteroids_bbox(const char*, unsigned long, float*);
void drw_type_asteroids_draw(const char*);

#endif

#endif /* drw_type_asteroids_h */
