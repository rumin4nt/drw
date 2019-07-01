//
//  drw_type_hershey.c
//  drw
//
//  Created by vs on 1/11/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_type_hershey.h"

//#include <drw/src/drw_config.h>

#include <drw/src/type/drw_type.h>
#ifdef DRW_TYPE_PROVIDER_ENABLE_HERSHEY

/*
 	http://sol.gfxile.net/hershey/index.html
 
 */


//	linux (gnu? REAlly does not like this generated file lol

//#include <hershey/futural.h>


typedef struct {
	int count;
	int height;
	int width[96];
	int realwidth[96];
	int size[96];
	char* data[96];
} HersheyFontRec;

#include "../../contrib/fonts/hershey_decon/futural.h"
#include "../../contrib/fonts/hershey_decon/timesr.h"

#define HFONT rowmans

//#include <r4/src/r4_config.h>
#include <stdbool.h>
//#include <drw/drw.h>
#include <drw/src/drw_render_gl.h>



static int font = 0;
//const static  int hf_counts[2] = { 96, 96 };
//const static char hf_height[2] = {32, 32};
//const static hf_widths[2][96] = {futural_width, timesr_width};


#ifdef DRW_EXT_R4
void* drw_type_hershey_render(const char* text);
#endif

static int fnum = 0;
static HersheyFontRec* recs = NULL;

static HersheyFontRec mk_timesr(void)
{
	HersheyFontRec rec;
	rec.count = timesr_count;
	rec.height = timesr_height;
	memcpy(rec.width,timesr_width, sizeof(timesr_width));
	memcpy(rec.realwidth, timesr_realwidth, sizeof(timesr_realwidth));
	memcpy(rec.size, timesr_size, sizeof(timesr_size));
	memcpy(rec.data, timesr, sizeof(timesr));
	return rec;
}
static HersheyFontRec mk_futural(void)
{
	HersheyFontRec rec;
	rec.count = futural_count;
	rec.height = futural_height;
	memcpy(rec.width, futural_width, sizeof(futural_width));
	memcpy(rec.realwidth, futural_realwidth, sizeof(futural_realwidth));
	memcpy(rec.size, futural_size, sizeof(futural_size));
	memcpy(rec.data, futural, sizeof(futural));
	return rec;
}

static void load_fonts(void)
{
	HersheyFontRec futural = mk_futural();
	
	fnum++;
	recs = realloc(recs, fnum * sizeof( HersheyFontRec));
	recs[fnum -  1] = futural;
	
	HersheyFontRec timesr = mk_timesr();
	fnum++;
	recs = realloc(recs, fnum * sizeof( HersheyFontRec));
	recs[fnum -  1] = timesr;

	font = 1;
}



void drw_type_hershey_bbox(const char* str, unsigned long num, float* data);
void drw_type_hershey_draw(const char* text);

void drw_type_hershey_initialize(void)
{
	load_fonts();
	
#ifdef DRW_EXT_R4
	drw_type_provider_register("hershey", drw_type_hershey_draw, drw_type_hershey_bbox, drw_type_hershey_render);
#else
	drw_type_provider_register("hershey", drw_type_hershey_draw, drw_type_hershey_bbox, NULL);

#endif
}

void drw_type_hershey_bbox(const char* str, unsigned long num, float* data)
{
}

static double get_real_sz()
{
	double x = 16;
	//double   y = HPVEC_GRID_SIZE;
	unsigned w, h;
	
	if (!drw_screenspace_get()) {
		drw_screensize_get(&w, &h);
		x /= w;
		//y /= w;
		//		x *= .5;
		//		y *= .5;
	}
	return x;
}


