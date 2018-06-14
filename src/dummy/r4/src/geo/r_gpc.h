
//  r_gpc.c
//  r_gpc
//
//  Created by Andrew Macfarlane on 19/04/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef r_gpc_h_
#define r_gpc_h_

#include "r_line.h"

#include <brsh/brsh.h>

typedef struct GPCRec
{
	void* polygon;
} GPCRec;

//note to andrew: don't bother documenting this, it's r4 and will not be released
// and it contains proprietary code anyway
/*
 void r_gpc_tess_create(WLine* line);
void r_gpc_tess_destroy(WLine* line);
*/
void* r_gpc_tess_create(WLine* line);
void r_gpc_tess_destroy(void* tess);

GPCRec* r_gpc_polygon_create(RLine* line);
void    r_gpc_polygon_destroy(GPCRec* rec);

GPCRec* r_gpc_polygon_boolean_raw(GPCRec* a, GPCRec* b, int mode);

int	r_gpc_polygon_boolean(RLine* la, RLine* lb, RLine** res_addr, int *num);

RLine*	r_gpc_rline(GPCRec* rec);
void	r_gpc_rlines(GPCRec* rec, RLine* first, int num);

void	r_gpc_rlines_raw(GPCRec* src, RLine** res_addr, int* num);

#endif
