#include 'map.h'

// Priority Queue Nodes
typedef struct{
    unsigned long index;
    Pq *next;
}Pq;

unsigned long extract_min(Pq *pq){
    Pq first = *pq;
    unsigned long index = first.index;

    *pq = 
    return index;
}

typedef char Queue;
enum whichQueue {NONE, OPEN, CLOSED};

typedef struct {
double g, h;
unsigned long parent;
Queue whq;
} AStarStatus;

double heuristic_dist(unsigned long index, node *nodes, AStarStatus *astar_status, )