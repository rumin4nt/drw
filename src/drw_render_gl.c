//
//  render.c
//  r4
//
//  Created by Andrew Macfarlane on 12/9/16.
//  Copyright © 2016 vaporstack. All rights reserved.
//

#include "drw_render_gl.h"

#include "drw_config.h"
#include "drw_platform.h"
#include <drw/drw.h>


#include <gpc/gpc.h>
#include "ext/drw_ext_gpc.h"


//#include <r4/src/r4_config.h>
//#include <r4/src/r4_platform.h>

#ifndef DRW_PLATFORM_IOS
#define ENABLE_OLD_HACKY_GLUT
#endif

#ifdef ENABLE_OLD_HACKY_GLUT

#ifdef DRW_PLATFORM_DARWIN
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#endif

#ifdef DRW_PLATFORM_IOS

//#include "../core/r_app_ios.h"

//#include <OpenGLES/ES1/gl.h>
//#include <OpenGLES/ES3/gl.h>

#include "type/drw_font_ftgles.h"
#include <glulookat/gluLookAt.h>
#else
#ifdef DRW_PLATFORM_DARWIN
//#include <OpenGL/gl.h>
//#include "type/drw_font_ftgl.h"
//#undef DRW_PLATFORM_IOS
#else

#ifdef DRW_PLATFORM_WIN
#include "src/gluLookAt.h"
//#include <GL/gl.h>
#include <windows.h>
/*

 typedef void (APIENTRY * PFNGLBLENDEQUATIONPROC) (GLenum mode);
 PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;
 */
#endif

//#include <GLFW/glfw3.h>
//#include <GL/glut.h>

#include <gl-matrix/gl-matrix.h>

//#include <GL/glew.h>
//#include "type/drw_font_ftgl.h"

#include "hacks/drw_snoop.h"

//#undef DRW_PLATFORM_IOS
#endif
#endif

#define TEXT_MAX 1024

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>

//  these things used to be part of the app, have to talk now from across
// the sea
static bool _ortho;
static bool _screenspace;
static int  _text_size;

static double _left       = -1;
static double _right      = -1;
static double _top	= -1;
static double _bottom     = -1;
static double _near       = -1;
static double _far	= -1;
static int    _prev_blend = 0;
static int    _blend      = 0;
static double fov	 = 45;
static double window_width;
static double window_height;

static double framebuffer_width;
static double framebuffer_height;

static bool color_bypass = false;
//#include <r4/r4.h>

//  NOPE this belongs to a parent library.
// extern DebugSettings debug_settings;

// typedef unsigned int uint;

//#include <r4/r4.h>

// extern AppSettings app_settings;

#define MAX_CIRCLE_PRECISION 512
#define PRECISION_WARNING_LIMIT 128
static int override_circle_limit = PRECISION_WARNING_LIMIT;

static float* circle_defs[MAX_CIRCLE_PRECISION];
static int    circle_precision = 12;

// static double scale_factor_retina = 1;
//  these will be leaving
static double scale_factor_downsample = 1;
static double scale_factor	    = 1;

// replaced by these
static bool   _landscape    = false;
static double _aspect       = 1;
static double _retina_scale = 1;

static int  axis_radius = 64;
static bool fill	= false;
static bool prev_fill   = false;

static float  _r, _g, _b, _a;
static WColor prev;
static float  prev_alpha;
//  data

static double alpha_mult = 1.;

// This will identify our vertex buffer
GLuint vertexbuffer;

// static bool initted = false;

/*

 static const GLfloat g_vertex_buffer_data[] = {
 -1.0f,-1.0f,-1.0f, // triangle 1 : begin
 -1.0f,-1.0f, 1.0f,
 -1.0f, 1.0f, 1.0f, // triangle 1 : end
 1.0f, 1.0f,-1.0f, // triangle 2 : begin
 -1.0f,-1.0f,-1.0f,
 -1.0f, 1.0f,-1.0f, // triangle 2 : end
 1.0f,-1.0f, 1.0f,
 -1.0f,-1.0f,-1.0f,
 1.0f,-1.0f,-1.0f,
 1.0f, 1.0f,-1.0f,
 1.0f,-1.0f,-1.0f,
 -1.0f,-1.0f,-1.0f,
 -1.0f,-1.0f,-1.0f,
 -1.0f, 1.0f, 1.0f,
 -1.0f, 1.0f,-1.0f,
 1.0f,-1.0f, 1.0f,
 -1.0f,-1.0f, 1.0f,
 -1.0f,-1.0f,-1.0f,
 -1.0f, 1.0f, 1.0f,
 -1.0f,-1.0f, 1.0f,
 1.0f,-1.0f, 1.0f,
 1.0f, 1.0f, 1.0f,
 1.0f,-1.0f,-1.0f,
 1.0f, 1.0f,-1.0f,
 1.0f,-1.0f,-1.0f,
 1.0f, 1.0f, 1.0f,
 1.0f,-1.0f, 1.0f,
 1.0f, 1.0f, 1.0f,
 1.0f, 1.0f,-1.0f,
 -1.0f, 1.0f,-1.0f,
 1.0f, 1.0f, 1.0f,
 -1.0f, 1.0f,-1.0f,
 -1.0f, 1.0f, 1.0f,
 1.0f, 1.0f, 1.0f,
 -1.0f, 1.0f, 1.0f,
 1.0f,-1.0f, 1.0f
 };
 */

int drw_get_gl_error()
{

	GLenum e = glGetError();
	// printf("err: %d\n", e);

	switch (e)
	{
	case GL_NO_ERROR:
		// printf("cool\n");
		break;

	case GL_INVALID_ENUM:
		printf("GL_INVALID_ENUM\n");
		break;

	case GL_INVALID_VALUE:
		printf("GL_INVALID_VALUE\n");

		break;

	case GL_INVALID_OPERATION:
		printf("GL_INVALID_OPERATION\n");
		break;

	// case GL_INVALID_FRAMEBUFFER_OPERATION:
	//    printf("invalid framebuffer\n");
	//    break;

	case GL_OUT_OF_MEMORY:
		printf("out of memory\n");
		break;

	case GL_STACK_UNDERFLOW:
		printf("underflow\n");
		break;
	case GL_STACK_OVERFLOW:
		printf("overflow\n");
		break;

	default:
		break;
	}

	return e;
}

void drw_init()
{
	for (int i = 0; i < MAX_CIRCLE_PRECISION; i++)
	{
		circle_defs[i] = NULL;
	}
	// glBlendEquation =
	// (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");

	// GLenum error = glGetError();
	// printf("err0: %d\n", error);

	glEnableClientState(GL_VERTEX_ARRAY);
	//  todo: figure out why this becomes fucking 5 when toggling fullscreen

	drw_get_gl_error();
	glEnable(GL_BLEND);
#ifndef DRW_PLATFORM_WIN
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

	//  MISH MASH OF CRAP FROM BEFORE THAT I DON'T FULLY GET
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);

	glEnableClientState(GL_VERTEX_ARRAY);
