//
//  drw_text.h
//  drw
//
//  Created by vs on 12/10/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_text_h
#define drw_text_h

enum
{
	DRW_TEXT_PROVIDER_FTGL,
	DRW_TEXT_PROVIDER_HPVEC,
	DRW_TEXT_PROVIDER_HERSHEY,
	DRW_TEXT_PROVIDER_ASTEROIDS,
	DRW_TEXT_PROVIDER_NONE
};


void   drw_text_get_bbox(const char*, unsigned int, float*);
double drw_text_get_width(const char*);
int    drw_text_get_fontsize(void);
void   drw_text_set_align(int, int);

int	drw_text_register_provider(void);

#endif /* drw_text_h */
