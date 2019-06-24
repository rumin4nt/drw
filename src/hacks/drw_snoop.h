//
//  drw_snoop.h
//  drw
//
//  Created by vs on 6/21/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_snoop_h
#define drw_snoop_h

#include <stdbool.h>
//	todo, get rid of this dependency, replace with void*
//#include <r4/src/geo/r_line.h>

#include "drw_hacks.h"


#ifdef DRW_ENABLE_SNOOP
typedef void (*snoop_record_fun)(void* l);

bool  drw_snoop_get(void);
void  drw_snoop_set(bool val);
void  drw_snoop_dump(const char* path);
void  drw_snoop_fun_set(snoop_record_fun fun);
void  drw_snoop_add_rline(void* l);
void* drw_snoop_rline_from_f(float* data, int num);

#endif

//RLine* drw_snoop_rline_from_f(float* data, int num);

//#endif

#endif /* drw_snoop_h */
