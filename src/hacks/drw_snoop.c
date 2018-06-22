//
//  drw_snoop.c
//  drw
//
//  Created by vs on 6/21/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_snoop.h"


//#include <stdio.h>
#ifdef DRW_ENABLE_SNOOP


static RLine** snoopdata = NULL;
static int snoopnum = 0;
static bool _snooping = false;
//typedef void (*snoop_record_fun)(RLine* l);

void drw_snoop_add(RLine* l)
{
	snoopnum++;
	if ( !snoopdata )
	{
		snoopdata = calloc(1, sizeof(RLine));
	}else{
		snoopdata = realloc( snoopdata, snoopnum * sizeof(RLine));
	}
	snoopdata[snoopnum - 1] = l;
	
	
	
}


RLine* drw_snoop_rline_from_f(float* data, int num)
{
	RLine* l =  r_line_create();
	for ( int i = 0 ; i < num; i+= 2)
	{
		r_line_add_point2f(l, data[i], data[i+1]);
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
	printf("Dumping snoop data!\n");
	for ( int i = 0; i < snoopnum; i++ )
	{
		RLine* l = snoopdata[i];
		printf("%lu points\n", l->num);
	}
	
	free(snoopdata);
	snoopdata = NULL;
	snoopnum = 0;
}

void drw_snoop_fun_set(snoop_record_fun fun)
{
	printf("Setting snoop function\n");
	_snoop = fun;
}

#endif

