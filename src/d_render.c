//
//  render.c
//  r4
//
//  Created by Andrew Macfarlane on 12/9/16.
//  Copyright © 2016 vaporstack. All rights reserved.
//

#include "d_render.h"

#include <r4/src/r4_config.h>
#include <r4/src/r4_platform.h>



#ifndef RPLATFORM_IOS
#define ENABLE_OLD_HACKY_GLUT
#endif

#ifdef ENABLE_OLD_HACKY_GLUT
#include <GLUT/glut.h>
#endif


#ifdef RPLATFORM_IOS

//#include "../core/r_app_ios.h"

//#include <OpenGLES/ES1/gl.h>
//#include <OpenGLES/ES3/gl.h>



#include "d_font_ftgles.h"
#include "src/gluLookAt.h"
#else
#ifdef RPLATFORM_DARWIN
//#include <OpenGL/gl.h>
#include "d_font_ftgl.h"
//#undef RPLATFORM_IOS
#else

#ifdef RPLATFORM_WIN
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
#include "d_font_ftgl.h"

//#undef RPLATFORM_IOS
#endif
#endif

#ifdef RPLATFORM_IOS

#define VERTEX_POINTER_IDENT GL_FLOAT
#else
//#ifndef RPLATFORM_WIN
//#error fuck
#define VERTEX_POINTER_IDENT GL_DOUBLE
//#endif
#endif

#define TEXT_MAX 1024

#include <math.h>
#include <stdbool.h>

//  these things used to be part of the app, have to talk now from across
// the sea
static bool _ortho;
static bool _screenspace;

static double fov = 45;
static double window_width;
static double window_height;

static double framebuffer_width;
static double framebuffer_height;

static bool color_bypass = false;
#include <r4/r4.h>

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

int d_get_gl_error()
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

void d_init()
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

	d_get_gl_error();
	glEnable(GL_BLEND);
#ifndef RPLATFORM_WIN
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

	//  MISH MASH OF CRAP FROM BEFORE THAT I DON'T FULLY GET
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);

	glEnableClientState(GL_VERTEX_ARRAY);
#ifndef RPLATFORM_WIN
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

#ifndef RPLATFORM_IOS
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
#endif

	d_calculate_scale();

	d_setup_view();

	// const char* path = r_resource_load("ruminant","ttf");
	// if ( debug_settings.render )
	//  printf("loading font, app scale is %f\n",
	//  app_settings.scale_retina);

	// 0if ( path != NULL )
	//{
	// const char* path = r_resource_load("vs-junk","otf");
	//  r_font_load(path);
	//    d_font_size(72 * app_settings.scale_retina);
	// }

	d_get_gl_error();
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	// glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	// glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	// glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
	// g_vertex_buffer_data, GL_STATIC_DRAW);  glBindBuffer(0,
	// vertexbuffer);
	d_clear();
}

void d_deinit(void)
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

void d_set_line_width(float v)
{
	glLineWidth(v);

	d_get_gl_error();
}

void d_text_load(const char* path)
{

// 0if ( path != NULL )
//{
// const char* path = r_resource_load("vs-junk","otf");
#ifdef R4_ENABLE_FTGL
	d_font_load(path);

#endif

	// d_font_size(72);
	// }
}

