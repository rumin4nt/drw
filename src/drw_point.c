//
//  drw_point.c
//  drw
//
//  Created by vs on 5/1/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_point.h"

#include <drw/drw.h>

void mathInvertMatrix4f(float* restrict inv, const float* restrict m)
{
	float det;
	int   i;
	inv[0 * 4 + 0] = m[1 * 4 + 1] * m[2 * 4 + 2] * m[3 * 4 + 3] - m[1 * 4 + 1] * m[2 * 4 + 3] * m[3 * 4 + 2] - m[2 * 4 + 1] * m[1 * 4 + 2] * m[3 * 4 + 3] + m[2 * 4 + 1] * m[1 * 4 + 3] * m[3 * 4 + 2] + m[3 * 4 + 1] * m[1 * 4 + 2] * m[2 * 4 + 3] - m[3 * 4 + 1] * m[1 * 4 + 3] * m[2 * 4 + 2];
	inv[1 * 4 + 0] = -m[1 * 4 + 0] * m[2 * 4 + 2] * m[3 * 4 + 3] + m[1 * 4 + 0] * m[2 * 4 + 3] * m[3 * 4 + 2] + m[2 * 4 + 0] * m[1 * 4 + 2] * m[3 * 4 + 3] - m[2 * 4 + 0] * m[1 * 4 + 3] * m[3 * 4 + 2] - m[3 * 4 + 0] * m[1 * 4 + 2] * m[2 * 4 + 3] + m[3 * 4 + 0] * m[1 * 4 + 3] * m[2 * 4 + 2];
	inv[2 * 4 + 0] = m[1 * 4 + 0] * m[2 * 4 + 1] * m[3 * 4 + 3] - m[1 * 4 + 0] * m[2 * 4 + 3] * m[3 * 4 + 1] - m[2 * 4 + 0] * m[1 * 4 + 1] * m[3 * 4 + 3] + m[2 * 4 + 0] * m[1 * 4 + 3] * m[3 * 4 + 1] + m[3 * 4 + 0] * m[1 * 4 + 1] * m[2 * 4 + 3] - m[3 * 4 + 0] * m[1 * 4 + 3] * m[2 * 4 + 1];
	inv[3 * 4 + 0] = -m[1 * 4 + 0] * m[2 * 4 + 1] * m[3 * 4 + 2] + m[1 * 4 + 0] * m[2 * 4 + 2] * m[3 * 4 + 1] + m[2 * 4 + 0] * m[1 * 4 + 1] * m[3 * 4 + 2] - m[2 * 4 + 0] * m[1 * 4 + 2] * m[3 * 4 + 1] - m[3 * 4 + 0] * m[1 * 4 + 1] * m[2 * 4 + 2] + m[3 * 4 + 0] * m[1 * 4 + 2] * m[2 * 4 + 1];
	inv[0 * 4 + 1] = -m[0 * 4 + 1] * m[2 * 4 + 2] * m[3 * 4 + 3] + m[0 * 4 + 1] * m[2 * 4 + 3] * m[3 * 4 + 2] + m[2 * 4 + 1] * m[0 * 4 + 2] * m[3 * 4 + 3] - m[2 * 4 + 1] * m[0 * 4 + 3] * m[3 * 4 + 2] - m[3 * 4 + 1] * m[0 * 4 + 2] * m[2 * 4 + 3] + m[3 * 4 + 1] * m[0 * 4 + 3] * m[2 * 4 + 2];
	inv[1 * 4 + 1] = m[0 * 4 + 0] * m[2 * 4 + 2] * m[3 * 4 + 3] - m[0 * 4 + 0] * m[2 * 4 + 3] * m[3 * 4 + 2] - m[2 * 4 + 0] * m[0 * 4 + 2] * m[3 * 4 + 3] + m[2 * 4 + 0] * m[0 * 4 + 3] * m[3 * 4 + 2] + m[3 * 4 + 0] * m[0 * 4 + 2] * m[2 * 4 + 3] - m[3 * 4 + 0] * m[0 * 4 + 3] * m[2 * 4 + 2];
	inv[2 * 4 + 1] = -m[0 * 4 + 0] * m[2 * 4 + 1] * m[3 * 4 + 3] + m[0 * 4 + 0] * m[2 * 4 + 3] * m[3 * 4 + 1] + m[2 * 4 + 0] * m[0 * 4 + 1] * m[3 * 4 + 3] - m[2 * 4 + 0] * m[0 * 4 + 3] * m[3 * 4 + 1] - m[3 * 4 + 0] * m[0 * 4 + 1] * m[2 * 4 + 3] + m[3 * 4 + 0] * m[0 * 4 + 3] * m[2 * 4 + 1];
	inv[3 * 4 + 1] = m[0 * 4 + 0] * m[2 * 4 + 1] * m[3 * 4 + 2] - m[0 * 4 + 0] * m[2 * 4 + 2] * m[3 * 4 + 1] - m[2 * 4 + 0] * m[0 * 4 + 1] * m[3 * 4 + 2] + m[2 * 4 + 0] * m[0 * 4 + 2] * m[3 * 4 + 1] + m[3 * 4 + 0] * m[0 * 4 + 1] * m[2 * 4 + 2] - m[3 * 4 + 0] * m[0 * 4 + 2] * m[2 * 4 + 1];
	inv[0 * 4 + 2] = m[0 * 4 + 1] * m[1 * 4 + 2] * m[3 * 4 + 3] - m[0 * 4 + 1] * m[1 * 4 + 3] * m[3 * 4 + 2] - m[1 * 4 + 1] * m[0 * 4 + 2] * m[3 * 4 + 3] + m[1 * 4 + 1] * m[0 * 4 + 3] * m[3 * 4 + 2] + m[3 * 4 + 1] * m[0 * 4 + 2] * m[1 * 4 + 3] - m[3 * 4 + 1] * m[0 * 4 + 3] * m[1 * 4 + 2];
	inv[1 * 4 + 2] = -m[0 * 4 + 0] * m[1 * 4 + 2] * m[3 * 4 + 3] + m[0 * 4 + 0] * m[1 * 4 + 3] * m[3 * 4 + 2] + m[1 * 4 + 0] * m[0 * 4 + 2] * m[3 * 4 + 3] - m[1 * 4 + 0] * m[0 * 4 + 3] * m[3 * 4 + 2] - m[3 * 4 + 0] * m[0 * 4 + 2] * m[1 * 4 + 3] + m[3 * 4 + 0] * m[0 * 4 + 3] * m[1 * 4 + 2];
	inv[2 * 4 + 2] = m[0 * 4 + 0] * m[1 * 4 + 1] * m[3 * 4 + 3] - m[0 * 4 + 0] * m[1 * 4 + 3] * m[3 * 4 + 1] - m[1 * 4 + 0] * m[0 * 4 + 1] * m[3 * 4 + 3] + m[1 * 4 + 0] * m[0 * 4 + 3] * m[3 * 4 + 1] + m[3 * 4 + 0] * m[0 * 4 + 1] * m[1 * 4 + 3] - m[3 * 4 + 0] * m[0 * 4 + 3] * m[1 * 4 + 1];
	inv[3 * 4 + 2] = -m[0 * 4 + 0] * m[1 * 4 + 1] * m[3 * 4 + 2] + m[0 * 4 + 0] * m[1 * 4 + 2] * m[3 * 4 + 1] + m[1 * 4 + 0] * m[0 * 4 + 1] * m[3 * 4 + 2] - m[1 * 4 + 0] * m[0 * 4 + 2] * m[3 * 4 + 1] - m[3 * 4 + 0] * m[0 * 4 + 1] * m[1 * 4 + 2] + m[3 * 4 + 0] * m[0 * 4 + 2] * m[1 * 4 + 1];
	inv[0 * 4 + 3] = -m[0 * 4 + 1] * m[1 * 4 + 2] * m[2 * 4 + 3] + m[0 * 4 + 1] * m[1 * 4 + 3] * m[2 * 4 + 2] + m[1 * 4 + 1] * m[0 * 4 + 2] * m[2 * 4 + 3] - m[1 * 4 + 1] * m[0 * 4 + 3] * m[2 * 4 + 2] - m[2 * 4 + 1] * m[0 * 4 + 2] * m[1 * 4 + 3] + m[2 * 4 + 1] * m[0 * 4 + 3] * m[1 * 4 + 2];
	inv[1 * 4 + 3] = m[0 * 4 + 0] * m[1 * 4 + 2] * m[2 * 4 + 3] - m[0 * 4 + 0] * m[1 * 4 + 3] * m[2 * 4 + 2] - m[1 * 4 + 0] * m[0 * 4 + 2] * m[2 * 4 + 3] + m[1 * 4 + 0] * m[0 * 4 + 3] * m[2 * 4 + 2] + m[2 * 4 + 0] * m[0 * 4 + 2] * m[1 * 4 + 3] - m[2 * 4 + 0] * m[0 * 4 + 3] * m[1 * 4 + 2];
	inv[2 * 4 + 3] = -m[0 * 4 + 0] * m[1 * 4 + 1] * m[2 * 4 + 3] + m[0 * 4 + 0] * m[1 * 4 + 3] * m[2 * 4 + 1] + m[1 * 4 + 0] * m[0 * 4 + 1] * m[2 * 4 + 3] - m[1 * 4 + 0] * m[0 * 4 + 3] * m[2 * 4 + 1] - m[2 * 4 + 0] * m[0 * 4 + 1] * m[1 * 4 + 3] + m[2 * 4 + 0] * m[0 * 4 + 3] * m[1 * 4 + 1];
	inv[3 * 4 + 3] = m[0 * 4 + 0] * m[1 * 4 + 1] * m[2 * 4 + 2] - m[0 * 4 + 0] * m[1 * 4 + 2] * m[2 * 4 + 1] - m[1 * 4 + 0] * m[0 * 4 + 1] * m[2 * 4 + 2] + m[1 * 4 + 0] * m[0 * 4 + 2] * m[2 * 4 + 1] + m[2 * 4 + 0] * m[0 * 4 + 1] * m[1 * 4 + 2] - m[2 * 4 + 0] * m[0 * 4 + 2] * m[1 * 4 + 1];
	det	    = 1.0f / (m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12]);
	for (i = 0; i < 16; i++)
		inv[i] *= det;
	return;
}

