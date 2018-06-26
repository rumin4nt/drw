//
//  drw_snoop.h
//  drw
//
//  Created by vs on 6/21/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_snoop_h
#define drw_snoop_h


#include <drw/src/drw_config.h>

#ifdef DRW_ENABLE_SNOOP

//	todo, get rid of this dependency, replace with void*
#include <r4/src/geo/r_line.h>

typedef void (*snoop_record_fun)(RLine* l);

bool drw_snoop_get(void);
void drw_snoop_set(bool val);
void drw_snoop_dump(const char* path);
void drw_snoop_fun_set(snoop_record_fun fun);
void drw_snoop_add(RLine* l);

RLine* drw_snoop_rline_from_f(float* data, int num);

#endif

#endif /* drw_snoop_h */
