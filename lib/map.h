# include <stdio.h>
# include <stdlib.h>

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

  while ((read = getline(&line, &len, fp)) != -1) {
    printf("%s", line);
  }

  fclose(fp);
  if (line) free(line);
}
