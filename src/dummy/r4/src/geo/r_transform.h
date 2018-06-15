//
//  r_transform.h
//  r4
//
//  Created by vs on 11/26/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef r_transform_h
#define r_transform_h

#include <stdlib.h>

#include <gl-matrix/gl-matrix.h>

typedef struct RTransform
{
	vec3_t position;
	vec3_t rotation;
	vec3_t anchor;
} RTransform;

RTransform* r_transform_create(void);
void	r_transform_destroy(RTransform* t);

#endif /* r_transform_h */
