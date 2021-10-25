# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <limits.h>

# define N_NODES 23895681

const char delim[] = "|";

typedef struct{
  unsigned long id;  // Node identification
  char *name;
  double lat, lon;  // Node position
  unsigned short nsucc;  // Number of node successors; i. e. length of successors
  unsigned long *successors;  // Vector of successor indeces not ids
}node;


bool starts_with(const char *a, const char *b){
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   else return 0;
}

unsigned long nodesearch(node * nodes, unsigned long id, int no_nodes){
  unsigned long i;
  for (i = 0; i < no_nodes; i++){
    if (nodes[i].id == id) return i;
  }
  return -1;
}

void allocate_succ(node * nodes, unsigned long pos){
  if (nodes[pos].nsucc == 0){
    if ((nodes[pos].successors = (unsigned long *) malloc(sizeof(unsigned long))) == NULL)
      printf("Cannot allocate memory for successors \n");
  } else {
    if ((nodes[pos].successors = (unsigned long *) realloc(nodes[pos].successors, (nodes[pos].nsucc + 1) * sizeof(unsigned long))) == NULL)
      printf("Cannot REallocate memory for successors \n");
  }
}

void set_succ(node * nodes, unsigned long prev_pos, unsigned long curr_pos){
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
      if (strcmp(tok, "oneway") != 0) {oneway = 1;}
    } else if (counter == 8){
      // Find Current node
      prev = strtoul(tok, &tok, 10);
      if ((prev_pos = nodesearch(nodes, prev, no_nodes)) == -1) {printf("Node id not found\n"); return;}
    } else if (counter > 8){
      // Find Current node
      curr = strtoul(tok, &tok, 10);
      if ((curr_pos = nodesearch(nodes, curr, no_nodes)) == -1) {printf("Node id not found\n"); return;}

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
  // If Counter is 9, we have only one node in the way, free memory
  if (counter == 9){
    printf("Encountered way with <2 nodes, clearing \n");
    free(nodes[prev_pos].successors);
    nodes[prev_pos].successors = NULL;
  }

  // Logging
  if (i % 100 == 0){
    printf("Read %d number of ways\n", i);
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
        if ((nodes[i].name = (char *) malloc(sizeof(char) * (strlen(tok) + 1))) != NULL) strcpy(nodes[i].name, tok);
      case 8:
        nodes[i].lat = atof(tok);
      case 9:
        nodes[i].lon = atof(tok);
    }
    nodes[i].nsucc = 0;
    nodes[i].successors = NULL;
    counter++;
  }
  // Logging
  if (i % 1000000 == 0){
    printf("Read %d number of nodes\n", i);
  }
}

void print_nodes(node * nodes, int no_nodes){
  for(int i = 0;i < no_nodes; i++){
    printf("Node id of node %d: %ld\n", i, nodes[i].id);
    printf("Name of node %d: %s\n", i, nodes[i].name);
    printf("Latitude of node %d: %f\n", i, nodes[i].lat);
    printf("Longitude of node %d: %f\n", i, nodes[i].lon);
    printf("No. of successors of node %d: %d\n", i, nodes[i].nsucc);

    // Print successors
    for (int j = 0; j < nodes[i].nsucc; j++)
      printf("Successor %d of node %d is %ld\n", j, i, nodes[nodes[i].successors[j]].id);
    printf("\n");
  }
}

void create_map(char * path){
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
    //if(no_ways > 3000) break;
  }

  // clean up
  fclose(fp);
  if (line) free(line);

  print_nodes(nodes, 100);
}
