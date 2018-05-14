//
//  d_wobject_ops.c
//  drw
//
//  Created by Andrew Macfarlane on 9/20/17.
//  Copyright © 2017 ruminant. All rights reserved.
//

#include "d_wobject_ops.h"
#include <r4/r4.h>

WPoint d_wobject_ops_randompointfromobject(WObject* obj, int* line_i,
					   int* point_i)
{
	int num   = obj->num_lines;
	int which = r_rand_double(NULL) * num;
	if (line_i)
		*line_i = which;
	return d_wobject_ops_randompointfromline(obj->lines[which], point_i);
}

WPoint d_wobject_ops_randompointfromline(WLine* line, int* point_i)
{
	int which = r_rand_double(NULL) * line->num;
	if (point_i)
		*point_i = which;
	return line->data[which];
}
