

#include "drw.h"
#include <string.h>

static int font_size = 12;


int drw_check_version_match(const char* header_generated_version)
{
	char* v = drw_get_version_string();
	bool res = (0 == strcmp(v, header_generated_version));
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
	printf("%d.%d.%d\n", DRW_VERSION_MAJOR, DRW_VERSION_MINOR,
	       DRW_VERSION_PATCH);
}

void drw_font_size_set(int sz)
{
	printf("Unified font size %d\n", sz);
	font_size = sz;
}

double drw_font_size_real_get(void)
{
	int w, h;
	drw_query_framebuffer(&w, &h);
	
	double sz = drw_font_size_get();
	
	double sc = 1. / (h/sz*8);
	
	return sc;
}

int drw_font_size_get(void)
{
	return font_size;
}

int drw_lib_init(void)
{
	//	dummy method, gonna call the font functions to find out of dead code stripping
	//	is the culprit for these link errors, or some other c++ fuckery?

	drw_font_load("null_path");
	drw_font_draw("hello");
	return 0;
}

int drw_lib_deinit(void)
{
	return 0;
}
