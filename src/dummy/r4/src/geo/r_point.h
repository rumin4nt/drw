//
//  r_point.h
//  r4
//
//  Created by Andrew Macfarlane on 10/21/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef r_point_h
#define r_point_h

#include <stdbool.h>
#include <stdio.h>

typedef struct RPoint
{
	double x;
	double y;

} RPoint;

typedef struct RPoint3
{
	double x;
	double y;
	double z;
} RPoint3;

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool   r_point_on_segment(RPoint p, RPoint q, RPoint r);

int    r_point_orientation(RPoint p, RPoint q, RPoint r);
RPoint r_point_mid(RPoint a, RPoint b);

RPoint r_point_intersection(RPoint a0, RPoint a1, RPoint b0, RPoint b1, double eps);


#endif /* r_point_h */
