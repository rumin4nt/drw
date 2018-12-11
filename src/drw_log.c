//
//  drw_log.c
//  drw
//
//  Created by vs on 12/10/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_log.h"


#define DRW_LOG_MAX 256
#include <stdarg.h>

void drw_log(char* format, ...)
{
	
	char buf[DRW_LOG_MAX];
	sprintf(buf, "%s", format);
	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);
	
#ifdef DEBUG
	printf("[drw ]: %s\n", buf);
#else
	printf("DO NOTHING LATER: %s\n", buf);
#endif
}
