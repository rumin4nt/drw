//
//  render.h
//  r4
//
//  Created by Andrew Macfarlane on 12/9/16.
//  Copyright © 2016 vaporstack. All rights reserved.
//

#ifndef d_rended_h
#define d_rended_h

#include <stdbool.h>
#include <stdlib.h>

//	todo: get this out?  a lot of deps for a convenience
// #include <wsh/wsh.h>

#pragma GCC visibility push(default)

#ifdef DRW_BUILD_STANDALONE
#include "drw_dummy.h"
#else
#include <grdn/grdn.h>
#include <r4/src/core/r_color.h>

//#include <r4/r4.h>
#endif

//#include <wsh/wsh.h>

//#include <r4/src/core/d_math.h>
//#include <grdn/grdn.h>
//#include <r4/r4.h>

//#include <gl-matrix/gl-matrix.h>

#ifdef DRW_ENABLE_SNOOP

#include "hacks/drw_snoop.h"
#endif

enum
{
	DRW_BLEND_MODE_NONE,
	DRW_BLEND_MODE_NORMAL,
	DRW_BLEND_MODE_ADD,
	DRW_BLEND_MODE_MULTIPLY,
	DRW_BLEND_MODE_SCREEN,
	DRW_BLEND_MODE_SUBTRACT
};

void drw_init(void);
void drw_deinit(void);

//	geometry
double drw_query_aspectratio(void);
bool   drw_query_landscape(void);
double drw_query_retina(void);
void   drw_query_framebuffer(int*, int*);

//	color

void drw_clear(void);

RColor8 drw_checkcolor(void);

void drw_blend_set(int);
void drw_blend_push(void);
void drw_blend_pop(void);

void drw_color_clear(float, float, float, float);
void drw_color_clear_color(RColor);
void drw_color_clear_color8(RColor8);
void drw_color_clear_color16(RColor16);

void drw_set_line_width(float v);
//void drw_set_screenspace(bool v);
void drw_set_colorbypass(bool v);

void drw_color(double r, double g, double b, double a);
void drw_color4f(float, float, float, float);
void drw_color_c(RColor);
void drw_color_c8(RColor8);
void drw_color_c16(RColor16);
void drw_color_wc(WColor);
//void drw_color_wc8(WColor8);
void drw_color_wc16(WColor16);

void drw_color3f(float, float, float);
void drw_color_u(float);
void drw_color_pop(void);
void drw_color_push(void);

// void drw_color_c(RColor*);
// void drw_color_c8(RColor8*);
// void drw_color_c16(RColor16*);
void drw_alpha(float);
void drw_alpha_pop(void);
void drw_alpha_mult(double v);
void drw_alpha_mult_pop(void);

//	debug stuff
void drw_axis(void);
void drw_axis_nice(void);
void drw_axis_living(void);

//	transforms
void drw_push(void);
void drw_pop(void);

void drw_tess(void* tess);
void drw_transform_apply(WTransform t);
void drw_transform_apply_inverse(WTransform t);
void drw_gtransform_apply(GTransform t);
void drw_transform_deapply(WTransform t);
void drw_gtransform_deapply(GTransform t);

void drw_translate_wvec(WVec3d v);
void drw_scale_wvec(WVec3d v);
void drw_rotate_wvec(WVec3d v);
void drw_rotate_vec3(vec3_t v);
void drw_translate_wvec_inverse(WVec3d v);
void drw_scale_wvec_inverse(WVec3d v);
void drw_rotate_wvec_inverse(WVec3d v);
//void drw_rotate_vec3(vec3_t v);

void drw_scale(float, float, float);
void drw_scale_2f(float, float);
void drw_scale_u(float);
void drw_scale_x(float);
void drw_scale_y(float);
void drw_scale_z(float);
void drw_scale_vec3(vec3_t v);

void drw_translate_x(float);
void drw_translate_y(float);
void drw_translate_z(float);
// void drw_translate_vec2(Vec2* vec);
void drw_translate_rvec(RVec* vec);
void drw_translate_vec3(vec3_t vec);
void drw_translate_rp(RPoint p);
void drw_translate_rp3(RPoint3 p);

