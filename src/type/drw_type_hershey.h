//
//  drw_type_hershey.h
//  drw
//
//  Created by vs on 1/11/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_type_hershey_h
#define drw_type_hershey_h

#include <drw/src/drw_config.h>

#ifdef DRW_TYPE_PROVIDER_ENABLE_HERSHEY
void drw_type_hershey_initialize(void);
#endif

void drw_type_hershey_draw(const char* text);
void drw_type_hershey_bbox(const char*, unsigned long, float*);

#endif /* drw_type_hershey_h */
