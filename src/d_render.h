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
#include <wsh/wsh.h>

#pragma GCC visibility push(default)

#include <wsh/wsh.h>

//#include <r4/src/core/d_math.h>
#include <grdn/grdn.h>
#include <r4/r4.h>

#include <gl-matrix/gl-matrix.h>

enum
{
	R4_BLEND_MODE_NONE,
	R4_BLEND_MODE_NORMAL,
	R4_BLEND_MODE_ADD,
	R4_BLEND_MODE_MULTIPLY,
	R4_BLEND_MODE_SCREEN,
	R4_BLEND_MODE_SUBTRACT
};

void d_init(void);
void d_deinit(void);

//	color

void d_clear(void);

RColor8 d_check_color(void);

void d_set_blend(int);

void d_color_clear(float, float, float, float);
void d_color_clear_color(RColor);
void d_color_clear_color8(RColor8);
void d_color_clear_color16(RColor16);

void d_set_line_width(float v);

void d_set_colorbypass(bool v);

void d_color(double r, double g, double b, double a);
void d_color4f(float, float, float, float);
void d_color_c(RColor);
void d_color_c8(RColor8);
void d_color_c16(RColor16);
void d_color_wc(WColor);
//void d_color_wc8(WColor8);
void d_color_wc16(WColor16);

void d_color3f(float, float, float);
void d_color_u(float);
void d_color_pop(void);
void d_color_push(void);

// void d_color_c(RColor*);
// void d_color_c8(RColor8*);
// void d_color_c16(RColor16*);
void d_alpha(float);
void d_alpha_pop(void);
void d_alpha_mult(double v);
void d_alpha_mult_pop(void);

//	debug stuff
void d_axis(void);
void d_nice_axis(void);
void d_axis_living(void);

//	transforms
void d_push(void);
void d_pop(void);

void d_transform_apply(WTransform t);
void d_gtransform_apply(GTransform t);
void d_transform_deapply(WTransform t);
void d_gtransform_deapply(GTransform t);

void d_translate_wvec(WVec3d v);
void d_scale_wvec(WVec3d v);
void d_scale_vec3(vec3_t v);
void d_rotate_wvec(WVec3d v);
void d_rotate_vec3(vec3_t v);

void d_scale(float, float, float);
void d_scale_2f(float, float);
void d_scale_u(float);
void d_scale_x(float);
void d_scale_y(float);
void d_scale_z(float);

void d_translate_x(float);
void d_translate_y(float);
void d_translate_z(float);
// void d_translate_vec2(Vec2* vec);
void d_translate_vec3(vec3_t vec);
void d_translate_rp(RPoint p);
void d_translate_rp3(RPoint3 p);

void d_translate2f(float, float);
void d_translate(float, float, float);
void d_translate3f(float, float, float);
void d_rotate_x(float);
void d_rotate_y(float);
void d_rotate_z(float);
void d_rotate_vec3(vec3_t vec);
void d_rotate(float, float, float);

int d_get_gl_error(void);

//	primitives

void d_line_r(RLine* line);
void d_line3_r(RLine3* line);

void d_line(float, float, float, float);
void d_line_p(WPoint, WPoint);
void d_line_3f(float, float, float, float, float, float);
void d_tri(float, float, float, float, float, float);
void d_point(void);

// void d_rect_r(RRect);
void d_rect_w(WRect);
void d_rect_r(RRect);

void d_rect(float, float, float, float);
void d_rect_rp(RPoint a, RPoint b);
void d_rect_wp(WPoint a, WPoint b);
void d_square(float);
void d_square_at(float, float, float);
void d_circle(float);
void d_ellipse(float, float);

// void d_rrect(RRect);

void d_rline(RLine*);
void d_poly(WLine*);
void d_poly_extras(WLine* line);

void d_polyfill(WLine*);
void d_rawpoly_2f(float*, int, bool);
void d_rawpoly_2d(double*, int, bool);
void d_rawpoly_3f(float*, int, bool);
void d_rawpoly_3d(double*, int, bool);

void d_point3f(float, float, float);
void d_rawpoints_3f(float*, int);
void d_rawpoints_3d(double*, int);

// void	d_vecline2(Vec2Line *);
// void	d_vecline3(Vec3Line *);
void d_tris_3d(double* arr, int num, bool closed);
void d_tristrip_3d(double* arr, int num, bool closed);
void d_triangle_strip(WLine*);

//	fancy
void d_text(const char* text, ...);
void d_text_size(int, int);
void d_text_load(const char* path);

//	wash primitives
void d_verts(WLine*);
void d_verts_r(RLine*);
void d_wobject_verts(WObject* obj);
void d_robject(RObject* obj);
void d_wline_fill(WLine* line);
void d_wline(WLine* line);
void d_wline_strokeonly(WLine* line);
void d_wobject(WObject*);
void d_wobject_strokeonly_notransform(WObject*);
void d_wobject_strokeonly(WObject*);
void d_wobject_notransform(WObject*);
void d_wobject_e(WObject*);
void d_wsequence_e(WSequence*, int);

//	glut prims
void d_glut_init(void);
void d_tetrahedron(float r);
void d_cube(float r);
void d_sphere(float);
void d_doda(float);
void d_teapot(float);
void d_teapot(float v);
void d_octahedron(float v);
void d_dodecahedron(float v);
void d_icosahedron(float v);

//	debug
void d_grid(float, int);

//	matrix helpers
void d_setup_view(void);
void d_setup_view_ortho(void);
void d_setup_view_persp(void);

void d_calculate_scale(void);
void d_set_retina(double);
void d_set_downsample(double);
void d_set_circle_precision_override_limit(bool v);
void d_set_circle_precision(int sides);
void d_set_fill(bool);
void d_pop_fill(void);

void d_set_fov(double v);
void d_set_ortho(bool v);
void d_set_screenspace(bool);
void d_set_screensize(double, double);
void d_set_framebuffer(double, double);

void d_rgbtri(double gamma);

//	debug - strip this with a macro?
int d_check_matrix(void);

#include <gpc/gpc.h>

void d_gpc_polygon_outline(GPCRec*);
void d_gpc_polygon(GPCRec*);
void d_gpc_verts(void*);
void d_gpc_triwire(void* dat);
void d_gpc_tristrip(void*);

#endif /* rended_h */