void drw_type_hershey_draw(const char* text)
{

	//#ifdef RPLATFORM_DARWIN
	//#ifdef __APPLE__
	drw_push();
	char c    = (char)0;
	bool done = false;
	int  i    = 0;
	double rsz = get_real_sz();
	double acc = 0;
	while (!done) {
		c = text[i];
		
		//printf("%c", c);
		if (c == '\0') {

			done = true;
		} else {
			//int w = futural_width;

			//int h = futural_height;
			//done = true;
			int idx = (int)(c);
			idx -= 32; //hershey fonts don't have the 32 nonprintables
			//idx -= 1; //not sure what this is about
			//const char* local     = &futural;

			//			int	 corrected = -1 + idx;
			int corrected = 0 + idx;
			if (corrected < 0)
				break;
//			int	 sz   = recs[font].size[corrected];
//			const char* data = recs[font].data[corrected];
//			int  w    = recs[font].width[corrected];
			int sz   = futural_size[corrected];
			const char* data = futural[corrected];
			int  w    = futural_width[corrected];

			
			
			drw_push();
			
			drw_scale(1, -1, 1);
			drw_scale_u(.25);
			
			drw_translate(acc , 0, 0);
			acc += w * .25 * .25 * .25;
			for (int i2 = 0; i2 < sz; i2 += 4) {
				const char a  = data[i2];
				const char b  = data[i2 + 1];
				const char c  = data[i2 + 2];
				const char d  = data[i2 + 3];
				int	a2 = (int)a;
				int	b2 = (int)b;
				int	c2 = (int)c;
				int	d2 = (int)d;

				//printf("%c %c", a, b);
				double arr[4];
				
				double ax = (double)a2 * rsz;
				double ay = (double)b2 * rsz;
				double bx = (double)c2 * rsz;
				double by = (double)d2 * rsz;
				arr[0] = ax;
				arr[1] = ay;
				arr[2] = bx;
				arr[3] = by;

				glVertexPointer(2, GL_DOUBLE, 0, &arr);
				glDrawArrays(GL_LINES, 0, 4);
			}
			drw_pop();
			//todo: bring this back
			// glColor4f(1,0,1,1);
			//glTranslated(16,0,0);
			//glVertexPointer(2, GL_INT, 0, &data);
			//glDrawArrays(GL_LINES, 0, sz);

			//printf("\n");
			i++;
			
		}
	}

	
	drw_pop();

	
}


#ifdef DRW_EXT_R4
void* drw_type_hershey_render(const char* text)
{
	
	//drw_push();
	char c    = (char)0;
	bool done = false;
	int  i    = 0;
	double rsz = get_real_sz();
	double acc = 0;
	RObject* obj = r_object_create();
	
	while (!done) {
		c = text[i];
		
		//printf("%c", c);
		if (c == '\0') {
			
			done = true;
		} else {
			//int w = futural_width;
			
			//int h = futural_height;
			//done = true;
			int idx = (int)(c);
			idx -= 32; //hershey fonts don't have the 32 nonprintables
			//idx -= 1; //not sure what this is about
			//const char* local     = &futural;
			
			//			int	 corrected = -1 + idx;
			int corrected = 0 + idx;
			if (corrected < 0)
				break;
			//			int	 sz   = recs[font].size[corrected];
			//			const char* data = recs[font].data[corrected];
			//			int  w    = recs[font].width[corrected];
			int sz   = futural_size[corrected];
			const char* data = futural[corrected];
			int  w    = futural_width[corrected];
			
			
			
//			drw_push();
//
//			drw_scale(1, -1, 1);
//			drw_scale_u(.25);
//
//			drw_translate(acc , 0, 0);
		
			for (int i2 = 0; i2 < sz; i2 += 4) {
				const char a  = data[i2];
				const char b  = data[i2 + 1];
				const char c  = data[i2 + 2];
				const char d  = data[i2 + 3];
				int	a2 = (int)a;
				int	b2 = (int)b;
				int	c2 = (int)c;
				int	d2 = (int)d;
				
				//printf("%c %c", a, b);
				double arr[4];
				
				double ax = (double)a2 * rsz;
				double ay = (double)b2 * rsz;
				double bx = (double)c2 * rsz;
				double by = (double)d2 * rsz;
				arr[0] = ax;
				arr[1] = ay;
				arr[2] = bx;
				arr[3] = by;
				RLine* line = r_line_create2f(ax, ay, bx, by);
				r_line_scale(line, 1, -1);
				
				r_line_scale(line, .25, .25);
				r_line_translate(line, acc, 0);
				
				r_object_add_line(obj, line);
				
//				glVertexPointer(2, GL_DOUBLE, 0, &arr);
//				glDrawArrays(GL_LINES, 0, 4);
			}
				acc += w * .25 * .25 * .25 * .25;
			//drw_pop();
			//todo: bring this back
			// glColor4f(1,0,1,1);
			//glTranslated(16,0,0);
			//glVertexPointer(2, GL_INT, 0, &data);
			//glDrawArrays(GL_LINES, 0, sz);
			
			//printf("\n");
			i++;
			
		}
	}
	
	
	//drw_pop();
	return obj;
}
#endif

#endif
