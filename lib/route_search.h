#include "map.h"
#include "priority_queue.h"
#include "math.h"
#include <stdbool.h>
#include <stdlib.h>
#include <values.h>
#define PI 3.14159265358979323846

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

float haversine_dist(node node1, node node2){
    /*
    Geodesic distance between two nodes with the haversine equation (high precission)
    */
    double phi1 = node1.lat*PI/180.0;
    double phi2 = node2.lat*PI/180.0;
    double phi_dif =  phi1 - phi2;
    double lambda_dif = (node1.lon-node2.lon)*PI/180.0;
    double a = 2*asin(sqrt(pow(sin(phi_dif*0.5), 2) + cos(phi2)*cos(phi1)*pow(sin(lambda_dif*0.5), 2)));
    return 6371008.8*a;
};

float heuristic(node current_node, node goal){
    return haversine_dist(current_node, goal);    
};

PqElem* update_neighbours_distance(unsigned long index, unsigned long goal, AStarStatus *status, PqElem *pq, node *nodes){
    unsigned long index_neig;
    float new_distance;
    for(int i=0; i<nodes[index].nsucc; i++){
        index_neig = nodes[index].successors[i];
        if(status[index_neig].visited == false){
            new_distance = status[index].g + haversine_dist(nodes[index], nodes[index_neig]);
            
            if(status[index_neig].g == MAXFLOAT){
                status[index_neig].g = new_distance;
                status[index_neig].parent = index;
                status[index_neig].h = heuristic(nodes[index_neig], nodes[goal]);
                pq = add_with_priority(pq, index_neig, status[index_neig].g+status[index_neig].h);
            }
            else if(status[index_neig].g > new_distance){
                status[index_neig].g = new_distance;
                status[index_neig].parent = index;
                pq = increase_priority(pq, index_neig, status[index_neig].g+status[index_neig].h);
            }
        }
    }
    return pq;

}


void astar(unsigned long source_index, unsigned long goal_index, AStarStatus *status, node *nodes, unsigned long n_nodes){
    PqElem *pq = init_pq(source_index, 0);
    unsigned long index = n_nodes+1;
    while(index != goal_index){
        index = extract_min(&pq);
        status[index].visited = true;
        pq = update_neighbours_distance(index, goal_index, status, pq, nodes);
    }
    return;
}

void save_route(unsigned long source_index, unsigned long goal_index, AStarStatus *status, node *nodes, char* path){
    FILE *file;
    if ((file = fopen(path ,"w")) == NULL){
        printf("Cannot open file.\n");
        exit(1);
    }
    fprintf(file, "longitude latitude\n");
    unsigned long index = goal_index;
    while(index != source_index){
        fprintf(file, "%f %f\n", nodes[index].lon, nodes[index].lat);
        index = status[index].parent;
    }
    fclose(file);
    return;
}