//
//  drw_platform.h
//  r4
//
//  Created by Andrew Macfarlane on 4/8/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef drw_platform_h
#define drw_platform_h

#ifdef _WIN64
#define DRW_PLATFORM_WIN
#elif _WIN32
#define DRW_PLATFORM_WIN

#elif __APPLE__
#include <TargetConditionals.h>

#if TARGET_OS_IPHONE
#define DRW_PLATFORM_IOS
//#define DRW_PLATFORM_IOS
#elif TARGET_IPHONE_SIMULATOR
//#define DRW_PLATFORM_IOS_SIM
//#define DRW_PLATFORM_IOS
#define DRW_PLATFORM_IOS

#ifndef PATH_MAX
#define PATH_MAX 2048
#endif

#else

#define DRW_PLATFORM_DARWIN 1
#undef DRW_PLATFORM_IOS_SIM
#undef DRW_PLATFORM_IOS
#endif

#elif __linux
#define DRW_PLATFORM_LINUX

#elif __linux__
#define DRW_PLATFORM_LINUX
#elif __unix // all unices not caught above

#define DRW_PLATFORM_UNIX
#error unix
#elif __posix

#define DRW_PLATFORM_POSIX
#error posix
#else

#error unable to determine platform
#endif

#endif /* drw_h */
