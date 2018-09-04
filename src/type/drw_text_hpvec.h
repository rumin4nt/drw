//
//  drw_text_hpvec.h
//  drw
//
//  Created by vs on 4/27/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_text_hpvec_h
#define drw_text_hpvec_h

#define HPVEC_FONT_SIZE 16

typedef struct HPGlyph
{
	unsigned long num;
	const int*    points;
} HPGlyph;

void drw_text_hpvec_set_alignment(int h, int v);
void drw_text_hpvec_draw(const char* text);

HPGlyph** drw_text_hpvec_render(const char* text);
void      drw_text_hpvec_get_offset_scale(const char* text, double* ox, double* oy, double* sc);

#endif /* drw_text_hpvec_h */
