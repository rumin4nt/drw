

#include "drw.h"

int drw_check_version_match(const char* header_generated_version)
{
	char* v = drw_get_version_string();
	return (0 == strcmp(v, header_generated_version));
	free(v);
}

char* drw_get_version_string(void)
{
	char* buf = calloc(256, sizeof(char));
	sprintf(buf, "%d_%d_%d", DRW_VERSION_MAJOR, DRW_VERSION_MINOR,
		DRW_VERSION_PATCH);
	return buf;
}

void drw_print_version(void)
{
	printf("%d_%d_%d\n", DRW_VERSION_MAJOR, DRW_VERSION_MINOR,
	       DRW_VERSION_PATCH);
}
