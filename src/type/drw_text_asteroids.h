//
//  drw_text_asteroids.h
//  drw
//
//  Created by vs on 4/27/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_text_asteroids_h
#define drw_text_asteroids_h

#include "../drw_config.h"

#ifdef DRW_TEXT_PROVIDER_ENABLE_ASTEROIDS
void drw_text_asteroids_initialize(void);

void drw_text_asteroids(const char* text);

#endif

#endif /* drw_text_asteroids_h */