static inline void mathMulMatrix4fPoint4f(float* restrict out, const float* restrict m, const float* restrict v)
{
	out[0] = (v[0] * m[0 * 4 + 0]) + (v[1] * m[1 * 4 + 0]) + (v[2] * m[2 * 4 + 0]) + (v[3] * m[3 * 4 + 0]);
	out[1] = (v[0] * m[0 * 4 + 1]) + (v[1] * m[1 * 4 + 1]) + (v[2] * m[2 * 4 + 1]) + (v[3] * m[3 * 4 + 1]);
	out[2] = (v[0] * m[0 * 4 + 2]) + (v[1] * m[1 * 4 + 2]) + (v[2] * m[2 * 4 + 2]) + (v[3] * m[3 * 4 + 2]);
	out[3] = (v[0] * m[0 * 4 + 3]) + (v[1] * m[1 * 4 + 3]) + (v[2] * m[2 * 4 + 3]) + (v[3] * m[3 * 4 + 3]);
	return;
}

void mathMulMatrix4fMatrix4f(float* restrict out, const float* restrict m1, const float* restrict m2)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		out[i * 4 + 0] = m1[i * 4 + 0] * m2[0 * 4 + 0] + m1[i * 4 + 1] * m2[1 * 4 + 0] + m1[i * 4 + 2] * m2[2 * 4 + 0] + m1[i * 4 + 3] * m2[3 * 4 + 0];
		out[i * 4 + 1] = m1[i * 4 + 0] * m2[0 * 4 + 1] + m1[i * 4 + 1] * m2[1 * 4 + 1] + m1[i * 4 + 2] * m2[2 * 4 + 1] + m1[i * 4 + 3] * m2[3 * 4 + 1];
		out[i * 4 + 2] = m1[i * 4 + 0] * m2[0 * 4 + 2] + m1[i * 4 + 1] * m2[1 * 4 + 2] + m1[i * 4 + 2] * m2[2 * 4 + 2] + m1[i * 4 + 3] * m2[3 * 4 + 2];
		out[i * 4 + 3] = m1[i * 4 + 0] * m2[0 * 4 + 3] + m1[i * 4 + 1] * m2[1 * 4 + 3] + m1[i * 4 + 2] * m2[2 * 4 + 3] + m1[i * 4 + 3] * m2[3 * 4 + 3];
	}
	return;
}

