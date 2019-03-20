//
//  drw_primitives.c
//  drw
//
//  Created by vs on 5/12/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_primitives.h"

#include <math.h>

RLine* drw_primitives_calculate_square(double radius)
{
	RLine* res = r_line_create();
	
	r_line_add_point2f(res, radius * -.5, radius * -.5 );
	r_line_add_point2f(res, radius *  .5, radius * -.5 );
	r_line_add_point2f(res, radius *  .5, radius *  .5 );
	r_line_add_point2f(res, radius * -.5, radius *  .5 );
	return res;
	
	//return drw_primitives_calculate_circle(4, radius);
}

RLine* drw_primitives_calculate_triangle(double radius, double offset)
{
	return drw_primitives_calculate_circle(3, radius, offset);
}

RLine* drw_primitives_calculate_hexagon(double radius, double offset)
{
	return drw_primitives_calculate_circle(6, radius, offset);
}

RLine* drw_primitives_calculate_circle(int sides, double radius, double offset)
{
	RLine* res = r_line_create();

	float deg2rad = (M_PI / sides);

	const int renderLineSize = (sides * 2);

	int i;
	for (i = 0; i < renderLineSize; i += 2)
	{
		float degInRad = deg2rad * (float)i;
		float x	= cos(offset + degInRad + M_PI * .5) * radius;
		float y	= sin(offset + degInRad + M_PI * .5) * radius;
		r_line_add_point2f(res, x, y);
	}
	res->closed = true;
	return res;
}