void drw_translate2f(float, float);
void drw_translate(float, float, float);
void drw_translate3f(float, float, float);
void drw_rotate_x(float);
void drw_rotate_y(float);
void drw_rotate_z(float);
void drw_rotate_vec3(vec3_t vec);
void drw_rotate(float, float, float);
void drw_rotate_r(double, double, double);

int drw_get_gl_error(void);

//	primitives

void drw_line_r(RLine* line);
void drw_line3_r(RLine3* line);

void drw_line(float, float, float, float);
void drw_line_wp(WPoint, WPoint);
void drw_line_rp(RPoint, RPoint);
void drw_line_3f(float, float, float, float, float, float);
void drw_tri(float, float, float, float, float, float);
void drw_point(void);

// void drw_rect_r(RRect);
void drw_rect_w(WRect);
void drw_rect_r(RRect);

void drw_rect(float, float, float, float);
void drw_rect_rp(RPoint a, RPoint b);
void drw_rect_wp(WPoint a, WPoint b);
void drw_square(float);
void drw_square_at(float, float, float);
void drw_circle(float);
void drw_ellipse(float, float);

// void d_rrect(RRect);

void drw_rline(RLine*);
void drw_poly(WLine*);
void drw_poly_extras(WLine* line);

void drw_polyfill(WLine*);
void drw_rawpoly_2f(float*, int, bool);
void drw_rawpoly_2d(double*, int, bool);
void drw_rawpoly_3f(float*, int, bool);
void drw_rawpoly_3d(double*, int, bool);

void drw_point3f(float, float, float);
void drw_rawpoints_3f(float*, int);
void drw_rawpoints_3d(double*, int);

// void	d_vecline2(Vec2Line *);
// void	d_vecline3(Vec3Line *);
void drw_tris_3d(double* arr, int num, bool closed);
void drw_tristrip_3d(double* arr, int num, bool closed);
void drw_triangle_strip(WLine*);

//	fancy
void drw_text(const char* text, ...);
void drw_text_set_size(int, int);
int  drw_text_get_size(void);
void drw_text_load(const char* path);

//	wash primitives
void drw_verts(WLine*);
void drw_verts_r(RLine*);
void drw_wobject_verts(WObject* obj);
void drw_robject(RObject* obj);
void drw_wline_fill(WLine* line);
void drw_wline(WLine* line);
void drw_wline_strokeonly(WLine* line);
void drw_wobject(WObject*);
void drw_wobject_strokeonly_notransform(WObject*);
void drw_wobject_strokeonly(WObject*);
void drw_wobject_notransform(WObject*);
void drw_wobject_e(WObject*);
//void d_wsequence_e(WSequence*, int);

//	glut prims
void drw_glut_init(void);
void drw_tetrahedron(float r);
void drw_cube(float r);
void drw_sphere(float);
//void d_doda(float);
void drw_teapot(float);
void drw_teapot(float v);
void drw_octahedron(float v);
void drw_dodecahedron(float v);
void drw_icosahedron(float v);

//	debug
void drw_grid(float, int);

//	matrix helpers

void drw_setup_view(void);
void drw_setup_view_ortho(void);
void drw_setup_view_persp(void);

void drw_calculate_scale(void);
void drw_set_retina(double);
void drw_set_downsample(double);
void drw_set_circle_precision_override_limit(bool v);
void drw_set_circle_precision(int sides);
void drw_fill_set(bool);
void drw_fill_pop(void);

void drw_set_fov(double v);
void drw_ortho_set(bool v);
bool drw_ortho_get(void);
void drw_set_screenspace(bool);
bool drw_get_screenspace(void);
void drw_set_screensize(double, double);
void drw_set_framebuffer(double, double);

//	this is for tr I guess (tiled render)
void drw_get_screencoords(double* l, double* r, double* t, double* b, double* n, double* f);

void drw_rgbtri(double gamma);

//	debug - strip this with a macro?
int drw_checkmatrix(void);

void drw_swap(void);
void drw_finish(void);

#endif /* rended_h */
