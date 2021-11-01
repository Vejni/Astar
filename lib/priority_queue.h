// Priority Queue Nodes
typedef struct PqElem{
    unsigned long index;
    double cost;
    struct PqElem *next;
}PqElem;

PqElem *init_pq(unsigned long index){
    PqElem *head = (PqElem *)malloc(sizeof(PqElem));
    if(head == NULL){
        exit; 
    };
    head->next = NULL;
    head->cost = 0;
}

unsigned long extract_min(PqElem *pq){
    if(pq != NULL){
        PqElem *first = pq;
        unsigned long index = first->index;
        pq = pq->next;
        free(first);
        return index;
    }
    else{
        return(-1);
    }
};

PqElem *find_parent_elem(PqElem *head, double cost){
    if(cost < head->cost){
        return NULL;
    }
    PqElem *parent = (PqElem*) malloc(sizeof(PqElem));
    if(parent == NULL){
        exit;
    }
    short int end_search = 0; // Condition to stop the search
    parent->next = head;

    while(!end_search){
        parent = parent->next;
        if(parent->next==NULL){
            end_search = 1;
        }
        else if(cost < parent->next->cost){
            end_search = 1;
        }
    }  
    return parent;
};

void add_with_priority(PqElem *head, unsigned long node_index, double cost){
    // Instanciate new pq element and set its fields
    PqElem *new_pq_elem = (PqElem *)malloc(sizeof(PqElem));
    if(new_pq_elem == NULL){
        return; //should I use exit??
    };
    new_pq_elem->index = node_index;
    new_pq_elem->cost = cost;

    // Finds its place
    PqElem *parent = find_parent_elem(head, new_pq_elem->cost);
    // find_parent_elem returns NULL if new_pq_elem is to be the head
    if(parent == NULL){
        new_pq_elem->next = head;
        head = new_pq_elem;
    }
    else{
        new_pq_elem->next = parent->next;
        parent->next = new_pq_elem;
    }
};

void increase_priority(PqElem *head, unsigned long index);