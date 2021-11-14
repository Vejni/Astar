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
    PqElem *pq = init_pq(source_index, 0);
    AStarStatus *status = init_astarstatus(N_NODES);
    int n = 5;
    for(int i = 0; i<3; i++){
        status[i].g = 1;
        status[i].h = i;
        double cost = status[i].g + status[i].h;
        add_with_priority(pq, i, cost);
    }

    show_queue(pq);
    pq = increase_priority(pq, 2, 0.25);
    show_queue(pq);
    pq = increase_priority(pq, 1, 0.75);
    show_queue(pq);
    return 0;
    
}