int mathProjectPoint(float* point, const float* restrict combinedmatrix, float pointx, float pointy, float pointz)
{
	float in[4], out[4], finv;
	in[0] = pointx;
	in[1] = pointy;
	in[2] = pointz;
	in[3] = 1.0f;
	mathMulMatrix4fPoint4f(out, combinedmatrix, in);
	if (out[3] == 0.0f)
		return 0;
	finv     = 0.5f / out[3];
	point[0] = (finv * out[0]) + 0.5f;
	point[1] = (finv * out[1]) + 0.5f;
	point[2] = (finv * out[2]) + 0.5f;
	return 1;
}

int mathUnprojectPoint(float* point, const float* restrict combinedmatrix, float pointx, float pointy, float pointz)
{
	float in[4], out[4], matrixinverse[16];
	mathInvertMatrix4f(matrixinverse, combinedmatrix);
	in[0] = (pointx * 2.0f) - 1.0f;
	in[1] = (pointy * 2.0f) - 1.0f;
	in[2] = (pointz * 2.0f) - 1.0f;
	in[3] = 1.0f;
	mathMulMatrix4fPoint4f(out, matrixinverse, in);
	if (out[3] == 0.0f)
		return 0;
	point[0] = out[0] / out[3];
	point[1] = out[1] / out[3];
	point[2] = out[2] / out[3];
	return 1;
}

