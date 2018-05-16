//
//  drw_config.h
//  drw
//
//  Created by vs on 5/16/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_config_h
#define drw_config_h

#include "drw_platform.h"

#ifdef DRW_PLATFORM_DARWIN

//#include <GL/glew.h>
//#include <glad/glad.h>
//#include "GL/glus.h"
#define R4_ENABLE_RTMIDI

#define GLFW_EXPOSE_NATIVE_COCOA
#include "GLFW/glfw3.h"
#include <OpenGL/glu.h>

//#include <GLUT/glut.h>
#endif

#ifdef DRW_PLATFORM_WIN
//#include <glad/glad.h>

//#include <gl/glew.h>

//#if defined(WIN32) || defined(WIN64)
#define strcasecmp _stricmp
//#endif /* Def WIN32 or Def WIN64 */

#include <GLFW/glfw3.h>
#include <gl/GL.h>
#endif

#ifdef DRW_PLATFORM_LINUX

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#endif

#ifdef DRW_PLATFORM_IOS
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES3/gl.h>
#endif

#endif /* drw_config_h */