void d_set_blend(int v)
{

#ifndef RPLATFORM_WIN

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

void d_clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

RColor8 d_check_color(void)
{
	RColor8 c;
	c.r = _r;
	c.g = _g;
	c.b = _b;
	c.a = _a;
	return c;
}

void d_color_clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void d_color_clear_color(RColor c)
{
	d_color_clear(c.r, c.g, c.b, c.a);
}

void d_color_clear_color8(RColor8 c)
{
	d_color_clear(c.r, c.g, c.b, c.a);

	//glClearColor(c.r, c.g, c.b, c.a);
}

void d_color_clear_color16(RColor16 c)
{
	d_color_clear(c.r, c.g, c.b, c.a);
	//glClearColor(c.r, c.g, c.b, c.a);
}

void _set_internal_colors(float r, float g, float b, float a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void d_set_colorbypass(bool v)
{
	color_bypass = v;
}

void d_color(double r, double g, double b, double a)
{
	_set_internal_colors(r, g, b, a);
#ifndef RPLATFORM_IOS
	glColor4d(r, g, b, a);
#else
	glColor4f(r, g, b, a);
#endif
}

void d_color4f(float r, float g, float b, float a)
{
	_set_internal_colors(r, g, b, a);
	glColor4f(r, g, b, a);
}

void d_color_c(RColor c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}

void d_color_c8(RColor8 c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}

void d_color_c16(RColor16 c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}

void d_color_wc(WColor c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}

/*
void d_color_wc8(WColor8 c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}
*/

void d_color_wc16(WColor16 c)
{
	_set_internal_colors(c.r, c.g, c.b, c.a);
	glColor4f(c.r, c.g, c.b, c.a);
}
void d_color3f(float r, float g, float b)
{
	_set_internal_colors(r, g, b, _a);
	glColor4f(r, g, b, _a);
}

void d_color_u(float v)
{
	_set_internal_colors(v, v, v, 1.);
	glColor4f(v, v, v, 1.);
}

void d_color_push()
{
	prev.r = _r;
	prev.g = _g;
	prev.b = _b;
	prev.a = _a;
}

void d_color_pop()
{
	_set_internal_colors(prev.r, prev.g, prev.b, prev.a);

	glColor4f(prev.r, prev.g, prev.b, prev.a);
}

/*
 void d_color_c(RColor* c)
 {
 _set_internal_colors(c->r, c->g, c->b, 1.0f);
 glColor4f(c->r,c->g,c->b,1.);
 }

 void d_color_c8(RColor8* c)
 {
 _set_internal_colors(c->r, c->g, c->b, c->a);
 glColor4f(c->r,c->g,c->b,c->a);
 }

 void d_color_c16(RColor16* c)
 {
 _set_internal_colors(c->r, c->g, c->b, c->a);
 glColor4f(c->r,c->g,c->b,c->a);
 }
 */

void d_alpha(float a)
{
	prev_alpha = a;
	glColor4f(_r, _g, _b, a);
}

void d_alpha_pop()
{
	_a = prev_alpha;
	glColor4f(_r, _g, _b, _a);
}

#ifdef RPLATFORM_IOS
#define MATRIX_STACK_MAX 16
#else
#define MATRIX_STACK_MAX 32
#endif

static int matrix_stack_count = 0;

void d_alpha_mult(double v)
{
	alpha_mult = v;
}

int d_check_matrix(void)
{
	return matrix_stack_count;
}

void d_alpha_mult_pop()
{
	alpha_mult = 1;
}

void d_push()
{
	matrix_stack_count++;
	if (matrix_stack_count > MATRIX_STACK_MAX)
	{
		printf("Matrix overflowed! %d\n", matrix_stack_count);
	}
	glPushMatrix();
}

void d_pop()
{
	matrix_stack_count--;
	if (matrix_stack_count < 0)
	{
		printf("Matrix underflowed!\n");
	}
	glPopMatrix();
}

void d_rotate_x(float r)
{
	glRotatef(r, 1, 0, 0);
}

void d_rotate_y(float r)
{
	glRotatef(r, 0, 1, 0);
}

void d_rotate_z(float r)
{
	glRotatef(r, 0, 0, 1);
}

void d_rotate(float x, float y, float z)
{
	glRotatef(x, 1, 0, 0);
	glRotatef(y, 0, 1, 0);
	glRotatef(z, 0, 0, 1);
}

#pragma mark primitives

void d_line_r(RLine* poly)
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

void d_line_3f(float ax, float ay, float az, float bx, float by, float bz)
{
	const GLfloat renderLine[] = {ax, ay, az, bx, by, bz};
	glVertexPointer(3, GL_FLOAT, 0, renderLine);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void d_line3_r(RLine3* poly)
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

void d_line(float ax, float ay, float bx, float by)
{
	const GLfloat renderLine[] = {ax, ay, bx, by};
	glVertexPointer(2, GL_FLOAT, 0, renderLine);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void d_line_p(WPoint a, WPoint b)
{
	const GLfloat renderLine[] = {a.x, a.y, b.x, b.y};
	glVertexPointer(2, GL_FLOAT, 0, renderLine);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void d_tri(float ax, float ay, float bx, float by, float cx, float cy)
{
	const GLfloat renderLine[] = {ax, ay, bx, by, cx, cy};
	glVertexPointer(2, GL_FLOAT, 0, renderLine);
	glDrawArrays(GL_LINE_STRIP, 0, 3);
}

void d_scale_2f(float x, float y)
{
	glScalef(x, y, 1);
}

void d_scale(float x, float y, float z)
{
	glScalef(x, y, z);
}
void d_scale_u(float v)
{
	glScalef(v, v, v);
}
void d_scale_x(float x)
{
	glScalef(x, 1, 1);
}
void d_scale_y(float y)
{
	glScalef(1, y, 1);
}
void d_scale_z(float z)
{
	glScalef(1, 1, z);
}
/*
 void d_translate_vec2(Vec2* v)
 {
 glTranslatef(v->x, v->y, 0);
 }

 */
void d_translate_vec3(vec3_t v)
{
	glTranslatef(v[0], v[1], v[2]);
}

void d_translate_x(float v)
{
	glTranslatef(v, 0, 0);
}

void d_translate_y(float v)
{
	glTranslatef(0, v, 0);
}

void d_translate_z(float v)
{
	glTranslatef(0, 0, v);
}

void d_translate2f(float x, float y)
{
	glTranslatef(x, y, 0);
}

void d_translate(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void d_translate_rp(RPoint p)
{
	glTranslatef(p.x, p.y, 0);
}

void d_translate_rp3(RPoint3 p)
{
	glTranslatef(p.x, p.y, p.z);
}

void d_translate3f(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void d_gtransform_apply(GTransform t)
{
	d_translate_vec3(t.position);
	d_translate_vec3(t.anchor);
	d_scale_vec3(t.scale);
	d_rotate_vec3(t.rotation);
}

void d_transform_apply(WTransform t)
{
	d_translate_wvec(t.position);
	d_translate_wvec(t.anchor);
	d_scale_wvec(t.scale);
	d_rotate_wvec(t.rotation);
}

void d_transform_deapply(WTransform t)
{
	/*d_rotate_wvec(t.rotation);

	d_translate_wvec(t.position);
	d_translate_wvec(t.anchor);
	d_scale_wvec(t.scale);
*/
	
}

void d_gtransform_deapply(GTransform t)
{
	
}

#ifdef RPLATFORM_IOS
#define glTranslated glTranslatef
#define glScaled glScalef
#define glRotated glRotatef
#endif

void d_translate_wvec(WVec3d v)
{
	glTranslated(v.x, v.y, v.z);
}

void d_scale_wvec(WVec3d v)
{
	// printf("%f %f %f\n", v.x, v.y, v.z);
	glScaled(v.x, v.y, v.z);
}

void d_scale_vec3(vec3_t v)
{
	// printf("%f %f %f\n", v.x, v.y, v.z);
	glScaled(v[0], v[1], v[2]);
}
/*
void d_rotate_vec3(vec3_t v)
{
	glRotated(v[0], 1, 0, 0);
	glRotated(v[1], 0, 1, 0);
	glRotated(v[2], 0, 0, 1);
}
*/
void d_rotate_wvec(WVec3d v)
{
	glRotated(v.x, 1, 0, 0);
	glRotated(v.y, 0, 1, 0);
	glRotated(v.z, 0, 0, 1);
}

void d_rotate_vec3(vec3_t vec)
{
	d_rotate(vec[0], vec[1], vec[2]);
}

void d_nice_axis()
{
	float x = axis_radius * scale_factor;

	d_push();
	d_line(x, 0, x * 2, 0);
	d_rotate_z(120.0);
	d_line(x, 0, x * 2, 0);
	d_rotate_z(120.0);
	d_line(x, 0, x * 2, 0);
	d_rotate_z(120.0);
	d_pop();
}

void d_text_size(int sz, int resolution)
{
#ifdef R4_ENABLE_FTGL
	d_font_size(sz, resolution);

#endif
}
void d_text(const char* format, ...)
{
	char buf[TEXT_MAX];
	sprintf(buf, "%s", format);
	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

#ifdef R4_ENABLE_FTGL

	d_font_draw(buf);
#endif
}

void d_wsequence_e(WSequence* seq, int frame)
{

	// TODO
	// implement this.
	//
	// WObject* fr = seq->frames[frame];

	//  do stuff here.
}

void d_robject(RObject* obj)
{

	size_t num = obj->lines.length;

	RLine* l = r_line_create();
	int    i;
	for (i = 0; i < num; ++i)
	{
		vector_get(&obj->lines, i, l);
		// RLine* l = obj->lines[i];
		if (!l)
		{
			printf("ack!\n");
			return;
		}

		d_rline(l);
	}
}

void d_wobject_normal(WObject* obj)
{
	// d_color(1,1,1,.9);
	d_push();
	d_translate(obj->transform.position.x, obj->transform.position.y,
		    obj->transform.position.z);

	d_scale_2f(obj->transform.scale.x, obj->transform.scale.y);
	int i;

	for (i = 0; i < obj->num_lines; ++i)
	{
		WLine* l = obj->lines[i];
		if (!l)
		{
			printf("ack!\n");
			return;
		}

		d_poly(l);
	}

	d_pop();
}

void d_wobject_naive(WObject* obj)
{

	d_rect_w(obj->bounds);

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

	for (i = 0; i < obj->num_lines; ++i)
	{
		WLine* l = obj->lines[i];
		if (!l)
		{
			printf("ack!\n");
			return;
		}

		d_poly(l);
	}
}

void d_wobject_e(WObject* obj)
{

	if (obj->normalized)
	{
		d_wobject_normal(obj);
	}
	else
	{
		d_wobject_naive(obj);
	}
}

void d_wline_fill(WLine* l)
{
	d_set_fill(true);
	d_poly(l);
	d_pop_fill();
}

/*
 static void _wlinepoly(WLine* line)
{
}
*/

void d_wline_strokeonly(WLine* l)
{
	if (l == NULL)
	{
		printf("Tried to render a null line!\n");
		return;
	}

	d_poly(l);
}

void d_wline(WLine* l)
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
			d_color(c.r, c.g, c.b, c.a * alpha_mult);
			/// dirty hack to have color and also transparent onion
			/// skins oops
			//r_alpha(c.a * alpha_mult);
		}
		if (l->closed)
		{
			d_set_fill(l->closed);
			d_poly(l);
			d_pop_fill();
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
			d_gpc_tristrip(l->tess);
			return;
		}

		if (l->brush->stroke)
		{
			if (l->brush->stroke->tess)
			{
				d_gpc_tristrip(l->brush->stroke->tess);
			}
			else
			{

				d_triangle_strip(l->brush->stroke);
			}
		}
		// else{

		//  d_poly(l->brush->stroke);
		//}
	}
	else
	{

		if (l->tess)
		{
			d_gpc_tristrip(l->tess);
		}
		else
		{
			d_poly(l);
		}
	}
#else
	d_poly(l);
#endif
	//  TODO color pop?
}

void d_wobject(WObject* obj)
{

	// d_rect_w(obj->bounds);
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

	d_push();
	d_transform_apply(obj->transform);
	int i;
	for (i = 0; i < obj->num_lines; ++i)
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
		d_wline(l);
	}
	d_pop();
}

void d_wobject_notransform(WObject* obj)
{

	// d_rect_w(obj->bounds);

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

	d_push();
	//d_transform_apply(obj->transform);
	int i;
	for (i = 0; i < obj->num_lines; ++i)
	{

		WLine* l = obj->lines[i];
		if (!l)
			continue;

		if (!l)
		{
			printf("ack!\n");
			return;
		}

		d_wline(l);
	}
	d_pop();
}

void d_wobject_strokeonly(WObject* obj)
{

	// d_rect_w(obj->bounds);

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

	d_push();
	d_transform_apply(obj->transform);
	int i;
	for (i = 0; i < obj->num_lines; ++i)
	{

		WLine* l = obj->lines[i];
		if (!l)
			continue;

		if (!l)
		{
			printf("ack!\n");
			return;
		}

		d_wline_strokeonly(l);
	}
	d_pop();
}

void d_wobject_strokeonly_notransform(WObject* obj)
{

	// d_rect_w(obj->bounds);

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

	d_push();
	//d_transform_apply(obj->transform);
	int i;
	for (i = 0; i < obj->num_lines; ++i)
	{

		WLine* l = obj->lines[i];
		if (!l)
			continue;

		if (!l)
		{
			printf("ack!\n");
			return;
		}

		d_wline_strokeonly(l);
	}
	d_pop();
}

void d_verts_r(RLine* l)
{
	int i;
	for (i = 0; i < l->num; ++i)
	{
		RPoint* p = &l->data[i];
		d_push();
		d_translate2f(p->x, p->y);
		double pv = 10;
		d_square(pv);
		d_pop();
	}
}

void d_verts(WLine* l)
{
	int i;
	for (i = 0; i < l->num; ++i)
	{
		WPoint* p = &l->data[i];
		d_push();
		d_translate2f(p->x, p->y);
		double pv = p->pressure;
		pv *= 10;
		d_square(pv);
		d_pop();
	}
}

void d_wobject_verts(WObject* obj)
{
	int i;
	for (i = 0; i < obj->num_lines; i++)
	{
		WLine* line = obj->lines[i];
		if (!line)
		{
			printf("Error, bogus line!\n");
			continue;
		}
		d_verts(line);
	}
}

void d_axis()
{
	/// ones here used to be scale factor
	int axis_render_radius = axis_radius * scale_factor;
	// d_push();
	// d_translate2f(x * 1,y * 1);
	d_line(-axis_render_radius, 0, axis_render_radius, 0);
	d_line(0, -axis_render_radius, 0, axis_render_radius);
	// d_pop();
}

void d_axis_living()
{
	static float r			= 0.f;
	static float speed		= .01f;
	float	  axis_render_radius = axis_radius * 1;

	//axis_render_radius = axis_radius * 1;
	r += speed;
	// d_push();
	// d_translate2f(x * 1,y * 1);
	d_rotate_z(r * 90);
	d_line(-axis_render_radius, 0, axis_render_radius, 0);
	d_line(0, -axis_render_radius, 0, axis_render_radius);
	d_rotate_z(-r * 90);
	// d_pop();
}
/*

 void d_rect_r(RRect r )
 {
 d_rect(r.pos.x, r.pos.y, r.pos.x + r.size.x, r.pos.y + r.size.y);

 }
 */

void d_rect_w(WRect r)
{
	d_rect(r.pos.x, r.pos.y, r.pos.x + r.size.x, r.pos.y + r.size.y);
}

void d_rect_r(RRect r)
{
	d_rect(r.pos.x, r.pos.y, r.pos.x + r.size.x, r.pos.y + r.size.y);
}

void d_rect_wp(WPoint a, WPoint b)
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

void d_rect_rp(RPoint a, RPoint b)
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

void d_rect(float ax, float ay, float bx, float by)
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
}

void d_point(void)
{
	float arr[2];
	glVertexPointer(2, GL_FLOAT, 0, &arr);
	glDrawArrays(GL_POINTS, 0, 4);
	//  printf("~");
}

void d_square(float r)
{
	d_rect(r * -.5, r * -.5, r * .5, r * .5);
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

void d_circle(float r)
{
	d_ellipse(r, r);
	/*
	float* circle = circle_defs[circle_precision];
	if (circle == NULL) {
		printf("calculating circle for %i\n", circle_precision);
		calculate_circle(circle_precision);
	}
	circle = circle_defs[circle_precision];
	glVertexPointer(2, GL_FLOAT, 0, circle);

	// cout << "c:" << circleArray[2] << endl;
	d_scale_u(r);
	fill ? glDrawArrays(GL_TRIANGLE_FAN, 0, circle_precision)
	     : glDrawArrays(GL_LINE_LOOP, 0, circle_precision);

	//r_ellipse(r, r);
*/
}

void d_ellipse(float _x, float _y)
{
	/*if ( _x == _y )
	{
		d_circle(_x);
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

	// cout << '+';
	glVertexPointer(2, GL_FLOAT, 0, arr);

	// cout << "c:" << circleArray[2] << endl;
	fill ? glDrawArrays(GL_TRIANGLE_FAN, 0, circle_precision)
	     : glDrawArrays(GL_LINE_LOOP, 0, circle_precision);

	free(arr);

	// fill ?
	// glDrawArrays(GL_TRIANGLE_FAN, 0, circle_precision ):
	// glDrawArrays(GL_LINE_LOOP, 0, circle_precision );
}

/*
 void r_rrect(RRect rec)
 {

 d_rect(rec.pos.x, rec.pos.y, rec.pos.x + rec.size.x, rec.pos.y + rec.size.y);

 }
 */

/*
 void r_vecline2(Vec2Line * vec)
 {
 const unsigned long long renderLineSize = ( vec->num * 2);

 GLfloat arr[ renderLineSize ];

 for (int i=0, j=0; i < vec->num ; i++, j+=2 ) {
 Vec2 *p = &vec->data[i];
 arr[j] = p->x;
 arr[j+1] = p->y;
 //arr[j+2] = p->z;
 }

 glVertexPointer(2, GL_FLOAT, 0, &arr);

 fill ?
 glDrawArrays(GL_TRIANGLE_FAN, 0, vec->num ):
 glDrawArrays(GL_LINE_STRIP, 0, vec->num );



 }

 void r_vecline3(Vec3Line * vec)
 {

 unsigned long long renderLineSize = ( vec->num * 3);

 GLfloat arr[ renderLineSize ];
 if ( vec->num <= 2 )
 return;

 for (unsigned long long i=0, j=0; i < vec->num ; i++, j+=3 ) {
 vec3_t *p = &vec->data[i];
 arr[j] = p->x;
 arr[j+1] = p->y;
 arr[j+2] = p->z;
 }

 glVertexPointer(3, GL_FLOAT, 0, &arr);

 fill ?
 glDrawArrays(GL_TRIANGLE_FAN, 0, (int)vec->num ):
 glDrawArrays(GL_LINE_STRIP, 0, (int)vec->num );



 }
 */
void d_rline(RLine* poly)
{

	const unsigned long long renderLineSize = (poly->num * 2);

	// printf("poly is %d num\n", poly->num);
	// GLfloat arr[ renderLineSize ];
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

void d_poly(WLine* line)
{
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

void d_poly_extras(WLine* line)
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
		d_set_fill(line->closed);
		d_poly(line);
		d_pop_fill();
	}
#ifdef DISABLE_UNTIL_WORKLINE_REFACTOR_COMPLETE

	if (line->brush)
	{
		// w_brush_draw(line->brush);
	}
	else
	{
		// if (!line->closed)
		d_set_fill(line->closed);
		d_poly(line);
		d_pop_fill();
	}

	if (line->tess)
	{
		d_gpc_tristrip(line->tess);
	}
#endif
}

void d_gpc_polygon_outline(GPCRec* rec)
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

void d_gpc_polygon(GPCRec* rec)
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

void d_gpc_verts(void* dat)
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

	 d_translate2f(x,y);
	 d_square(20);
	 d_translate2f(-x,-y);

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

void d_gpc_triwire(void* dat)
{
#ifndef RPLATFORM_IOS
	glPolygonMode(GL_FRONT, GL_LINE);
	d_gpc_tristrip(dat);
	glPolygonMode(GL_FRONT, GL_FILL);
#endif
}

void d_gpc_tristrip(void* dat)
{
	//#ifndef RPLATFORM_IOS
	gpc_tristrip* tri = dat;
	int	   j, s, v;

	for (s = 0; s < tri->num_strips; s++)
	{

		gpc_vertex_list str = tri->strip[s];
		GLfloat*	arr = malloc(sizeof(GLfloat) * str.num_vertices * 2);

		// glBegin(GL_TRIANGLE_STRIP);
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

void d_triangle_strip(WLine* poly)
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

void d_tristrip_3d(double* arr, int num, bool closed)
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
	 */
	glVertexPointer(3, VERTEX_POINTER_IDENT, 0, arr);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, num);
	free(arr);
}

void d_tris_3d(double* arr, int num, bool closed)
{

	glVertexPointer(3, VERTEX_POINTER_IDENT, 0, arr);

	glDrawArrays(GL_TRIANGLES, 0, num);
	free(arr);
}

void d_rawpoly_2d(double* arr, int num, bool closed)
{

	glVertexPointer(2, VERTEX_POINTER_IDENT, 0, arr);

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

void d_rawpoly_2f(float* arr, int num, bool closed)
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

void d_rawpoly_3d(double* arr, int num, bool closed)
{

	glVertexPointer(3, VERTEX_POINTER_IDENT, 0, arr);

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

void d_rawpoly_3f(float* arr, int num, bool closed)
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

void d_rawpoints_3d(double* arr, int num)
{

	glVertexPointer(3, VERTEX_POINTER_IDENT, 0, arr);
	glDrawArrays(GL_POINTS, 0, num);
}

void d_point3f(float x, float y, float z)
{
	float arr[3] = {x, y, z};
	glVertexPointer(3, GL_FLOAT, 0, arr);
	glDrawArrays(GL_POINTS, 0, 3);
}

void d_rawpoints_3f(float* arr, int num)
{

	glVertexPointer(3, GL_FLOAT, 0, arr);
	glDrawArrays(GL_POINTS, 0, num);
}
void d_rgbtri(double gamma)
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

	d_set_circle_precision(3);
	d_set_fill(true);
	d_circle(1);
	d_pop_fill();
	glDisableClientState(GL_COLOR_ARRAY);
}

void d_grid(float size, int subdiv)
{

	// d_push();
	// d_rotate_x(90);
	// d_translate2f(size * -.5, size *-.5);
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
		d_line(x1, y1, x2, y2);
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
		d_line(x1, y1, x2, y2);
	}

	// d_pop();
}

void r_set_window(double w, double h)
{
	window_width  = w;
	window_height = h;

	d_calculate_scale();
	d_setup_view();
}

void d_set_ortho(bool val)
{
#ifdef DEBUG
	printf("Setting ortho to %d\n", val);
#endif
	_ortho = val;
}

void d_set_screenspace(bool val)
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

void d_setup_view(void)
{
	if (_ortho)
	{
		d_setup_view_ortho();
	}
	else
	{

		d_setup_view_persp();
	}

	int bits;
	glGetIntegerv(GL_RED_BITS, &bits); //(GL_RED_BITS);
	l_info("Bits is %d\n", bits);
}

#include <glulookat/gluLookAt.h>

void d_setup_view_persp()
{
	// if(debug_settings.render)
	l_info("Setting up perspective projection.\n");
	// static int zoomFactor = 1;
	float left, right, top, bottom, znear, zfar;
	float x, y;

	x = window_width * _retina_scale;
	y = window_height * _retina_scale;

	/// x = app_settings.current_window_x;
	// y = app_settings.current_window_y;

	// x = size.x;
	// y = size.y;

	// glfwGetFramebufferSize(window,&x, &y);

	// x *= _retina_scale;
	// y *= _retina_scale;
	// x *= app_settings.scale_retina;
	// y *= app_settings.scale_retina;

	glViewport(0, 0, x, y);
	glViewport(-.5 * x, -.5 * y, .5 * x, .5 * y);

	// Select and setup the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Select and setup the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float pixelAspect = 1.f;

	bool landscape = true;

	// landscape
	if (x > y)
	{
		pixelAspect = (float)x / (float)y;
		landscape   = true;
	}

	// portrait
	if (y > x)
	{
		pixelAspect = (float)y / (float)x;
		landscape   = false;
	}

	left = top = znear = -1.f;
	right = bottom = zfar = 1.f;

	znear = -1024.f;
	znear = 0.00001f;
	zfar  = 1024.f;

	if (landscape)
	{
		left *= pixelAspect;
		right *= pixelAspect;
	}
	else
	{
		top *= pixelAspect;
		bottom *= pixelAspect;
	}
	_aspect = pixelAspect;
	left    = 0;
	top     = 0;
	right   = x;
	bottom  = y;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

// glOrtho(left*zoomFactor, right*zoomFactor, top*zoomFactor, bottom*zoomFactor,
// near, far);  glFrustum(left,right,bottom,top,near, far);
//#ifdef RPLATFORM_WIN

//	printf("glu stuff doesn't link on windows >:[\n");
//#else
#ifndef RPLATFORM_IOS
	gluPerspective(fov, right / bottom, znear, zfar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, y, 0, 0, 0.0, 0.0, znear, zfar);
#else
	gluPerspective(fov, right / bottom, znear, zfar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, y, 0, 0, 0.0, 0.0, znear, zfar);

// printf("Error, gluLookAt doesn't exist on ios\n");
//#endif
#endif

	// gluLookAt( right / 2 , bottom / 2, 400, right / 2, bottom / 2,
	// 0.,0.0, 1.0, 0.0);

	// cout << "FRUST" << endl;
	// glFrustum(left,right,bottom,top,near, far);
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
	// glTranslatef(0.375f,0.375f,0.f);
	
	
}

void d_setup_view_ortho()
{
	// if(debug_settings.render )

	// printf("Ortho view!\n");

	// float ratio;
	// float pixelAspect = 1.;

	float width, height;

	width    = framebuffer_width;
	height   = framebuffer_height;
	double x = width;
	double y = height;
	// dumb lazy in a hurry
	// printf("%f %f\n", width, height );

	float pixelAspect = 1.f;

	// bool landscape = true;

	// landscape
	if (x > y)
	{
		pixelAspect = (float)x / (float)y;
		// landscape = true;
	}

	// portrait
	if (y > x)
	{
		pixelAspect = (float)y / (float)x;
	}

	glViewport(0, 0, (int)width, (int)height);

	// if ( !app_settings.screenspace )
	if (!_screenspace)
	{
		// if(debug_settings.render )
		printf("Not using screenspace(normals)\n");
		width  = 1.;
		height = 1.;
	}

	_aspect = pixelAspect;
	/*
	 if (true) {
	 width = app_settings.current_window_x;
	 height = app_settings.current_window_y;
	 }else{
	 height = app_settings.current_window_y;;
	 width = app_settings.current_window_x;
	 }

	 */

	/*

	 //portrait
	 if ( height > width ) {
	 ratio = (float)height / (float)width;
	 }else{
	 //landscape
	 ratio = (float)width / (float)height;
	 //landscape = true;
	 }
	 */

	d_get_gl_error();

	glLoadIdentity();

	d_get_gl_error();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// if ( _screenspace )
	//{
	//#ifdef RPLATFORM_IOS
	//    glOrthof(-width*.5, width*.5, height*-.5,height*.5, -10000,
	// 10000); #else
	//    glOrtho(-width*.5, width*.5, height*-.5,height*.5, -10000,
	// 10000); #endif }else{

	//  printf("IDK what happens. here.\n");
	//}

	/*
	 if ( !app_settings.screenspace ){

	 #ifdef RPLATFORM_IOS
	 glOrthof(-width*.5, width*.5, height*-.5,height*.5, -10000, 10000);
	 #else
	 glOrtho(-width*.5, width*.5, height*-.5,height*.5, -10000, 10000);
	 #endif

	 }else{
	 */
	_aspect = pixelAspect;

	double l = width * -.5;
	double r = width * .5;
	double t = height * -.5;
	double b = height * .5;
	double n = 1048*-8;
	double f = 1024*8;

#ifdef RPLATFORM_IOS
	glOrthof(l, r, t, b, n, f);
#else

	glOrtho(l, r, t, b, n, f);
#endif

	if (_screenspace)
	{
		glTranslatef(0.375f, 0.375f, 0.f);
	}

	//}

	d_get_gl_error();
#ifndef RPLATFORM_IOS
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#endif
}

void d_set_screensize(double w, double h)
{
	// printf("Setting renderer to %f %f\n", w, h);
	window_width  = w;
	window_height = h;

	// d_calculate_scale();
	// d_setup_view();
}
void d_set_framebuffer(double w, double h)
{
	// printf("Setting framebuffer to %f %f\n", w, h);
	framebuffer_width  = w;
	framebuffer_height = h;

	// d_calculate_scale();
	// d_setup_view();
}

void d_set_fov(double v)
{
	fov = v;
}

void d_set_downsample(double v)
{
	scale_factor_downsample = v;
	d_calculate_scale();
}

void d_set_retina(double v)
{
	// if(debug_settings.render )
	// printf("Setting retina scale: %f\n", v);
	// scale_factor_retina = v;
	d_calculate_scale();
	_retina_scale = v;
	glLineWidth(1);
}

void d_calculate_scale()
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

void d_set_circle_precision_override_limit(bool v)
{
	override_circle_limit = v;
}

void d_set_circle_precision(int v)
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
				l_warning(buf);
				emit_warning_about_high_precision_circles_once =
				    true;
			}
			circle_precision = override_circle_limit;

			return;
		}
	}
	circle_precision = v;
}

