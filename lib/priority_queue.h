#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
// Priority Queue Nodes
typedef struct PqElem{
    unsigned long index;
    double cost;
    struct PqElem *next;
}PqElem;

PqElem *init_pq(unsigned long index, double cost){
    PqElem *head = (PqElem *)malloc(sizeof(PqElem));
    if(head==NULL){
        exit(66); 
    };
    head->index = index;
    head->next = NULL;
    head->cost = cost;
    return head;
}

unsigned long extract_min(PqElem **head){
    if(*head != NULL){
        PqElem *first = *head;
        unsigned long index = first->index;
        *head = (*head)->next;
        free(first);
        return index;
    }
    else{
        return(-1);
    }
};

PqElem *find_parent_elem(PqElem *head, double cost){
    if(head==NULL || cost < head->cost){
        return NULL;
    }
    register PqElem *parent = head;
    while(parent->next!=NULL && cost > parent->next->cost){
        parent = parent->next;
    }  
    return parent;
};

PqElem* add_with_priority(PqElem *head, unsigned long node_index, double cost){
    // Instanciate new pq element and set its fields
    PqElem *new_pq_elem = (PqElem *)malloc(sizeof(PqElem));
    if(new_pq_elem==NULL){
        return head;
    }
    new_pq_elem->index = node_index;
    new_pq_elem->cost = cost;
    
    PqElem *parent = find_parent_elem(head, new_pq_elem->cost);
    // find_parent_elem returns NULL if new_pq_elem is to be the head
    if(parent==NULL){
        new_pq_elem->next = head;
        head = new_pq_elem;
    }
    else{
        new_pq_elem->next = parent->next;
        parent->next = new_pq_elem;
    }
    return head;
};

PqElem* increase_priority(PqElem *head, unsigned long index, double cost){
    // If the elem is already the head, update and return
    if(head->index==index){
        head->cost = cost;
        return head;
    }  
    // Otherwise, find who should be the parent
    PqElem *future_parent = find_parent_elem(head, cost);

    // Then, it looks for current parent
    register PqElem *current_parent = future_parent==NULL? head:future_parent;
    while(current_parent->next!=NULL && current_parent->next->index!=index){
        current_parent = current_parent->next;   
    }

    // We must check that the current_parent is actually the right one
    if(current_parent->next == NULL) return head; // The elem is not in the queue
    
    PqElem *aux = current_parent->next;
    aux->cost = cost;
    current_parent->next = aux->next;
    // If future_parent is NULL, the searched_elem is to be the head
    if(future_parent == NULL){
        aux->next = head;
        head = aux;
    }
    // Otherwise, the searched_elem is place right after future_parent
    else{ 
        aux->next = future_parent->next;
        future_parent->next = aux;
    }
   
    return head;
};


void show_queue(PqElem* head){
    for(PqElem *elem = head; elem!=NULL; elem=elem->next){
        printf("Node %li, distance %f\n", elem->index, elem->cost);
    }
};

#endif
