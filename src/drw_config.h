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
//#define R4_ENABLE_RTMIDI

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#define GLFW_EXPOSE_NATIVE_COCOA

#define GL_GLEXT_PROTOTYPES 1
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
#define TEMP_GL_LEGACY_BACKPORT

#ifdef TEMP_GL_LEGACY_BACKPORT
#include <OpenGLES/ES1/gl.h>
#else
#include <OpenGLES/ES2/gl.h>
#endif
#include <OpenGLES/ES3/gl.h>
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef DRW_PLATFORM_IOS
#ifndef TARGET_IPHONE_SIMULATOR
#define DRW_ENABLE_FTGLES
#endif

#define DRW_VERTEX_POINTER_IDENT GL_FLOAT
#else

//#define DRW_ENABLE_PNG_SCREENSHOT
#ifndef DRW_PLATFORM_IOS
#define DRW_ENABLE_FTGL
#endif

//#ifndef DRW_PLATFORM_WIN
//#error fuck
#define DRW_VERTEX_POINTER_IDENT GL_DOUBLE
//#endif

#endif

#ifdef DRW_PLATFORM_IOS
#include <TargetConditionals.h>
#define DRW_TYPE_PROVIDER_ENABLE_FTGLES

#ifndef TARGET_IPHONE_SIMULATOR
#define DRW_TYPE_PROVIDER_ENABLE_FTGLES
#endif

#else
#define DRW_TYPE_PROVIDER_ENABLE_FTGL
#endif

#define DRW_TYPE_PROVIDER_ENABLE_HPVEC
#define DRW_TYPE_PROVIDER_ENABLE_ASTEROIDS

#ifndef DRW_PLATFORM_LINUX
#define DRW_TYPE_PROVIDER_ENABLE_HERSHEY
#endif

#ifndef DRW_PLATFORM_IOS
#define DRW_ENABLE_SNOOP
#endif

#endif /* drw_config_h */
