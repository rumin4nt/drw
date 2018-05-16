//
//  r_vec.h
//  r4
//
//  Created by Andrew Macfarlane on 1/7/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef r_vec_h
#define r_vec_h

//#ifdef THIS_IS_DEPRECATED

#define VEC_LINE_MAX 256

typedef struct Vec2
{
	double x;
	double y;
} Vec2;

/*
typedef struct Vec3
{
	float x;
	float y;
	float z;
} Vec3;
*/
typedef struct
{
	signed long long num;
	signed long long reserved;
	Vec2		 data[VEC_LINE_MAX];
} Vec2Line;

/*
typedef struct
{
	signed long long num;
	signed long long reserved;
	vec3_t data[VEC_LINE_MAX];
} Vec3Line;
*/

// Vec3* r_vec3_create(void);
// Vec3Line* vec3line_create(void);

void vec2_add(Vec2*, Vec2*);
// void r_vec3_add(Vec3*, Vec3*);

void vec2_sub(Vec2*, Vec2*);
// void vec3_sub(Vec3*, Vec3*);

void vec2_mul(Vec2*, Vec2*);
// void vec3_mul(Vec3*, Vec3*);

// void r_vec3_set(Vec3*, float);
// void vec3_reset(Vec3*);

// void vec3line_add_vec(Vec3Line*, Vec3*);

// void vec2line_shift(Vec2Line* );
// void vec3line_shift(Vec3Line* );

// void vec2line_unshift(Vec2Line* );
// void vec3line_unshift(Vec3Line* );

// void vec3_print(Vec3* );

//#endif

#endif /* r_vec_h */
