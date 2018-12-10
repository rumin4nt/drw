//
//  drw_type.c
//  drw
//
//  Created by vs on 12/10/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_type.h"

static int align_x = 0;
static int align_y = 0;

void drw_type_get_align(int* x, int* y)
{
	*x = align_x;
	*y = align_y;
}

void drw_type_set_align(int x, int y)
{
	align_x = x;
	align_y = y;
}
