//
//  drw_primitives.c
//  drw
//
//  Created by vs on 5/12/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_primitives.h"

RLine* drw_primitives_calculate_hexagon(double radius)
{
	return drw_primitives_calculate_circle(6, radius);
}

RLine* drw_primitives_calculate_circle(int sides, double radius)
{
	RLine* res = r_line_create();
	
	float deg2rad = (M_PI / sides);
	
	const int renderLineSize = (sides * 2);

	int i;
	for (i = 0; i < renderLineSize; i += 2)
	{
		float degInRad = deg2rad * (float)i;
		float x	= cos(degInRad + M_PI * .5) * radius;
		float y	= sin(degInRad + M_PI * .5) * radius;
		r_line_addrw_point2f(res, x, y);

		
	}
	res->closed = true;
	return res;
}
