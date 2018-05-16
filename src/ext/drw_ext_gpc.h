//
//  drw_ext_gpc.h
//  drw
//
//  Created by vs on 5/16/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_ext_gpc_h
#define drw_ext_gpc_h

#include "../drw_config.h"
#include <r4/src/geo/r_gpc.h>

#include <gpc/gpc.h>

//#ifdef RUMINANT4_PRESENT
void drw_gpc_polygon_outline(GPCRec*);
void drw_gpc_polygon(GPCRec*);
void drw_gpc_verts(void*);
void drw_gpc_triwire(void* dat);
void drw_gpc_tristrip(void*);
//#endif

#endif /* drw_ext_gpc_h */
