# include "map.h"
# include "math.h"
# define PI 3.14159265358979323846

int suma(int a);

typedef char Queue;
enum whichQueue {NONE, OPEN, CLOSED};

typedef struct {
double g, h;
unsigned long parent;
Queue whq;
} AStarStatus;

AStarStatus *init_astarstatus(unsigned long n_nodes){
    AStarStatus *status = (AStarStatus*) malloc(n_nodes*sizeof(AStarStatus));
    if(status == NULL){
        exit;
    }
    for(unsigned long index = 0; index < n_nodes; index++){
        status[index].whq = NONE;
    }
    return status;
}

double haversine_dist(node node1, node node2){
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

double heuristic(node current_node, node goal){
    return haversine_dist(current_node, goal);    
};
