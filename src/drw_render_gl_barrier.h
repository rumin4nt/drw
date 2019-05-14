//
//  drw_render_gl_barrier.h
//  drw
//
//  Created by vs on 5/7/19.
//  Copyright © 2019 ruminant. All rights reserved.
//

#ifndef drw_render_gl_barrier_h
#define drw_render_gl_barrier_h

#define R4_GLES_1
#ifndef R4_GLES_1
#include "src/drw_render_gl.h"
#elif R4_GLES_2
#include "src/drw_render_gl_2.h"
#endif

#endif /* drw_render_gl_barrier_h */
