//
//  r_rect.h
//  r4
//
//  Created by Andrew Macfarlane on 1/7/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef r_rect_h
#define r_rect_h

#include "r_vec.h"

#include <gl-matrix/gl-matrix.h>
#include <stdbool.h>

typedef struct
{
	Vec2 pos;
	Vec2 size;
} RRect;

RRect* r_rect_create(float, float, float, float);
void   r_rect_destroy(RRect*);
bool   r_rect_within(RRect, float, float);
void   r_rect_set_pos(RRect*, float, float);
void   r_rect_set_size(RRect*, float, float);
void r_rect_print(RRect* r);

#endif /* r_rect_h */
