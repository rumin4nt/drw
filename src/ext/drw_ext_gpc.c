//
//  drw_ext_gpc.c
//  drw
//
//  Created by vs on 5/16/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_ext_gpc.h"

#include "../drw_config.h"
#include <stdlib.h>

//#ifdef RUMINANT4_PRESENT

void drw_gpc_polygon_outline(GPCRec* rec)
{
	gpc_polygon* poly = (gpc_polygon*)rec->polygon;
	// gpc_polygon* poly = rec->polygon;
	// int	   j, s;
	int i, k, j;

	for (i = 0; i < poly->num_contours; i++)
	{
		// printf("c %d\n", i);
		gpc_vertex_list contour = poly->contour[i];

		GLfloat* arr =
		    malloc(sizeof(GLfloat) * contour.num_vertices * 2);
		for (k = 0, j = 0; k < contour.num_vertices; k++, j += 2)
		{
			if (!contour.vertex)
			{
				printf("ack!\n");
			}
			else
			{
				gpc_vertex v = contour.vertex[k];
				arr[j]       = v.x;
				arr[j + 1]   = v.y;
			}
		}

		glVertexPointer(2, GL_FLOAT, 0, arr);

		glDrawArrays(GL_LINE_LOOP, 0, contour.num_vertices);
		free(arr);
	}
}

void drw_gpc_polygon(GPCRec* rec)
{
	gpc_polygon* poly = (gpc_polygon*)rec->polygon;
	// gpc_polygon* poly = rec->polygon;
	// int	   j, s;
	int i, j, k;
	for (i = 0; i < poly->num_contours; i++)
	{
		// printf("c %d\n", i);
		gpc_vertex_list contour = poly->contour[i];

		GLfloat* arr =
		    malloc(sizeof(GLfloat) * contour.num_vertices * 2);
		for (k = 0, j = 0; k < contour.num_vertices; k++, j += 2)
		{
			if (!contour.vertex)
			{
				printf("ack!\n");
			}
			else
			{
				gpc_vertex v = contour.vertex[k];
				arr[j]       = v.x;
				arr[j + 1]   = v.y;
			}
		}

		glVertexPointer(2, GL_FLOAT, 0, arr);

		glDrawArrays(GL_LINE_LOOP, 0, contour.num_vertices);
		free(arr);
	}
}

void drw_gpc_verts(void* dat)
{
	/*
	 gpc_polygon poly
	 glLineWidth(2.0);
	 int c, v;
	 //double offset = 0;;
	 for (c= 0; c < poly.num_contours; c++)
	 {
	 for (v= 0; v < poly.contour[c].num_vertices; v++)
	 {

	 double x = poly.contour[c].vertex[v].x;
	 double y = poly.contour[c].vertex[v].y;

	 drw_translate2f(x,y);
	 drw_square(20);
	 drw_translate2f(-x,-y);

	 //printf("%f\n", x);
	 glBegin(GL_LINE_LOOP);

	 glVertex2d(poly.contour[c].vertex[v].x + offset,
	 poly.contour[c].vertex[v].y);
	 glVertex2d(poly.contour[c].vertex[v].x,
	 poly.contour[c].vertex[v].y + offset);
	 glVertex2d(poly.contour[c].vertex[v].x - offset,
	 poly.contour[c].vertex[v].y);
	 glVertex2d(poly.contour[c].vertex[v].x,
	 poly.contour[c].vertex[v].y - offset);
	 glEnd();

	 }
	 }
	 */
}

void drw_gpc_triwire(void* dat)
{
#ifndef DRW_PLATFORM_IOS
	glPolygonMode(GL_FRONT, GL_LINE);
	drw_gpc_tristrip(dat);
	glPolygonMode(GL_FRONT, GL_FILL);
#endif
}

void drw_gpc_tristrip(void* dat)
{
	gpc_tristrip* tri = dat;
	int	   j, s, v;

	for (s = 0; s < tri->num_strips; s++)
	{

		gpc_vertex_list str = tri->strip[s];
		GLfloat*	arr = malloc(sizeof(GLfloat) * str.num_vertices * 2);

		if (str.num_vertices < 2)
			continue;

		for (v = 0, j = 0; v < str.num_vertices; v++, j += 2)
		{
			arr[j]     = str.vertex[v].x;
			arr[j + 1] = str.vertex[v].y;

			// glVertex2d(str.vertex[v].x, str.vertex[v].y);
		}
		glVertexPointer(2, GL_FLOAT, 0, arr);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, str.num_vertices);
		free(arr);
	}
	//#endif
}

void drw_triangle_strip(WLine* poly)
{

	const unsigned long long renderLineSize = (poly->num * 2);

	// printf("poly is %d num\n", poly->num);
	// GLfloat arr[ renderLineSize ];
	GLfloat* arr = malloc(sizeof(GLfloat) * renderLineSize);
	int      i, j;
	for (i = 0, j = 0; i < poly->num; i++, j += 2)
	{
		WPoint* p  = &poly->data[i];
		arr[j]     = p->x;
		arr[j + 1] = p->y;
	}

	glVertexPointer(2, GL_FLOAT, 0, arr);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, (int)poly->num);
	free(arr);
}
//#endif

/*
void drw_tristrip_3d(double* arr, int num, bool closed)
{

	// const unsigned long long renderLineSize = ( poly->num * 2);

	// printf("poly is %d num\n", poly->num);
	// GLfloat arr[ renderLineSize ];
	// GLfloat* arr = malloc(sizeof(GLfloat) * renderLineSize);

	/*
	 for (int i=0,j=0; i < poly->num ; i++, j+=6 ) {


	 arr[j] =  arr[
	 arr[j+1] = p->y;
	 arr[j+2] = p->z;
	 arr[j+3] = p->x;
	 arr[j+4] = p->y;
	 arr[j+5] = p->z;
	 }
 //////////
	glVertexPointer(3, DRW_VERTEX_POINTER_IDENT, 0, arr);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, num);
	free(arr);
}
*/
