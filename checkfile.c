#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "solve.h"

int checkfile(char *file) {
  int fi;
  struct stat fileinfo;
  
  fi=stat(file, &fileinfo);

  if (fi<0) {
    printf("\n\n\n\nThe file:\n");
    printf(file);
    printf("\nseems to be invalid\n\n\n\n");
  }
  return fileinfo.st_size;
  
}
