//
//  drw_export.h
//  drw
//
//  Created by vs on 6/13/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_export_h
#define drw_export_h



#include <drw/src/drw_config.h>
#include <stdbool.h>

#ifdef DRW_ENABLE_PNG

bool drw_export_png(const char *filename, int w, int h);
bool drw_export_png_save(const char *filename, uint8_t *pixels, int w, int h);
//bool drw_export_png(


#endif


#endif /* drw_export_h */
