void ExitError(const char *miss, int errcode) {
  fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}

void write_binary(char *argv, node * nodes){
  FILE *fin;
  unsigned long nnodes = 23895681UL;
  char name[257];
  int i;

  /*if((nodes = (node *) malloc(nnodes* sizeof(node))) == NULL)
    ExitError("when allocating memory for the nodes vector", 5);*/

  /* Computing the total number of successors */
  unsigned long ntotnsucc = 0UL;
  for(i=0; i < nnodes; i++) ntotnsucc += nodes[i].nsucc;

  /* Setting the name of the binary file */
  strcpy(name, argv); strcpy(strrchr(name, '.'), ".bin");
  if ((fin = fopen (name, "wb")) == NULL)
    ExitError("the output binary data file cannot be opened", 31);

  /* Global data −−− header */
  if( fwrite(&nnodes, sizeof(unsigned long), 1, fin) + fwrite(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 )
    ExitError("when initializing the output binary data file", 32);

  /* Writing all nodes */
  if( fwrite(nodes, sizeof(node), nnodes, fin) != nnodes )
    ExitError("when writing nodes to the output binary data file", 32);

  /* Writing sucessors in blocks */
  for(i=0; i < nnodes; i++) if(nodes[i].nsucc) {
    if( fwrite(nodes[i].successors, sizeof(unsigned long), nodes[i].nsucc, fin) !=nodes[i].nsucc )
      ExitError("when writing edges to the output binary data file", 32);
  }
  fclose(fin);
}


node * read_binary(char *argv){
  FILE *fin;
  unsigned long nnodes, ntotnsucc;
  unsigned long * allsuccessors;
  node *nodes;
  int i;

  if ((fin = fopen (argv, "r")) == NULL)
    ExitError("the data file does not exist or cannot be opened", 11);

  /* Global data −−− header */
  if( fread(&nnodes, sizeof(unsigned long), 1, fin) + fread(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 )
    ExitError("when reading the header of the binary data file", 12);

  /* getting memory for all data */
  if((nodes = (node *) malloc(nnodes* sizeof(node))) == NULL)
    ExitError("when allocating memory for the nodes vector", 13);
  if((allsuccessors = (unsigned long *) malloc(ntotnsucc* sizeof(unsigned long))) == NULL)
    ExitError("when allocating memory for the edges vector", 15);

  /* Reading all data from file */
  if(fread(nodes, sizeof(node), nnodes, fin) != nnodes )
    ExitError("when reading nodes from the binary data file", 17);
  if(fread(allsuccessors, sizeof(unsigned long), ntotnsucc, fin) != ntotnsucc)
    ExitError("when reading sucessors from the binary data file", 18);

  fclose(fin);

  /* Setting pointers to successors */
  for(i=0; i < nnodes; i++) if(nodes[i].nsucc) {
    nodes[i].successors = allsuccessors; allsuccessors += nodes[i].nsucc;
  }

  return nodes;
}
