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

#define D_FONT_SIZE 18

#ifdef DRW_PLATFORM_DARWIN
//#include <GL/glew.h>
//#include <glad/glad.h>
//#include "GL/glus.h"
#define R4_ENABLE_RTMIDI

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#define GLFW_EXPOSE_NATIVE_COCOA
//#include "GLFW/glfw3.h"
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

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef DRW_PLATFORM_IOS
#define DRW_ENABLE_FTGLES

#define DRW_VERTEX_POINTER_IDENT GL_FLOAT
#else

//#define DRW_ENABLE_PNG_SCREENSHOT
#define DRW_ENABLE_FTGL

//#ifndef DRW_PLATFORM_WIN
//#error fuck
#define DRW_VERTEX_POINTER_IDENT GL_DOUBLE
//#endif

#endif

#ifdef DRW_PLATFORM_IOS
#define DRW_TEXT_PROVIDER_ENABLE_FTGLES
#else
#define DRW_TEXT_PROVIDER_ENABLE_FTGL
#endif

#define DRW_TEXT_PROVIDER_ENABLE_HERSHEY
#define DRW_TEXT_PROVIDER_ENABLE_HPVEC
#define DRW_TEXT_PROVIDER_ENABLE_ASTEROIDS

#ifndef DRW_PLATFORM_IOS
#define DRW_ENABLE_SNOOP

#endif

#endif /* drw_config_h */
