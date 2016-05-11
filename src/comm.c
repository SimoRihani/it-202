#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOGFILE "tagfs.log"
FILE* mylog;
#define LOG(args...) do{ fprintf(mylog, args); fflush(mylog); } while(0)

void comm_getTags(const char *path, char ***tagsbuf){
  int nbr_tag = 0;
  char *tmp, *tmp2;
  char *indic;
  char *c;
  int size;
  int i;

  asprintf(&tmp, "%s", path);
  tmp2 = tmp;

  for(c = tmp; *c; ++c){
    if(*c == '/'){
      ++nbr_tag;
    }
  }

  if (nbr_tag == 1 && tmp[1] == 0){
    *tagsbuf = NULL;
    free(tmp);
    return;
  }

  indic = tmp;
  *tagsbuf = malloc(sizeof(char*) * (nbr_tag + 1));

  for(i = 0 ; i < nbr_tag ; ++i){
    size = 0;
    for(tmp = ++indic; *indic != '/' && *indic != '\0';++indic){
      ++size;
    }
	(*tagsbuf)[i] = malloc(sizeof(char) * (size + 1));

    *indic = '\0';
    strcpy((*tagsbuf)[i], tmp);
  }
  (*tagsbuf)[nbr_tag] = NULL;
  free(tmp2);
}


void comm_freeTags(char **tagbuf){
  int index = 0;
  while(tagbuf[index] != NULL){
    free(tagbuf[index]);
    ++index;
  }
  free(tagbuf);
}

int comm_isSingleTag(char *tagbuf){
  return (strchr(tagbuf,'/') == NULL);
}
