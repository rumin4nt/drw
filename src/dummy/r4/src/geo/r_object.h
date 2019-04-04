
//  r_object.c
//  r_object
//
//  Created by Andrew Macfarlane on 11/04/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef r_object_h_
#define r_object_h_

//	i sort of hate this, *this* is a rare perfect example of
//	a template being the correct solution
#include "r_line.h"
//#include <vector/vector.h>

typedef struct RObject
{
	RLine**       lines;
	unsigned long num;
	//struct vector_t lines;
} RObject;

RObject* r_object_create(void);
RObject* r_geo_interop_robject_from_wobject(WObject* obj);
void     r_object_destroy(RObject* obj);
void     r_object_add_line(RObject* obj, RLine* line);
void     r_object_remove_line(RObject* obj, RLine* line);

#endif
