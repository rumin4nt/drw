//
//  drw_gl.h
//  drw-ios
//
//  Created by vs on 5/28/19.
//  Copyright © 2019 ruminant. All rights reserved.
//

#ifndef drw_gl_h
#define drw_gl_h

#include "drw_config.h"
#ifdef RPLATFORM_IOS
//#error hi

#ifdef R4_GLES_1
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#endif

#ifdef R4_GLES_2
#include <OpenGLES/ES2/gl.h>
#endif

#ifdef R4_GLES_3
#include <OpenGLES/ES3/gl.h>
#endif

#endif

#endif /* drw_gl_h */
