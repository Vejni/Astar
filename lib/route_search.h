#include "map.h"
#include "priority_queue.h"
#include "heuristics.c"
#include <stdbool.h>
#include <stdlib.h>
#include <values.h>

typedef struct {
    double g, h;
    unsigned long parent;
    bool expanded;
} AStarStatus;

AStarStatus *init_astarstatus(unsigned long n_nodes){
    AStarStatus *status = (AStarStatus*) malloc(n_nodes*sizeof(AStarStatus));
    if(status == NULL){
        exit;
    }
    for(unsigned long index = 0; index < n_nodes; index++){
        status[index].expanded = false;
        status[index].g = DBL_MAX;
    }
    return status;
}

PqElem* update_neighbours_distance(unsigned long index, unsigned long goal, AStarStatus *status, PqElem *pq, node *nodes, int heuristic_func, double heuristic_param){
    unsigned long index_neig;
    double new_distance;
    for(int i=0; i<nodes[index].nsucc; i++){
        index_neig = nodes[index].successors[i];
        if(status[index_neig].expanded == false){
            new_distance = status[index].g + heuristic(nodes[index], nodes[index_neig], heuristic_func, 1);

            if(status[index_neig].g == DBL_MAX){
                status[index_neig].g = new_distance;
                status[index_neig].parent = index;
                status[index_neig].h = heuristic(nodes[index_neig], nodes[goal], heuristic_func, heuristic_param);
                pq = add_with_priority(pq, index_neig, status[index_neig].g + status[index_neig].h);
            }
            else if(status[index_neig].g > new_distance){
                status[index_neig].g = new_distance;
                status[index_neig].parent = index;
                pq = increase_priority(pq, index_neig, status[index_neig].g + status[index_neig].h);
            }
        }
    }
    return pq;
}


void save_results(unsigned long source_index, unsigned long goal_index, unsigned long n_nodes, AStarStatus *status, node *nodes, PqElem *pq, char* path_route, char* path_stats){
    FILE *file_route;
    FILE *file_stats;
    if ((file_route = fopen(path_route ,"w")) == NULL){
        printf("Cannot write on file: '%s'\n", path_route);
        exit(1);
    }

    if ((file_stats = fopen(path_stats ,"w")) == NULL){
        printf("Cannot write on file: '%s'\n", path_stats);
        exit(1);
    }

    fprintf(file_route, "longitude latitude\n");
    unsigned long index = goal_index;
    int n_route_nodes = 0;
    while(index != source_index){
        fprintf(file_route, "%f, %f\n", nodes[index].lon, nodes[index].lat);
        index = status[index].parent;
        n_route_nodes++;
    }

    int n_open_nodes = 0;
    PqElem *elem = pq;
    do{
        n_open_nodes++;
        elem = elem->next;
    } while(elem->next != NULL);

    int n_closed_nodes = 0;
    for(index = 0; index < n_nodes; index++){
        if (status[index].expanded == true){
            n_closed_nodes++;
        }
    }


    // print stats
    fprintf(file_stats, "Route distance: %f\n", status[goal_index].g);
    fprintf(file_stats, "Number of route nodes: %d\n", n_route_nodes);
    fprintf(file_stats, "Number of open nodes: %d\n", n_open_nodes);
    fprintf(file_stats, "Number of closed nodes: %d\n", n_closed_nodes);

    fclose(file_route);
    fclose(file_stats);
    return;
};

void astar(unsigned long source_index, unsigned long goal_index, node *nodes, unsigned long n_nodes, char * path_route, char * stats_route, int heuristic_func, double heuristic_param, int save){
    AStarStatus *status = init_astarstatus(n_nodes);
    status[source_index].g = 0;

    PqElem *pq = init_pq(source_index, 0);
    unsigned long index;
    do{
        index = extract_min(&pq);
        status[index].expanded = true;
        pq = update_neighbours_distance(index, goal_index, status, pq, nodes, heuristic_func, heuristic_param);
    } while(index != goal_index);

    if (save != 0) save_results(source_index, goal_index, n_nodes, status, nodes, pq, path_route, stats_route);
    return;
}
