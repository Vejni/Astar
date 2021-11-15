# include "lib/route_search.h"
# include "lib/priority_queue.h"
# include "lib/map.h"
# include "lib/binary_IO.h"
# include <string.h>
# define GOAL_ID 195977239 // Giralda, Sevilla
# define SOURCE_ID 240949599 //Santa María del Mar, Barcelona


int main(){
	node * nodes = read_binary("data/spain.bin");
    unsigned long goal_index = 461929; // Sevilla
    unsigned long source_index =  519833; // Barcelona
    unsigned long index;
    AStarStatus *status = init_astarstatus(N_NODES);
    status[source_index].g = 0;

    astar(source_index, goal_index, status, nodes, N_NODES);
    save_route(source_index, goal_index, status, nodes, "route.txt");
    
    return 0;
    
}