# include "lib/map.h"
# include "lib/binary_IO.h"
# include <string.h>

int main(int argc, char *argv[]) {
	char * path = malloc(50);
	strcpy(path, "data/spain.csv");

	node * nodes = create_map(path);
	write_binary("data/spain.bin", nodes);
	nodes = read_binary("data/spain.bin");

	print_nodes(nodes, 1000);
	return 0;
}
