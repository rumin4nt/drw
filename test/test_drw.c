

#include <drw/drw.h>

int main(int argc, char** argv)
{
	//	simple test program for wash.
	//	create some geometry, write an svg, possibly a png?

	printf("Running drw tests.\n");
	printf("making sure drw header matches library.\n");

	char* buf = calloc(256, sizeof(char));
	sprintf(buf, "%d_%d_%d", DRW_VERSION_MAJOR, DRW_VERSION_MINOR,
		DRW_VERSION_PATCH);

	if (drw_check_version_match(buf)) {
		printf("Version matches.\n");
	} else {
		printf("Version does not match!!\n");
		return 88;
	}

	// WLine   line;
	// WObject object;
	drw_print_version();
	return 0;
}
