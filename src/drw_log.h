//
//  drw_log.h
//  drw
//
//  Created by vs on 12/10/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_log_h
#define drw_log_h

//#define DEBUG 3

#if defined(DEBUG) && DEBUG > 0
#define drw_log(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
__FILE__, __LINE__, __func__, ##args, "\n")
#else
#define drw_log(fmt, args...) /* Don't do anything in release builds */
#endif
/*
#include <stdio.h>

#ifdef DEBUG
#define drw_log(char* fmt, ...);
#else
#define drw_log(fmt, ...) do {} while (0);
#endif

*/
/*
#ifdef DEBUG
#define drw_log(x) printf x
#else
#define drw_log(x) do {} while (0)
#endif
*/

/*
#ifdef DEBUG
void drw_log(char* fmt, ...);
#else
#define drw_log(fmt, ...) do {} while (0);
#endif
*/

#endif /* drw_log_h */
