# include "lib/map.h"
# include <string.h>

int main(int argc, char *argv[]) {
	char * path = malloc(50);
	strcpy(path, "data/test.txt");

	create_map(path);
	return 0;
}
