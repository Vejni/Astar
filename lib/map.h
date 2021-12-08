# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <limits.h>

# ifndef MAP_H
# define MAP_H
# define N_NODES 23895681
# define ALL_STEP 2
# define MAX_VALENCE 10

const char delim[] = "|";

typedef struct{
  unsigned long id;  // Node identification
  char *name;
  double lat, lon;  // Node position
  unsigned short nsucc;  // Number of node successors; i. e. length of successors
  unsigned short nsucc_all;  // Number of successors allocated
  unsigned long *successors;  // Vector of successor indeces not ids
}node;


bool starts_with(const char *a, const char *b){
  // String comparison
  if(strncmp(a, b, strlen(b)) == 0) return 1;
  else return 0;
}

unsigned long nodesearch(node * nodes, unsigned long id, int no_nodes){
  // Standard binary search
  unsigned long first = 0;
  unsigned long last = no_nodes;
  unsigned long middle = (first+last)/2;
  while (first <= last) {
    if (nodes[middle].id < id) first = middle + 1;
    else if (nodes[middle].id > id) last = middle - 1;
    else return middle;
    middle = (first + last)/2;
  }
  return -1;
}

void allocate_succ(node * nodes, unsigned long pos){
  if (nodes[pos].nsucc_all == 0){
    // No memory allocated yet
    if ((nodes[pos].successors = (unsigned long *) malloc(ALL_STEP * sizeof(unsigned long))) == NULL)
      printf("Cannot allocate memory for successors \n");
    nodes[pos].nsucc_all = ALL_STEP;
  } else if (nodes[pos].nsucc_all == nodes[pos].nsucc){
    // Used all allocated memory
    if ((nodes[pos].successors = (unsigned long *) realloc(nodes[pos].successors, (nodes[pos].nsucc_all + ALL_STEP) * sizeof(unsigned long))) == NULL)
      printf("Cannot REallocate memory for successors \n");
    nodes[pos].nsucc_all = nodes[pos].nsucc_all + ALL_STEP;
  }
}

void set_succ(node * nodes, unsigned long prev_pos, unsigned long curr_pos){
  // First check if edge would be repeated
  for(int i = 0; i < nodes[prev_pos].nsucc; i++){
    if((nodes[prev_pos].successors)[i] == curr_pos) return;
  }

  // Otherwise set position and increment successors
  (nodes[prev_pos].successors)[nodes[prev_pos].nsucc] = curr_pos;
  (nodes[prev_pos].nsucc)++;
}

void process_way(char * line, node * nodes, int no_nodes /* can remove last argument later*/, int i){
  // Variables to set ways
  bool oneway = 0;
  unsigned long prev, curr;
  unsigned long prev_pos, curr_pos;

  // Tokenize with delimiter |
  char *end = line;
  char *tok = strsep(&end, delim);  // we can skip the first field
  int counter = 0;
  while ((tok = strsep(&end, delim)) != NULL){
    if (counter == 6) {
      if (strcmp(tok, "oneway") == 0) oneway = 1;
    } else if (counter == 8){
      // Find Current node
      prev = strtoul(tok, &tok, 10);
      if ((prev_pos = nodesearch(nodes, prev, no_nodes)) == -1) continue; //printf("Node id not found\n");
    } else if (counter > 8){
      // Find Current node
      curr = strtoul(tok, &tok, 10);
      if ((curr_pos = nodesearch(nodes, curr, no_nodes)) == -1) continue; //printf("Node id not found\n");
      // Set up successor
      allocate_succ(nodes, prev_pos);
      set_succ(nodes, prev_pos, curr_pos);

      // Process two-way street
      if (oneway == 0){
        // Set up successor
        allocate_succ(nodes, curr_pos);
        set_succ(nodes, curr_pos, prev_pos);
      }

      // Set up for next iteration
      prev = curr;
      prev_pos = curr_pos;
    }
    counter++;
  }
}

void process_node(char * line, node * nodes, int i){
  // Tokenize with delimiter |
  char *end = line;
  char *tok = strsep(&end, delim);  // we can skip the first field
  int counter = 0;
  while ((tok = strsep(&end, delim)) != NULL){
    switch (counter){
      case 0:
        nodes[i].id = strtoul(tok, &tok, 10);
      case 1:
        // Most names are empty. Do we really need this?
        if ((nodes[i].name = (char *) malloc(sizeof(char) * (strlen(tok) + 1))) != NULL) strcpy(nodes[i].name, tok);
      case 8:
        nodes[i].lat = atof(tok);
      case 9:
        nodes[i].lon = atof(tok);
    }
    nodes[i].nsucc = 0;
    nodes[i].nsucc_all = 0;
    nodes[i].successors = NULL;
    counter++;
  }
}

void print_node(node * nodes, int i){
  printf("Node id of node %d: %ld\n", i, nodes[i].id);
  printf("Name of node %d: %s\n", i, nodes[i].name);
  printf("Latitude of node %d: %f\n", i, nodes[i].lat);
  printf("Longitude of node %d: %f\n", i, nodes[i].lon);
  printf("No. of successors of node %d: %d\n", i, nodes[i].nsucc);

  // Print successors
  for (int j = 0; j < nodes[i].nsucc; j++)
    printf("Successor %d of node %d is %ld\n", j, i, nodes[nodes[i].successors[j]].id);
}

void print_nodes(node * nodes, int no_nodes){
  for(int i = 0;i < no_nodes; i++){
    print_node(nodes, i);
    printf("\n");
  }
}

void print_valences(node * nodes){
  int valences[MAX_VALENCE] = {0};
  int index, i;
  for(i = 0; i < N_NODES; i++){
    if((index = nodes[i].nsucc) > MAX_VALENCE){
      printf("\nValence too large: %d\n\n", index);
      print_node(nodes, i);
    } else valences[index]++;
  }

  for(i = 0; i < MAX_VALENCE; i++){
    printf("Valence %d has %d nodes\n", i, valences[i]);
  }
}

node * create_map(char * path){
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  if ((fp = fopen(path ,"r")) == NULL){
    printf("Cannot open file.\n");
    exit(1);
  }

  // Create nodes array
  node * nodes = (node *) malloc(N_NODES * sizeof(node));
  if (nodes == NULL) exit(1);
  int no_nodes = 0;
  int no_ways = 0;

  // Read line by line
  printf("Reading Map\n");
  while ((read = getline(&line, &len, fp)) != -1) {
    if (starts_with(line, "node")) {
      process_node(line, nodes, no_nodes);
      no_nodes++;
    }
    else if (starts_with(line, "way")) {
      process_way(line, nodes, no_nodes, no_ways);
      no_ways++;
    }
    else if (starts_with(line, "#")) continue;  //skip lines starting with #
    else break;  // relations are last, we can stop reading
  }

  // clean up
  fclose(fp);
  if (line) free(line);

  return nodes;
}

#endif // MAP_H