#ifndef DRW_PLATFORM_WIN
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

#ifndef DRW_PLATFORM_IOS
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
#endif

	drw_calculate_scale();

	drw_setup_view();

	// const char* path = r_resource_load("ruminant","ttf");
	// if ( debug_settings.render )
	//  printf("loading font, app scale is %f\n",
	//  app_settings.scale_retina);

	// 0if ( path != NULL )
	//{
	// const char* path = r_resource_load("vs-junk","otf");
	//  r_font_load(path);
	//    drw_font_size(72 * app_settings.scale_retina);
	// }

	drw_get_gl_error();
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	// glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	// glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	// glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
	// g_vertex_buffer_data, GL_STATIC_DRAW);  glBindBuffer(0,
	// vertexbuffer);
	drw_clear();
}

void drw_deinit(void)
{
	for (int i = 0; i < MAX_CIRCLE_PRECISION; i++)
	{
		float* circle = circle_defs[i];
		if (circle)
		{
			free(circle);
		}
	}
}

double drw_query_retina(void)
{
	return _retina_scale;
}

void drw_query_framebuffer(int* w, int* h)
{
	*w = framebuffer_width;
	*h = framebuffer_height;
}

double drw_query_aspectratio(void)
{
	return _aspect;
}

bool drw_query_landscape(void)
{
	return _landscape;
}

void drw_set_line_width(float v)
{
	glLineWidth(v);

	drw_get_gl_error();
}

void drw_text_load(const char* path)
{

// 0if ( path != NULL )
//{
// const char* path = r_resource_load("vs-junk","otf");
#ifdef DRW_ENABLE_FTGL
	drw_font_load(path);

#endif
#ifdef DRW_ENABLE_FTGLES
	drw_font_load(path);
#endif

	// drw_font_size(72);
	// }
}

void drw_blend_set(int v)
{

	_blend = v;
#ifndef DRW_PLATFORM_WIN

	switch (v)
	{
	case 0:
		glDisable(GL_BLEND);
	case R4_BLEND_MODE_NORMAL:
		// printf("normal blend!\n");
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case R4_BLEND_MODE_ADD:
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	case R4_BLEND_MODE_MULTIPLY:
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case R4_BLEND_MODE_SCREEN:

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
		break;
	case R4_BLEND_MODE_SUBTRACT:
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	default:
		printf("Unhandled blend mode!\n");
		break;
	}
#else
	printf("Blend modes not currently implemented in windows.\n");
#endif
}

void drw_blend_push(void)
{
	_prev_blend = _blend;
}

void drw_blend_pop(void)
{
	drw_blend_set(_prev_blend);
	_blend = _prev_blend;
}

void drw_clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

RColor8 drw_checkcolor(void)
{
	RColor8 c;
	c.r = _r;
	c.g = _g;
	c.b = _b;
	c.a = _a;
	return c;
}

void drw_color_clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void drw_color_clear_color(RColor c)
{
	drw_color_clear(c.r, c.g, c.b, c.a);
}

void drw_color_clear_color8(RColor8 c)
{
	drw_color_clear(c.r, c.g, c.b, c.a);

	//glClearColor(c.r, c.g, c.b, c.a);
}

void drw_color_clear_color16(RColor16 c)
{
	drw_color_clear(c.r, c.g, c.b, c.a);
	//glClearColor(c.r, c.g, c.b, c.a);
}

