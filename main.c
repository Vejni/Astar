# include "lib/route_search.h"
# include "lib/priority_queue.h"
# include "lib/map.h"
# include "lib/binary_IO.h"
# include <string.h>
# include <unistd.h>

int main(int argc, char *argv[]) {
	char * path = malloc(50);
	strcpy(path, "data/spain.bin");

	if( access( path, R_OK ) != 0 ) {
    // file does not exists
		node * nodes_from_csv = create_map("data/spain.csv");
		write_binary(path, nodes_from_csv);
		print_valences(nodes_from_csv);
	}

	node * nodes = read_binary(path);

	unsigned long goal_index = 461929;  // Sevilla
	unsigned long source_index =  519833;  // Barcelona
	unsigned long index;

	int save_files = argc>1? atoi(argv[1]) : 1;
	char * route_path = argc>2? argv[2] : "/route.txt";
	char * stats_path = argc>3? argv[3] : "/stats.txt";
	int heuristic_func = argc>4? atof(argv[4]) : 1;
	double heuristic_param = argc>5? atof(argv[5]) : 1.0;


  	astar(source_index, goal_index, nodes, N_NODES, route_path, stats_path, heuristic_func, heuristic_param, save_files);

	return 0;
}
