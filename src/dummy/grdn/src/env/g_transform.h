
//  g_transform.c
//  g_transform
//
//  Created by Andrew Macfarlane on 11/04/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef g_transform_h_
#define g_transform_h_

#include <gl-matrix/gl-matrix.h>

typedef struct GTransform
{
	vec3_t anchor;
	vec3_t position;
	vec3_t rotation;
	vec3_t scale;
	double opacity;
	// WVec3d position;
	// WVec3d rotation;
	// WVec3d scale;
	// double opacity;
} GTransform;

void g_transform_create(GTransform* t);
void g_transform_destroy(GTransform* t);
void g_transform_reset(GTransform* t);

// GTransform* g_transform_create(void);

#endif