void _set_internal_colors(float r, float g, float b, float a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void drw_set_colorbypass(bool v)
{
	color_bypass = v;
}

void drw_color(double r, double g, double b, double a)
{
	_set_internal_colors(r, g, b, a);
#ifndef DRW_PLATFORM_IOS
	glColor4d(r, g, b, a);
#else
	glColor4f(r, g, b, a);
#endif
}

void drw_color4f(float r, float g, float b, float a)
{
	_set_internal_colors(r, g, b, a);
	glColor4f(r, g, b, a);
}

void drw_color_c(RColor c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}

void drw_color_c8(RColor8 c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}

void drw_color_c16(RColor16 c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}

void drw_color_wc(WColor c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}

/*
void drw_color_wc8(WColor8 c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}
*/

void drw_color_wc16(WColor16 c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}
void drw_color3f(float r, float g, float b)
{
	_set_internal_colors(r, g, b, _a);
	glColor4f(r, g, b, _a);
}

void drw_color_u(float v)
{
	_set_internal_colors(v, v, v, 1.);
	glColor4f(v, v, v, 1.);
}

void drw_color_push()
{
	prev.r = _r;
	prev.g = _g;
	prev.b = _b;
	prev.a = _a;
}

void drw_color_pop()
{
	_set_internal_colors(prev.r, prev.g, prev.b, prev.a);

	glColor4f(prev.r, prev.g, prev.b, prev.a);
}

/*
 void drw_color_c(RColor* c)
 {
 _set_internal_colors(c->r, c->g, c->b, 1.0f);
 glColor4f(c->r,c->g,c->b,1.);
 }

 void drw_color_c8(RColor8* c)
 {
 _set_internal_colors(c->r, c->g, c->b, c->a);
 glColor4f(c->r,c->g,c->b,c->a);
 }

 void drw_color_c16(RColor16* c)
 {
 _set_internal_colors(c->r, c->g, c->b, c->a);
 glColor4f(c->r,c->g,c->b,c->a);
 }
 */

void drw_alpha(float a)
{
	prev_alpha = a;
	glColor4f(_r, _g, _b, a);
}

void drw_alpha_pop()
{
	_a = prev_alpha;
	glColor4f(_r, _g, _b, _a);
}

#ifdef DRW_PLATFORM_IOS
#define MATRIX_STACK_MAX 16
#else
#define MATRIX_STACK_MAX 32
#endif

static int matrix_stack_count = 0;

void drw_alpha_mult(double v)
{
	alpha_mult = v;
}

int drw_checkmatrix(void)
{
	return matrix_stack_count;
}

void drw_alpha_mult_pop()
{
	alpha_mult = 1;
}

void drw_push()
{
	matrix_stack_count++;
	if (matrix_stack_count > MATRIX_STACK_MAX)
	{
		printf("Matrix overflowed! %d\n", matrix_stack_count);
	}
	glPushMatrix();
}

void drw_pop()
{
	matrix_stack_count--;
	if (matrix_stack_count < 0)
	{
		printf("Matrix underflowed!\n");
	}
	glPopMatrix();
}

void drw_rotate_x(float r)
{
	glRotatef(r, 1, 0, 0);
}

void drw_rotate_y(float r)
{
	glRotatef(r, 0, 1, 0);
}

void drw_rotate_z(float r)
{
	glRotatef(r, 0, 0, 1);
}

void drw_rotate(float x, float y, float z)
{
	glRotatef(x, 1, 0, 0);
	glRotatef(y, 0, 1, 0);
	glRotatef(z, 0, 0, 1);
}

#pragma mark primitives

void drw_line_r(RLine* poly)
{

	const unsigned long long renderLineSize = poly->num * 2;

	// printf("poly is %d num\n", poly->num);
	// GLfloat arr[ renderLineSize ];
	GLfloat* arr = malloc(sizeof(GLfloat) * renderLineSize);
	int      i, j;
	for (i = 0, j = 0; i < poly->num; i++, j += 2)
	{
		RPoint* p  = &poly->data[i];
		arr[j]     = p->x;
		arr[j + 1] = p->y;
	}

	glVertexPointer(2, GL_FLOAT, 0, arr);

	(poly->closed) ? glDrawArrays(GL_LINE_LOOP, 0, (int)poly->num) : glDrawArrays(GL_LINE_STRIP, 0, (int)poly->num);
	free(arr);
}

void drw_line_3f(float ax, float ay, float az, float bx, float by, float bz)
{
	const GLfloat renderLine[] = {ax, ay, az, bx, by, bz};
	glVertexPointer(3, GL_FLOAT, 0, renderLine);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void drw_line3_r(RLine3* poly)
{

	const unsigned long long renderLineSize = (poly->num * 3);

	// printf("poly is %d num\n", poly->num);
	// GLfloat arr[ renderLineSize ];
	GLfloat* arr = malloc(sizeof(GLfloat) * renderLineSize);

	int i, j;

	for (i = 0, j = 0; i < poly->num; i++, j += 3)
	{
		RPoint3* p = &poly->data[i];
		arr[j]     = p->x;
		arr[j + 1] = p->y;
		arr[j + 2] = p->z;
	}

	glVertexPointer(3, GL_FLOAT, 0, arr);

	glDrawArrays(GL_LINE_STRIP, 0, (int)poly->num);
	free(arr);
}

void drw_line(float ax, float ay, float bx, float by)
{
	const GLfloat renderLine[] = {ax, ay, bx, by};
	glVertexPointer(2, GL_FLOAT, 0, renderLine);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void drw_line_rp(RPoint a, RPoint b)
{
	const GLfloat renderLine[] = {a.x, a.y, b.x, b.y};
	glVertexPointer(2, GL_FLOAT, 0, renderLine);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void drw_line_wp(WPoint a, WPoint b)
{
	const GLfloat renderLine[] = {a.x, a.y, b.x, b.y};
	glVertexPointer(2, GL_FLOAT, 0, renderLine);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void drw_tri(float ax, float ay, float bx, float by, float cx, float cy)
{
	const GLfloat renderLine[] = {ax, ay, bx, by, cx, cy};
	glVertexPointer(2, GL_FLOAT, 0, renderLine);
	glDrawArrays(GL_LINE_STRIP, 0, 3);
}

void drw_scale_2f(float x, float y)
{
	glScalef(x, y, 1);
}

void drw_scale(float x, float y, float z)
{
	glScalef(x, y, z);
}

void drw_scale_u(float v)
{
	glScalef(v, v, v);
}

void drw_scale_x(float x)
{
	glScalef(x, 1, 1);
}

void drw_scale_y(float y)
{
	glScalef(1, y, 1);
}

void drw_scale_z(float z)
{
	glScalef(1, 1, z);
}

/*
 void drw_translate_vec2(Vec2* v)
 {
 glTranslatef(v->x, v->y, 0);
 }

 */
void drw_translate_vec3(vec3_t v)
{
	glTranslatef(v[0], v[1], v[2]);
}

void drw_translate_x(float v)
{
	glTranslatef(v, 0, 0);
}

void drw_translate_y(float v)
{
	glTranslatef(0, v, 0);
}

void drw_translate_z(float v)
{
	glTranslatef(0, 0, v);
}

void drw_translate2f(float x, float y)
{
	glTranslatef(x, y, 0);
}

void drw_translate(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void drw_translate_rp(RPoint p)
{
	glTranslatef(p.x, p.y, 0);
}

void drw_translate_rp3(RPoint3 p)
{
	glTranslatef(p.x, p.y, p.z);
}

void drw_translate3f(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void drw_gtransform_apply(GTransform t)
{
	drw_translate_vec3(t.position);
	drw_translate_vec3(t.anchor);
	drw_scale_vec3(t.scale);
	drw_rotate_vec3(t.rotation);
}

void drw_transform_apply(WTransform t)
{
	drw_translate_wvec(t.position);
	drw_translate_wvec(t.anchor);
	drw_scale_wvec(t.scale);
	drw_rotate_wvec(t.rotation);
}

void drw_transform_apply_inverse(WTransform t)
{
	drw_rotate_wvec_inverse(t.rotation);
	drw_scale_wvec_inverse(t.scale);
	drw_translate_wvec_inverse(t.anchor);
	drw_translate_wvec_inverse(t.position);
}

void drw_transform_deapply(WTransform t)
{
	/*drw_rotate_wvec(t.rotation);

	drw_translate_wvec(t.position);
	drw_translate_wvec(t.anchor);
	drw_scale_wvec(t.scale);
*/
}

void drw_gtransform_deapply(GTransform t)
{
}

#ifdef DRW_PLATFORM_IOS
#define glTranslated glTranslatef
#define glScaled glScalef
#define glRotated glRotatef
#endif

void drw_translate_wvec(WVec3d v)
{
	glTranslated(v.x, v.y, v.z);
}

void drw_scale_wvec(WVec3d v)
{
	// printf("%f %f %f\n", v.x, v.y, v.z);
	glScaled(v.x, v.y, v.z);
}

void drw_rotate_wvec(WVec3d v)
{
	glRotated(v.x, 1, 0, 0);
	glRotated(v.y, 0, 1, 0);
	glRotated(v.z, 0, 0, 1);
}
void drw_translate_wvec_inverse(WVec3d v)
{
	glTranslated(-v.x, -v.y, -v.z);
}

void drw_scale_wvec_inverse(WVec3d v)
{
	// printf("%f %f %f\n", v.x, v.y, v.z);
	glScaled(-v.x, -v.y, -v.z);
}

void drw_rotate_wvec_inverse(WVec3d v)
{
	glRotated(-v.x, 1, 0, 0);
	glRotated(-v.y, 0, 1, 0);
	glRotated(-v.z, 0, 0, 1);
}
void drw_scale_vec3(vec3_t v)
{
	// printf("%f %f %f\n", v.x, v.y, v.z);
	glScaled(v[0], v[1], v[2]);
}

void drw_rotate_vec3(vec3_t vec)
{
	drw_rotate(vec[0], vec[1], vec[2]);
}

void drw_axis_nice()
{
	float x = axis_radius * scale_factor;

	drw_push();
	drw_line(x, 0, x * 2, 0);
	drw_rotate_z(120.0);
	drw_line(x, 0, x * 2, 0);
	drw_rotate_z(120.0);
	drw_line(x, 0, x * 2, 0);
	drw_rotate_z(120.0);
	drw_pop();
}

int drw_text_get_size(void)
{
	return _text_size;
}

void drw_text_set_size(int sz, int resolution)
{
#ifdef DRW_ENABLE_FTGL
	drw_font_size(sz, resolution);
#endif
	_text_size = sz;
}

void drw_text(const char* format, ...)
{
	char buf[TEXT_MAX];
	sprintf(buf, "%s", format);
	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

#ifdef DRW_ENABLE_FTGL

	drw_font_draw(buf);
#endif
}

/*
void d_wsequence_e(WSequence* seq, int frame)
{

	// TODO
	// implement this.
	//
	// WObject* fr = seq->frames[frame];

	//  do stuff here.
}
*/

void drw_robject(RObject* obj)
{

	size_t num = obj->num;

	//RLine* l = r_line_create();
	int i;
	for (i = 0; i < num; ++i)
	{

		//vector_get(&obj->lines, i, l);
		RLine* l = obj->lines[i];
		if (!l)
		{
			printf("ack!\n");
			return;
		}

		drw_rline(l);
	}
}

void drw_wobject_normal(WObject* obj)
{
	// drw_color(1,1,1,.9);
	drw_push();
	drw_translate(obj->transform.position.x, obj->transform.position.y,
		      obj->transform.position.z);

	drw_scale_2f(obj->transform.scale.x, obj->transform.scale.y);
	int i;

	for (i = 0; i < obj->num; ++i)
	{
		WLine* l = obj->lines[i];
		if (!l)
		{
			printf("ack!\n");
			return;
		}

		drw_poly(l);
	}

	drw_pop();
}

void drw_wobject_naive(WObject* obj)
{

	drw_rect_w(obj->bounds);

	if (!obj)
	{
		printf("Error, tried to render a null obj!\n");
		return;
	}
	if (!obj->lines)
	{
		printf("no lines either!?\n");
		return;
	}

	int i;

	for (i = 0; i < obj->num; ++i)
	{
		WLine* l = obj->lines[i];
		if (!l)
		{
			printf("ack!\n");
			return;
		}

		drw_poly(l);
	}
}

void drw_wobject_e(WObject* obj)
{

	if (obj->normalized)
	{
		drw_wobject_normal(obj);
	}
	else
	{
		drw_wobject_naive(obj);
	}
}

void drw_wline_fill(WLine* l)
{
	drw_set_fill(true);
	drw_poly(l);
	drw_pop_fill();
}

/*
 static void _wlinepoly(WLine* line)
{
}
*/

void drw_wline_strokeonly(WLine* l)
{
	if (l == NULL)
	{
		printf("Tried to render a null line!\n");
		return;
	}

	drw_poly(l);
}


void drw_tess(void* tess)
{
	drw_gpc_tristrip(tess);

}

void drw_wline(WLine* l)
{
	if (l == NULL)
	{
		printf("Tried to render a null line!\n");
		return;
	}

	if (!color_bypass)
	{
		if (l->has_stroke)
		{
			WColor16 c = l->stroke;
			drw_color(c.r, c.g, c.b, c.a * alpha_mult);
			/// dirty hack to have color and also transparent onion
			/// skins oops
			//r_alpha(c.a * alpha_mult);
		}
		if (l->closed)
		{
			drw_set_fill(l->closed);
			drw_poly(l);
			drw_pop_fill();
		}
		if (l->has_stroke)
		{
			//r_alpha_pop();
		}
	}
	else
	{
	}
#ifdef DISABLE_UNTIL_WORKLINE_REFACTOR_COMPLETE

	if (l->brush)
	{
		if (l->tess)
		{
			drw_gpc_tristrip(l->tess);
			return;
		}

		if (l->brush->stroke)
		{
			if (l->brush->stroke->tess)
			{
				drw_gpc_tristrip(l->brush->stroke->tess);
			}
			else
			{

				drw_triangle_strip(l->brush->stroke);
			}
		}
		// else{

		//  drw_poly(l->brush->stroke);
		//}
	}
	else
	{

		if (l->tess)
		{
			drw_gpc_tristrip(l->tess);
		}
		else
		{
			drw_poly(l);
		}
	}
#else
	drw_poly(l);
#endif
	//  TODO color pop?
}

void drw_wobject(WObject* obj)
{

	// drw_rect_w(obj->bounds);
	if (!obj)
	{
		printf("Error, tried to render a null obj!\n");
		return;
	}
	if (!obj->lines)
	{
		// printf("no lines either!?\n");
		return;
	}

	drw_push();
	drw_transform_apply(obj->transform);
	int i;
	for (i = 0; i < obj->num; ++i)
	{

		WLine* l = obj->lines[i];
		if (!l)
			continue;

		if (!l)
		{
			printf("ack!\n");
			continue;
		}
		if (!l->data)
		{
			printf("AAACK\n");
			continue;
		}
		drw_wline(l);
	}
	drw_pop();
}

void drw_wobject_notransform(WObject* obj)
{

	// drw_rect_w(obj->bounds);

	if (!obj)
	{
		printf("Error, tried to render a null obj!\n");
		return;
	}
	if (!obj->lines)
	{
		// printf("no lines either!?\n");
		return;
	}

	drw_push();
	//drw_transform_apply(obj->transform);
	int i;
	for (i = 0; i < obj->num; ++i)
	{

		WLine* l = obj->lines[i];
		if (!l)
			continue;

		if (!l)
		{
			printf("ack!\n");
			return;
		}

		drw_wline(l);
	}
	drw_pop();
}

void drw_wobject_strokeonly(WObject* obj)
{

	// drw_rect_w(obj->bounds);

	if (!obj)
	{
		printf("Error, tried to render a null obj!\n");
		return;
	}
	if (!obj->lines)
	{
		// printf("no lines either!?\n");
		return;
	}

	drw_push();
	drw_transform_apply(obj->transform);
	int i;
	for (i = 0; i < obj->num; ++i)
	{

		WLine* l = obj->lines[i];
		if (!l)
			continue;

		if (!l)
		{
			printf("ack!\n");
			return;
		}

		drw_wline_strokeonly(l);
	}
	drw_pop();
}

void drw_wobject_strokeonly_notransform(WObject* obj)
{

	// drw_rect_w(obj->bounds);

	if (!obj)
	{
		printf("Error, tried to render a null obj!\n");
		return;
	}
	if (!obj->lines)
	{
		// printf("no lines either!?\n");
		return;
	}

	drw_push();
	//drw_transform_apply(obj->transform);
	int i;
	for (i = 0; i < obj->num; ++i)
	{

		WLine* l = obj->lines[i];
		if (!l)
			continue;

		if (!l)
		{
			printf("ack!\n");
			return;
		}

		drw_wline_strokeonly(l);
	}
	drw_pop();
}

void drw_verts_r(RLine* l)
{
	int i;
	for (i = 0; i < l->num; ++i)
	{
		RPoint* p = &l->data[i];
		drw_push();
		drw_translate2f(p->x, p->y);
		double pv = 10;
		drw_square(pv);
		drw_pop();
	}
}

void drw_verts(WLine* l)
{
	int i;
	for (i = 0; i < l->num; ++i)
	{
		WPoint* p = &l->data[i];
		drw_push();
		drw_translate2f(p->x, p->y);
		double pv = p->pressure;
		pv *= 10;
		drw_square(pv);
		drw_pop();
	}
}

void drw_wobject_verts(WObject* obj)
{
	int i;
	for (i = 0; i < obj->num; i++)
	{
		WLine* line = obj->lines[i];
		if (!line)
		{
			printf("Error, bogus line!\n");
			continue;
		}
		drw_verts(line);
	}
}

void drw_axis()
{
	/// ones here used to be scale factor
	int axis_render_radius = axis_radius * scale_factor;
	// drw_push();
	// drw_translate2f(x * 1,y * 1);
	drw_line(-axis_render_radius, 0, axis_render_radius, 0);
	drw_line(0, -axis_render_radius, 0, axis_render_radius);
	// drw_pop();
}

void drw_axis_living()
{
	static float r			= 0.f;
	static float speed		= .01f;
	float	axis_render_radius = axis_radius * 1;

	//axis_render_radius = axis_radius * 1;
	r += speed;
	// drw_push();
	// drw_translate2f(x * 1,y * 1);
	drw_rotate_z(r * 90);
	drw_line(-axis_render_radius, 0, axis_render_radius, 0);
	drw_line(0, -axis_render_radius, 0, axis_render_radius);
	drw_rotate_z(-r * 90);
	// drw_pop();
}
/*

 void drw_rect_r(RRect r )
 {
 	drw_rect(r.pos.x, r.pos.y, r.pos.x + r.size.x, r.pos.y + r.size.y);

 }
 */

void drw_rect_w(WRect r)
{
	drw_rect(r.pos.x, r.pos.y, r.pos.x + r.size.x, r.pos.y + r.size.y);
}

void drw_rect_r(RRect r)
{
	drw_rect(r.pos.x, r.pos.y, r.pos.x + r.size.x, r.pos.y + r.size.y);
}

void drw_rect_wp(WPoint a, WPoint b)
{
	float arr[8];
	arr[0] = a.x;
	arr[1] = a.y;
	arr[2] = b.x;
	arr[3] = a.y;
	arr[4] = b.x;
	arr[5] = b.y;
	arr[6] = a.x;
	arr[7] = b.y;

	glVertexPointer(2, GL_FLOAT, 0, &arr);
	fill ? glDrawArrays(GL_TRIANGLE_FAN, 0, 4)
	     : glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void drw_rect_rp(RPoint a, RPoint b)
{
	float arr[8];
	arr[0] = a.x;
	arr[1] = a.y;
	arr[2] = b.x;
	arr[3] = a.y;
	arr[4] = b.x;
	arr[5] = b.y;
	arr[6] = a.x;
	arr[7] = b.y;

	glVertexPointer(2, GL_FLOAT, 0, &arr);
	fill ? glDrawArrays(GL_TRIANGLE_FAN, 0, 4)
	     : glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void drw_rect(float ax, float ay, float bx, float by)
{
	float arr[8];
	arr[0] = ax;
	arr[1] = ay;
	arr[2] = bx;
	arr[3] = ay;
	arr[4] = bx;
	arr[5] = by;
	arr[6] = ax;
	arr[7] = by;

	glVertexPointer(2, GL_FLOAT, 0, &arr);
	fill ? glDrawArrays(GL_TRIANGLE_FAN, 0, 4)
	     : glDrawArrays(GL_LINE_LOOP, 0, 4);

	/*
#ifdef DRW_ENABLE_SNOOP
	if (drw_snoop_get())
	{
		RLine* nl = drw_snoop_rline_from_f(arr, 8);
		drw_snoop_add(nl);
	}
#endif
	*/
}

void drw_point(void)
{
	float arr[2];
	glVertexPointer(2, GL_FLOAT, 0, &arr);
	glDrawArrays(GL_POINTS, 0, 4);
	//  printf("~");
}

void drw_square(float r)
{
	drw_rect(r * -.5, r * -.5, r * .5, r * .5);
}

static void calculate_circle(int sides, int radius)
{
	float deg2rad = (M_PI / circle_precision);

	const int renderLineSize = (circle_precision * 2);

	GLfloat* arr = calloc(renderLineSize, sizeof(GLfloat));
	// GLfloat circleArray[ renderLineSize ];
	int i;
	for (i = 0; i < renderLineSize; i += 2)
	{
		float degInRad = deg2rad * (float)i;
		float x	= cos(degInRad + M_PI * .5) * radius;
		float y	= sin(degInRad + M_PI * .5) * radius;
		arr[i]	 = x;
		arr[i + 1]     = y;
	}
	circle_defs[sides] = arr;
}

void drw_circle(float r)
{
	drw_ellipse(r, r);
	/*
	float* circle = circle_defs[circle_precision];
	if (circle == NULL) {
		printf("calculating circle for %i\n", circle_precision);
		calculate_circle(circle_precision);
	}
	circle = circle_defs[circle_precision];
	glVertexPointer(2, GL_FLOAT, 0, circle);

	// cout << "c:" << circleArray[2] << endl;
	drw_scale_u(r);
	fill ? glDrawArrays(GL_TRIANGLE_FAN, 0, circle_precision)
	     : glDrawArrays(GL_LINE_LOOP, 0, circle_precision);

	//r_ellipse(r, r);
*/
}

void drw_ellipse(float _x, float _y)
{
	/*if ( _x == _y )
	{
		drw_circle(_x);
		return;
	}*/

	// glEnableClientState( GL_VERTEX_ARRAY );

	// glEnable(GL_BLEND);
	// glBlendEquation(GL_FUNC_ADD);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float deg2rad = (M_PI / circle_precision);

	const int renderLineSize = (circle_precision * 2);

	GLfloat* arr = malloc(sizeof(GLfloat) * renderLineSize);
	// GLfloat circleArray[ renderLineSize ];
	int i;
	for (i = 0; i < renderLineSize; i += 2)
	{
		float degInRad = deg2rad * (float)i;
		float x	= cos(degInRad + M_PI * .5) * (_x)*scale_factor;
		float y	= sin(degInRad + M_PI * .5) * (_y)*scale_factor;
		arr[i]	 = x;
		arr[i + 1]     = y;
	}
	/*
#ifdef DRW_ENABLE_SNOOP
	if (drw_snoop_get())
	{
		drw_snoop_add(drw_snoop_rline_from_f(arr, renderLineSize));
	}
#endif
	*/
	glVertexPointer(2, GL_FLOAT, 0, arr);

	fill ? glDrawArrays(GL_TRIANGLE_FAN, 0, circle_precision)
	     : glDrawArrays(GL_LINE_LOOP, 0, circle_precision);

	free(arr);
}

void drw_rline(RLine* poly)
{

#ifdef DRW_ENABLE_SNOOP
	if (drw_snoop_get())
	{
		drw_snoop_add(poly);
	}
#endif

	const unsigned long long renderLineSize = (poly->num * 2);

	GLfloat* arr = calloc(renderLineSize, sizeof(GLfloat));

	int i, j;
	for (i = 0, j = 0; i < poly->num; i++, j += 2)
	{
		RPoint* p  = &poly->data[i];
		arr[j]     = p->x;
		arr[j + 1] = p->y;
	}

	glVertexPointer(2, GL_FLOAT, 0, arr);
	if (fill)
	{
		glDrawArrays(GL_TRIANGLE_FAN, 0, (int)poly->num);
	}
	else
	{
		(poly->closed) ? glDrawArrays(GL_LINE_LOOP, 0, (int)poly->num) : glDrawArrays(GL_LINE_STRIP, 0, (int)poly->num);
	}
	free(arr);
}

void drw_poly(WLine* line)
{

#ifdef DRW_ENABLE_SNOOP
	if (drw_snoop_get())
	{
		drw_snoop_add(r_line_from_wline(line));
	}
#endif

	int			 i, j;
	const unsigned long long renderLineSize = (line->num * 2);

	GLfloat* arr = malloc(sizeof(GLfloat) * renderLineSize);

	for (i = 0, j = 0; i < line->num; i++, j += 2)
	{
		WPoint* p = &line->data[i];
		//  todo: REMOVE THIS HACK

		arr[j]     = p->x;
		arr[j + 1] = p->y;
	}

	glVertexPointer(2, GL_FLOAT, 0, arr);

	if (line->closed)
	{
		glDrawArrays(GL_TRIANGLE_FAN, 0, (int)line->num);
		free(arr);
		return;
	}

	fill ? glDrawArrays(GL_TRIANGLE_FAN, 0, (int)line->num)
	     : glDrawArrays(GL_LINE_STRIP, 0, (int)line->num);
	free(arr);
}

void drw_poly_extras(WLine* line)
{
	if (!line)
	{
		printf("wat");
		return;
	}

	if (line->num < 2)
	{
		// if ( debug_settings.render )
		printf("Can't draw a poly with a single point!\n");
		return;
	}

	if (line->closed)
	{
		drw_set_fill(line->closed);
		drw_poly(line);
		drw_pop_fill();
	}
#ifdef DISABLE_UNTIL_WORKLINE_REFACTOR_COMPLETE

	if (line->brush)
	{
		// w_brush_draw(line->brush);
	}
	else
	{
		// if (!line->closed)
		drw_set_fill(line->closed);
		drw_poly(line);
		drw_pop_fill();
	}

	if (line->tess)
	{
		drw_gpc_tristrip(line->tess);
	}
#endif
}

void drw_tris_3d(double* arr, int num, bool closed)
{

	glVertexPointer(3, DRW_VERTEX_POINTER_IDENT, 0, arr);

	glDrawArrays(GL_TRIANGLES, 0, num);
	free(arr);
}

void drw_rawpoly_2d(double* arr, int num, bool closed)
{

	glVertexPointer(2, DRW_VERTEX_POINTER_IDENT, 0, arr);

	if (fill)
	{
		glDrawArrays(GL_TRIANGLE_FAN, 0, (int)num * .5);
	}
	else
	{
		closed ? glDrawArrays(GL_LINE_LOOP, 0, (int)num * .5)
		       : glDrawArrays(GL_LINE_STRIP, 0, (int)num * .5);
	}
}

void drw_rawpoly_2f(float* arr, int num, bool closed)
{

	glVertexPointer(2, GL_FLOAT, 0, arr);

	if (fill)
	{
		glDrawArrays(GL_TRIANGLE_FAN, 0, (int)num * .5);
	}
	else
	{
		closed ? glDrawArrays(GL_LINE_LOOP, 0, (int)num * .5)
		       : glDrawArrays(GL_LINE_STRIP, 0, (int)num * .5);
	}
}

void drw_rawpoly_3d(double* arr, int num, bool closed)
{

	glVertexPointer(3, DRW_VERTEX_POINTER_IDENT, 0, arr);

	if (fill)
	{
		glDrawArrays(GL_TRIANGLE_FAN, 0, num);
	}
	else
	{
		closed ? glDrawArrays(GL_LINE_LOOP, 0, (int)num * .5)
		       : glDrawArrays(GL_LINE_STRIP, 0, (int)num * .5);
	}
}

void drw_rawpoly_3f(float* arr, int num, bool closed)
{

	glVertexPointer(3, GL_FLOAT, 0, arr);

	if (fill)
	{
		glDrawArrays(GL_TRIANGLE_FAN, 0, (int)num);
	}
	else
	{
		closed ? glDrawArrays(GL_LINE_LOOP, 0, (int)num)
		       : glDrawArrays(GL_LINE_STRIP, 0, (int)num);
	}
}

void drw_rawpoints_3d(double* arr, int num)
{

	glVertexPointer(3, DRW_VERTEX_POINTER_IDENT, 0, arr);
	glDrawArrays(GL_POINTS, 0, num);
}

void drw_point3f(float x, float y, float z)
{
	float arr[3] = {x, y, z};
	glVertexPointer(3, GL_FLOAT, 0, arr);
	glDrawArrays(GL_POINTS, 0, 3);
}

void drw_rawpoints_3f(float* arr, int num)
{

	glVertexPointer(3, GL_FLOAT, 0, arr);
	glDrawArrays(GL_POINTS, 0, num);
}

void drw_rgbtri(double gamma)
{
	GLint vertices[] = {0, 1, 0, 0, 0, 1, 1, 0, 0};

	static GLfloat colors[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};

	static GLubyte indices[] = {0, 1, 2};

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_INT, 0, vertices);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLubyte),
		       GL_UNSIGNED_BYTE, indices);

	drw_set_circle_precision(3);
	drw_set_fill(true);
	drw_circle(1);
	drw_pop_fill();
	glDisableClientState(GL_COLOR_ARRAY);
}

void drw_grid(float size, int subdiv)
{

	// drw_push();
	// drw_rotate_x(90);
	// drw_translate2f(size * -.5, size *-.5);
	double x1, y1, x2, y2;

	double sz = size / subdiv;
	int    y, x;

	for (y = 0; y <= subdiv; ++y)
	{
		x1 = 0;
		y1 = y * sz;
		x2 = size;
		y2 = y * sz;
		x1 -= size * .5;
		y1 -= size * .5;
		x2 -= size * .5;
		y2 -= size * .5;
		drw_line(x1, y1, x2, y2);
	}

	for (x = 0; x <= subdiv; ++x)
	{
		x1 = x * sz;
		y1 = 0;
		x2 = x * sz;
		y2 = size;

		x1 -= size * .5;
		y1 -= size * .5;
		x2 -= size * .5;
		y2 -= size * .5;
		drw_line(x1, y1, x2, y2);
	}

	// drw_pop();
}

void r_set_window(double w, double h)
{
	window_width  = w;
	window_height = h;

	drw_calculate_scale();
	drw_setup_view();
}

bool drw_get_ortho(void)
{
	return _ortho;
}

void drw_set_ortho(bool val)
{
#ifdef DEBUG
	printf("Setting ortho to %d\n", val);
#endif
	_ortho = val;
}

bool drw_get_screenspace(void)
{
	return _screenspace;
}

void drw_set_screenspace(bool val)
{
#ifdef DEBUG
	printf("Setting screenspace to %d\n", val);
#endif
	/*
	 if ( val ){

	 printf("yes\n");
	 }else{
	 printf("no\n");
	 }*/

	_screenspace = val;

	//r_set_ortho(val);
}

void drw_setup_view(void)
{
	if (_ortho)
	{
		drw_setup_view_ortho();
	}
	else
	{

		drw_setup_view_persp();
	}

	int bits;
	glGetIntegerv(GL_RED_BITS, &bits); //(GL_RED_BITS);
	printf("Bits is %d\n", bits);
}

#include <glulookat/gluLookAt.h>

static void print_debug(void)
{
	char sc = (_screenspace) ? 'Y' : 'N';
	char o  = (_ortho) ? 'Y' : 'N';
	printf("SCRN %c  ORTHO %c  AR:%f\n", sc, o, _aspect);
}

void drw_setup_view_persp()
{
	// if(debug_settings.render)
	printf("PERSP\n");
	print_debug();

	// static int zoomFactor = 1;
	//float left, right, top, bottom, znear, zfar;
	glViewport(0, 0, framebuffer_width, framebuffer_height);
	//glViewport(-.5 * framebuffer_width * _retina_scale, -.5 * framebuffer_height* _retina_scale, .5 * framebuffer_width * _retina_scale, .5 * framebuffer_height* _retina_scale);

	float width, height;

	if (_screenspace)
	{
		//printf("screenspace: YES %f\n", _aspect);

		width  = framebuffer_width;
		height = framebuffer_height;

		if (width > height)
		{
			// landscape
			_aspect    = (float)width / (float)height;
			_landscape = true;
		}
		else
		{
			// portrait

			_aspect    = (float)height / (float)width;
			_landscape = false;
		}
	}
	else
	{
		if (framebuffer_width > framebuffer_height)
		{
			_aspect = framebuffer_width / framebuffer_height;
		}
		else
		{
			_aspect = framebuffer_height / framebuffer_width;
		}
		_landscape = (framebuffer_width >= framebuffer_height);
		//printf("screenspace: NO %f\n", _aspect);
		width  = 1.;
		height = 1.;
	}

	/// x = app_settings.current_window_x;
	// y = app_settings.current_window_y;

	// x = size.x;
	// y = size.y;

	// glfwGetFramebufferSize(window,&x, &y);

	// x *= _retina_scale;
	// y *= _retina_scale;
	// x *= app_settings.scale_retina;
	// y *= app_settings.scale_retina;

	// Select and setup the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Select and setup the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//float pixelAspect = 1.f;

	//bool landscape = true;

	// landscape
	//if (x > y)
	//{
	//	pixelAspect = (float)x / (float)y;
	//	_landscape  = true;
	//}

	// portrait
	//if (y > x)
	//{
	//	pixelAspect = (float)y / (float)x;
	//	_landscape  = false;
	//}
	double greater = (width > height) ? width : height;

	//_aspect = pixelAspect;
	if (_screenspace)
	{
		_left   = 0;
		_top    = 0;
		_right  = width;
		_bottom = height;
	}
	else
	{
		_left   = greater * -.5;
		_top    = greater * -.5;
		_right  = greater * .5;
		_bottom = greater * .5;
	}
	//_left = _top = _near = -1.f;
	//_right = _bottom = _far = 1.f;

	_near = -1024.f;
	_near = 0.00001f;
	_far  = 1024.f;

	//maybe this block can go entirely?
	if (_ortho)
	{
		if (_landscape)
		{
			_left *= _aspect;
			_right *= _aspect;
		}
		else
		{
			_top *= _aspect;
			_bottom *= _aspect;
		}
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

// glOrtho(left*zoomFactor, right*zoomFactor, top*zoomFactor, bottom*zoomFactor,
// near, far);  glFrustum(left,right,bottom,top,near, far);
//#ifdef DRW_PLATFORM_WIN

//	printf("glu stuff doesn't link on windows >:[\n");
//#else
/*
	 _l = left;
	_r = right;
	_t = top;
	_b = bottom;
	_n = znear;
	_f = zfar;
	*/

#ifndef DRW_PLATFORM_IOS
	gluPerspective(fov, _aspect, _near, _far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, height, 0, 0, 0.0, 0.0, _near, _far);
#else
	gluPerspective(fov, _aspect, _near, _far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, height, 0, 0, 0.0, 0.0, _near, _far);

#endif
}

//	this appears to work in both configurations.
void drw_setup_view_ortho()
{
	printf("ORTHO\n");
	print_debug();

	float width, height;

	width  = framebuffer_width;
	height = framebuffer_height;
	//double x = width;
	//double y = height;

	if (width > height)
	{
		// landscape
		_aspect    = (float)width / (float)height;
		_landscape = true;
	}
	else
	{
		// portrait
		_aspect    = (float)height / (float)width;
		_landscape = false;
	}

	glViewport(0, 0, (int)width, (int)height);

	if (!_screenspace)
	{
		width  = 1.;
		height = 1.;

		if (_landscape)
		{
			width *= _aspect;
		}
		else
		{
			height *= _aspect;
		}
	}

	drw_get_gl_error();

	glLoadIdentity();

	drw_get_gl_error();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	_left   = width * -.5;
	_right  = width * .5;
	_top    = height * -.5;
	_bottom = height * .5;
	_near   = 1048 * -8;
	_far    = 1024 * 8;

#ifdef DRW_PLATFORM_IOS
	glOrthof(_left, _right, _top, _bottom, _near, _far);
#else
	glOrtho(_left, _right, _top, _bottom, _near, _far);
#endif

	if (_screenspace)
	{
		glTranslatef(0.375f, 0.375f, 0.f);
	}

	drw_get_gl_error();
#ifndef DRW_PLATFORM_IOS
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#endif
}

void drw_get_screencoords(double* l, double* r, double* t, double* b, double* n, double* f)
{
#ifdef DEBUG
//printf("Sanity check: %f %f %f %f %f %f\n", _left, _right, _top, _bottom, _near, _far);
#endif
	*l = _left;
	*r = _right;
	*t = _top;
	*b = _bottom;
	*n = _near;
	*f = _far;
}

void drw_set_screensize(double w, double h)
{
	// printf("Setting renderer to %f %f\n", w, h);
	window_width  = w;
	window_height = h;

	// drw_calculate_scale();
	// drw_setup_view();
}
void drw_set_framebuffer(double w, double h)
{
	// printf("Setting framebuffer to %f %f\n", w, h);
	framebuffer_width  = w;
	framebuffer_height = h;

	drw_color(0, 0, 0, 0);
	// drw_calculate_scale();
	// drw_setup_view();
}

void drw_set_fov(double v)
{
	fov = v;
}

void drw_set_downsample(double v)
{
	scale_factor_downsample = v;
	drw_calculate_scale();
}

void drw_set_retina(double v)
{
	// if(debug_settings.render )
	// printf("Setting retina scale: %f\n", v);
	// scale_factor_retina = v;
	drw_calculate_scale();
	_retina_scale = v;
	glLineWidth(1);
}

void drw_calculate_scale()
{

	scale_factor = scale_factor_downsample * _retina_scale;
	/*
	 if(debug_settings.render )
	 {
	 char log[256];
	 sprintf(log,"Calculated scale: (ds + ret) %f %f = %f\n",
	 scale_factor_downsample, _retina_scale, scale_factor); l_info(log);
	 }*/

	// glLineWidth(app_settings.scale_retina);
}

void drw_set_circle_precision_override_limit(bool v)
{
	override_circle_limit = v;
}

void drw_set_circle_precision(int v)
{
	if (v > PRECISION_WARNING_LIMIT)
	{
		if (v < override_circle_limit)
		{
			circle_precision = v;
		}
		else
		{
			static bool
			    emit_warning_about_high_precision_circles_once =
				false;
			if (!emit_warning_about_high_precision_circles_once)
			{
				char buf[256];
				sprintf(buf,
					"are you sure you want a circle this "
					"precise? (%d)\nsetting circ to max %d",
					v, override_circle_limit);
				// printf("are you sure you want a circle this
				// precise?\n");
				printf(buf);
				emit_warning_about_high_precision_circles_once =
				    true;
			}
			circle_precision = override_circle_limit;

			return;
		}
	}
	circle_precision = v;
}

void drw_pop_fill()
{
	fill = prev_fill;
}

void drw_set_fill(bool v)
{
	prev_fill = fill;
	fill      = v;
}

static void faces2(double r)
{
	drw_push();
	drw_translate_z(r * -.5);
	drw_square(r);
	drw_translate_z(r);
	drw_square(r);
	drw_pop();
}

static void cube(double r)
{
	//	the WORST poss impl but avoid shitty glut ness and I'm in a rush
	drw_push();
	faces2(r);
	drw_rotate_x(90);
	faces2(r);
	drw_rotate_y(90);
	faces2(r);

	drw_pop();
}
void drw_cube(float r)
{
	cube(r);
/*
	 // Draw the triangle !
	 glDrawArrays(GL_TRIANGLES, 0, 12*3);
	 // 1rst attribute buffer : vertices
	 glEnableVertexAttribArray(0);
	 glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	 glVertexAttribPointer(
	 0,                  // attribute 0. No particular reason for 0, but
	 must match the layout in the shader. 3,                  // size
   GL_FLOAT,
	 // type GL_FALSE,           // normalized? 0,                  //
   stride (void*)0
	 // array buffer offset
	 );
	 // Draw the triangle !
	 glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices
	 total
	 -> 1 triangle glDisableVertexAttribArray(0);

	 */
#ifndef DRW_PLATFORM_IOS
//glutWireCube(v);
#endif
}

extern char** app_argv;
extern int    app_argc;
static bool   glut_initted = false;

void _check_glut_init(void)
{
	if (glut_initted)
		return;
	drw_glut_init();
	glut_initted = true;
}

void drw_glut_init(void)
{
#ifdef ENABLE_OLD_HACKY_GLUT

// glutInit(app_argc, app_argv);
#endif
}

void drw_tetrahedron(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT
	drw_push();
	drw_scale_u(v);
	(fill) ? glutSolidTetrahedron() : glutWireTetrahedron();
	drw_pop();
#endif
}

void drw_sphere(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT
	_check_glut_init();
	glutWireSphere(v * 2, 12, 12);
#endif
}

void drw_octahedron(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT

	drw_push();
	drw_scale_u(v);
	(fill) ? glutSolidOctahedron() : glutWireOctahedron();
	drw_pop();
#endif
}

void drw_dodecahedron(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT

	drw_push();
	drw_scale_u(v);
	(fill) ? glutSolidDodecahedron() : glutWireDodecahedron();
	drw_pop();
#endif
}

void drw_icosahedron(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT

	drw_push();
	drw_scale_u(v * 2);
	(fill) ? glutSolidIcosahedron() : glutWireIcosahedron();
	drw_pop();
#endif
}

void drw_teapot(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT

	drw_push();
	glutWireTeapot(v);
	drw_pop();
#endif
}

void drw_swap(void)
{
}
void drw_finish(void)
{
	glFinish();
}
