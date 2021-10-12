# include <stdio.h>
# include <stdlib.h>
# include <string.h>

const char delim[] = "|";

void process_line(char * line){
  // Tokenize with delimiter |
  char *tok = line;
  char *end = line;
  while ((tok = strsep(&end, delim)) != NULL){
    printf("%s\n", tok);
  }
  printf("---------------\n");
}

void create_map(char * path){
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(path ,"r");
  if (fp == NULL){
    printf("Cannot open file.\n");
    exit(1);
  }

  // Read line by line
  while ((read = getline(&line, &len, fp)) != -1) {
    if (line[0] == '#') continue;  // skip lines starting with #
    process_line(line);
  }

  fclose(fp);
  if (line) free(line);
}
