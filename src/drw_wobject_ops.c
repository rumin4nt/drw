//
//  drw_wobject_ops.c
//  drw
//
//  Created by Andrew Macfarlane on 9/20/17.
//  Copyright © 2017 ruminant. All rights reserved.
//

#include "drw_wobject_ops.h"

#ifdef DRW_BUILD_STANDALONE
//	todo: get rid of this!
#include <r4/src/core/r_random.h>

WPoint drw_wobject_ops_randompointfromobject(WObject* obj, int* line_i,
					     int* point_i)
{
	int num   = obj->num;
	int which = r_rand_double(NULL) * num;
	if (line_i)
		*line_i = which;
	return drw_wobject_ops_randompointfromline(obj->lines[which], point_i);
}

WPoint drw_wobject_ops_randompointfromline(WLine* line, int* point_i)
{
	int which = r_rand_double(NULL) * line->num;
	if (point_i)
		*point_i = which;
	return line->data[which];
}

#endif
