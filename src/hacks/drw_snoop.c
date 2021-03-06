//
//  drw_snoop.c
//  drw
//
//  Created by vs on 6/21/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_snoop.h"

//	todo lose this it depends on r4
#include <r4/src/output/r_svg.h>

//#include <stdio.h>
#ifdef DRW_ENABLE_SNOOP

static RLine3** snoopdata = NULL;
static int      snoopnum  = 0;
static bool     _snooping = false;
//typedef void (*snoop_record_fun)(RLine* l);
#include <drw/drw.h>

void drw_snoop_add_rline3(void* data)
{

	RLine3* line = data;
	snoopnum++;
	if (!snoopdata)
	{
		snoopdata = calloc(1, sizeof(RLine3*));
	}
	else
	{
		snoopdata = realloc(snoopdata, snoopnum * sizeof(RLine3*));
	}

	double  rx, ry, rz;
	RLine3* rl = r_line3_create();
	for (int i = 0; i < line->num; i++)
	{
		RPoint3 p = line->data[i];

		double x = p.x;
		double y = p.y;
		double z = p.z;

		drw_point_3d_to_2d(x, y, z, &rx, &ry, &rz);
		r_line3_add_point3f(rl, rx, ry, rz);
	}

	if (line->closed)
	{
		RPoint3 p = line->data[0];
		double  x = p.x;
		double  y = p.y;
		double  z = p.z;
		drw_point_3d_to_2d(x, y, z, &rx, &ry, &rz);
		r_line3_add_point3f(rl, rx, ry, rz);
	}

	snoopdata[snoopnum - 1] = rl;
}

void drw_snoop_add_rline(void* data)
{
	RLine* line = data;
	snoopnum++;
	if (!snoopdata)
	{
		snoopdata = calloc(1, sizeof(RLine3*));
	}
	else
	{
		snoopdata = realloc(snoopdata, snoopnum * sizeof(RLine3*));
	}

	double  rx, ry, rz;
	RLine3* rl = r_line3_create();
	for (int i = 0; i < line->num; i++)
	{
		RPoint p = line->data[i];

		double x = p.x;
		double y = p.y;
		double z = 0;

		drw_point_3d_to_2d(x, y, z, &rx, &ry, &rz);
		r_line3_add_point3f(rl, rx, ry, rz);
	}

	
	if (line->closed)
	{
		RPoint p = line->data[0];

		double x = p.x;
		double y = p.y;
		double z = 0;

		drw_point_3d_to_2d(x, y, z, &rx, &ry, &rz);
		r_line3_add_point3f(rl, rx, ry, rz);
	}
	 
	
	
	snoopdata[snoopnum - 1] = rl;
}

void* drw_snoop_rline_from_f(float* data, int num)
{
	RLine* l = r_line_create();
	for (int i = 0; i < num; i += 2)
	{
		r_line_add_point2f(l, data[i], data[i + 1]);
	}
	return l;
}

static snoop_record_fun _snoop = NULL;

bool drw_snoop_get(void)
{
	return _snooping;
}

void drw_snoop_set(bool val)
{
	printf("Snoop: %d\n", val);
	_snooping = val;
}

void drw_snoop_dump(const char* path)
{

#ifdef R4_ENABLE_CAIRO
	RSVGRec* rec = r_svg_open(path);

	double dpi = drw_query_dpi();
	printf("are we scallllling our artwork by our stuuuupid high DPI display? %f\n", dpi);
	double sc = 1.0 / dpi;
	
	 sc = .75;
	printf("Dumping snoop data! %d lines\n", snoopnum);
	for (int i = 0; i < snoopnum; i++)
	{
		RLine3* l = snoopdata[i];
		r_line3_scale(l, sc, sc, 1);
		//printf("%lu points\n", l->num);
		r_svg_add_rline3(rec, l);
	}

	r_svg_close(rec);

	free(snoopdata);
	snoopdata = NULL;
	snoopnum  = 0;
#endif
}

void drw_snoop_fun_set(snoop_record_fun fun)
{
	printf("Setting snoop function\n");
	_snoop = fun;
}

#endif
