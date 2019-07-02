

#include "drw.h"
#include <string.h>

#include "src/drw_log.h"


//16:24 <                       gruebite@> #define ERR(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)

int drw_check_version_match(const char* header_generated_version)
{
	char* v   = drw_get_version_string();
	bool  res = (0 == strcmp(v, header_generated_version));
	free(v);
	return res;
}

char* drw_get_version_string(void)
{
	char* buf = calloc(256, sizeof(char));
	sprintf(buf, "%d.%d.%d", DRW_VERSION_MAJOR, DRW_VERSION_MINOR,
		DRW_VERSION_PATCH);
	return buf;
}

void drw_print_version(void)
{
	drw_log("%d.%d.%d", DRW_VERSION_MAJOR, DRW_VERSION_MINOR, DRW_VERSION_PATCH);
}


int drw_lib_init(void)
{
	//	dummy method, gonna call the font functions to find out of dead code stripping
	//	is the culprit for these link errors, or some other c++ fuckery?

	//drw_type_load_ttf("null_path");
	//drw_type_draw("hello");
	return 0;
}

int drw_lib_deinit(void)
{
	return 0;
}
