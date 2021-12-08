#include "map.h"
#include "priority_queue.h"
#include "heuristics.c"
#include <stdbool.h>
#include <stdlib.h>
#include <values.h>

typedef struct {
    float g, h;
    unsigned long parent;
    bool visited;
} AStarStatus;

AStarStatus *init_astarstatus(unsigned long n_nodes){
    AStarStatus *status = (AStarStatus*) malloc(n_nodes*sizeof(AStarStatus));
    if(status == NULL){
        exit;
    }
    for(unsigned long index = 0; index < n_nodes; index++){
        status[index].visited = false;
        status[index].g = MAXFLOAT;
    }
    return status;
}

PqElem* update_neighbours_distance(unsigned long index, unsigned long goal, AStarStatus *status, PqElem *pq, node *nodes, int heuristic_func, double heuristic_param){
    unsigned long index_neig;
    float new_distance;
    for(int i=0; i<nodes[index].nsucc; i++){
        index_neig = nodes[index].successors[i];
        if(status[index_neig].visited == false){
            new_distance = status[index].g + haversine_dist(nodes[index], nodes[index_neig]);

            if(status[index_neig].g == MAXFLOAT){
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


void save_results(unsigned long source_index, unsigned long goal_index, AStarStatus *status, node *nodes, PqElem *pq, char* path_route, char* path_stats){
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
    double eucl_distance = status[source_index].h;
    int n_route_nodes = 0;
    while(index != source_index){
        fprintf(file_route, "%f, %f\n", nodes[index].lon, nodes[index].lat);
        index = status[index].parent;
        n_route_nodes++;
    }

    int n_non_visited_nodes = 0;
    PqElem *elem = pq;
    do{
        n_non_visited_nodes++;
        elem = elem->next;
    } while(elem->next != NULL);


    // print stats
    fprintf(file_stats, "Route distance: %f\n", status[goal_index].g);
    fprintf(file_stats, "Number of visited nodes: %d\n", n_route_nodes);
    fprintf(file_stats, "Number of explored but non visited nodes: %d\n", n_non_visited_nodes);

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
        status[index].visited = true;
        pq = update_neighbours_distance(index, goal_index, status, pq, nodes, heuristic_func, heuristic_param);
    } while(index != goal_index);

    if (save != 0) save_results(source_index, goal_index, status, nodes, pq, path_route, stats_route);
    return;
}
