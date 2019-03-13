//
//  drw_primitives.h
//  drw
//
//  Created by vs on 5/12/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_primitives_h
#define drw_primitives_h

//#include <r4/r4.h>

#ifndef RUMINANT4_PRESENT
//#error hi

#include "dummy/r4/src/geo/r_line.h"
#else
#include <r4/src/geo/r_line.h>

#endif

RLine* drw_primitives_calculate_square(double radius);
RLine* drw_primitives_calculate_triangle(double radius, double offset);
RLine* drw_primitives_calculate_hexagon(double radius, double offset);
RLine* drw_primitives_calculate_circle(int sides, double radius, double offset);

#endif /* drw_primitives_h */
