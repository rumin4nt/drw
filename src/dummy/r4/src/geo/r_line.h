//
//  wsh_line.h
//  wash
//
//  Created by Andrew Macfarlane on 12/15/16.
//  Copyright © 2016 vaporstack. All rights reserved.
//

#ifndef r_line_h
#define r_line_h

//#include <wsh/wsh.h>

#include "r_point.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct RLineSeg
{
	RPoint a;
	RPoint b;
} RLineSeg;

typedef struct RLineSeg3
{
	RPoint3 a;
	RPoint3 b;
} RLineSeg3;

typedef struct RLine3
{
	bool	  closed;
	RPoint3*      data;
	unsigned long num;
	unsigned long reserved;

} RLine3;

typedef struct RLine
{
	bool	  closed;
	unsigned long num;
	unsigned long reserved;
	RPoint*       data;
	RPoint	center;
	double	inner;
	double	outer;
} RLine;

typedef struct RLineHnd
{
	RLine* src;

} RLineHnd;

// typedef unsigned long _ull;

RLineHnd* r_line_hnd_create(void);
RLineHnd* r_line_hnd_copy(RLineHnd*);

RLine* r_line_create(void);
RLine* r_line_copy(RLine*);

RPoint r_point_create(void);
RPoint r_point_create2f(double x, double y);

void r_line_addrw_point(RLine*, RPoint);
void r_line_addrw_point2f(RLine*, double x, double y);

void r_line_subdivide(RLine*, double radius);
void r_line_fuckup(RLine*, double radius);

RPoint3 r_point3_create(void);
RLine3* r_line3_create(void);
RLine3* r_line3_copy(RLine3* line);
void    r_line3_destroy(RLine3* line);
void    r_line_destroy(RLine* line);

void r_line3_addrw_point(RLine3*, RPoint3);
void r_line3_addrw_point_3f(RLine3*, double x, double y, double z);
void r_line3_clear(RLine3* line);
void r_line_move(RLine* src, double x, double y);

RPoint r_line_find_center_mean(RLine* line);
RPoint r_line_find_center_avg(RLine* line);

//RLine* r_line_from_wline(WLine* line);

void r_line_calculate_circle(RLine* line, RPoint* center, double* inner, double* outer);
/*

void 		wsh_line_hnd_destroy(WLineHnd*);
void		wsh_line_hnd_addrw_point(WLineHnd, WPoint);

void		wsh_line_move(WLine* src, double x, double y);
void		wsh_line_scale(WLine* src, double x, double y);

void		wsh_line_find_mins(WLine* src, double* x, double* y);
void		wsh_line_find_maxs(WLine* src, double* x, double* y);


//void		wsh_line_concat(WLine* dst, WLine* src, ull, ull);

//void 		wsh_line_destroy(WLine* );


//WLine* 		wsh_line_normalize(WLine*, double* dx, double* dy);
 */

#endif /* r_line_h */
