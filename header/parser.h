#ifndef _PARSER_H_
#define _PARSER_H_

#include "../header/database.h"
#include<fcntl.h>


#define MAX_SIZE 100

//struct Database* db;

//enum delimitor { ], [, #};

int parse(struct Database*, const char*);

int file_name(struct Database*, char*, char*, FILE*);

char* tag_name(struct Database*, char*, char*);

int comment_line(char*);

void print(struct Database *db, const char *file);


#endif