void d_pop_fill()
{
	fill = prev_fill;
}

void d_set_fill(bool v)
{
	prev_fill = fill;
	fill      = v;
}

static void faces2(double r)
{
	d_push();
	d_translate_z(r * -.5);
	d_square(r);
	d_translate_z(r);
	d_square(r);
	d_pop();
}

static void cube(double r)
{
	//	the WORST poss impl but avoid shitty glut ness and I'm in a rush
	d_push();
	faces2(r);
	d_rotate_x(90);
	faces2(r);
	d_rotate_y(90);
	faces2(r);

	d_pop();
}
void d_cube(float r)
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
#ifndef RPLATFORM_IOS
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
	d_glut_init();
	glut_initted = true;
}

void d_glut_init(void)
{
#ifdef ENABLE_OLD_HACKY_GLUT

	// glutInit(app_argc, app_argv);
#endif
}

void d_tetrahedron(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT
	d_push();
	d_scale_u(v);
	(fill) ? glutSolidTetrahedron() : glutWireTetrahedron();
	d_pop();
#endif
}

void d_sphere(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT
	_check_glut_init();
	glutWireSphere(v * 2 ,12,12);
#endif
}


void d_octahedron(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT

	d_push();
	d_scale_u(v);
	(fill) ? glutSolidOctahedron() : glutWireOctahedron();
	d_pop();
#endif
}

void d_dodecahedron(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT

	d_push();
	d_scale_u(v);
	(fill) ? glutSolidDodecahedron() : glutWireDodecahedron();
	d_pop();
#endif
}

void d_icosahedron(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT

	d_push();
	d_scale_u(v * 2);
	(fill) ? glutSolidIcosahedron() : glutWireIcosahedron();
	d_pop();
#endif
}

void d_teapot(float v)
{
#ifdef ENABLE_OLD_HACKY_GLUT

	d_push();
	glutWireTeapot(v);
	d_pop();
#endif
}