// Get combinedmatrix with:
//mathMulMatrix4fMatrix4f(combinedmatrix, modelviewmatrix, projectionmatrix);

void drw_point_3d_to_2d(double x, double y, double z, double* rx, double* ry, double* rz)
{
	float m[16];
	float p[16];
	GLint    viewport[4];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glGetFloatv(GL_PROJECTION_MATRIX, p);
	glGetIntegerv(GL_VIEWPORT, viewport);

	float* c = calloc(16, sizeof(float));
	mathMulMatrix4fMatrix4f(c, m, p);
	
	float* v = calloc(3, sizeof(float));

	
	
	mathProjectPoint(v, c, x, y, z);
	
	v[0] -= .5;
	v[1] -= .5;
	v[2] -= .5;
	v[0] *= 2;
	v[1] *= 2;
	v[2] *= 2;
	v[0] *= viewport[2];
	v[1] *= viewport[3];
	//printf("%f %f %f\n", v[0], v[1], v[2]);
	
	
	*rx = v[0];
	*ry = v[1];
	*rz = v[2];
	free(v);
	
	/*
	int w = viewport[2];
	int h = viewport[3];

	for (int i = 0; i < 4; i++)
	{
		viewport[i] *= 2;
	}
	viewport[0] -= w * 1;
	viewport[1] -= h * 1;
	viewport[2] -= w * 1;
	viewport[3] -= h * 1;

	//viewport[2] *= 2;
	//viewport[3] *= 2;

	 viewport[0] -= w;
	viewport[1] -= h;
	viewport[2] -= w;
	viewport[3] -= h;


	gluProject(x, y, z, m, p, viewport, rx, ry, rz);
	*/
}

/*
 public Vector2 form3Dto2D(Vector3 our3DPoint)
 {
 Vector3 our2DPoint;

 float[] modelviewMatrix =  new float[16];
 float[] projectionMatrix = new float[16];
 int[] viewport = new int[4];

 GL.GetFloat(GetPName.ModelviewMatrix, modelviewMatrix);
 GL.GetFloat(GetPName.ProjectionMatrix, projectionMatrix);
 GL.GetInteger(GetPName.Viewport, viewport);

 OpenTK.Graphics.Glu.Project(our3DPoint, convertFloatsToDoubles(modelviewMatrix),
 convertFloatsToDoubles(projectionMatrix), viewport, out our2DPoint);

 return new Vector2(our2DPoint.X, our2DPoint.Y)
 }
 */

void drw_point_2d_to_3d(double x, double y, double* _rx, double* _ry, double* _rz)
{
	GLint viewport[4];

#ifdef RPLATFORM_IOS
	GLfloat modelview[16];
	GLfloat projection[16];

	GLfloat worldx, worldy, worldz;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);

#else

	GLdouble modelview[16];
	GLdouble projection[16];

	GLdouble worldx, worldy, worldz;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

#endif

	double winx, winy, winz;

	glGetIntegerv(GL_VIEWPORT, viewport);
	winx = x;
	winy = (float)y;
	winz = 0.;

#ifndef RPLATFORM_WIN
	gluUnProject(winx, winy, winz, modelview, projection, viewport, &worldx,
		     &worldy, &worldz);

#ifdef RPLATFORM_IOS
// worldx += app_settings.current_window_x * .5;
// worldy += app_settings.current_window_y * .5;
#else
// worldx += app_settings.current_window_x;
// worldy += app_settings.current_window_y;
#endif
	// worldy
	// gluUnProject( winx, winy, winz, modelview, projection, viewport,
	// &worldx, &worldy, &worldz);
	*_rx = worldx;
	*_ry = worldy;
	*_rz = worldz;
//#endif
#endif
	///*/
}
