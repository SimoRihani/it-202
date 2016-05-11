#include "../header/parser.h"

/**
used in tag_name to expose '\n'
To debug in parse function
**/
void expose(char * str) {
  int i = 0;
  for(i = 0; i < (int) strlen(str); i++)
    if(str[i] == '\n')
      str[i] = '\0';
}


/**
 * says if it is a comment line or not
 * @Name : comment_line
 * @Param : char* - str
 * @Return : int - 1 if it is a comment line, else 0
 **/

int comment_line(char* str) {
  if(str[0] == '#') {
    //printf("comment line\n");
    return 1;
  }
  else
    return 0;
}

/**
 * returns the file name and inserts it in the database
 * @Name : file_name
 * @Param : char* - str
 * @Param : struct database* - db
 * @Param : char - a table of char that will contain the name of the file
 * @Return : int - 1 if the file doesn't have tag else 0
 **/

int file_name(struct Database* db, char* str, char *fileName, FILE * file) {
  int i = 1;
  char str2[100];
  int fileWithoutTag = 0;
  //printf("salut: %s\n", fileName);
  //char fileName[100];
  while(str[i] != '\n' && str[i] != ']') {
    //printf("%c\n", fileName[i-1]);
    str2[i-1] = str[i];
    //printf("%c\n", str2[i-1]);
    i++;
  }
  str2[i-1] = '\0';
  //printf("hello %s\n", str2);
  strcpy(fileName, str2);
  if (fgetc(file) == '[' || fgetc(file) == EOF) {
    //printf("fichier sans tag %s\n", fileName);
    add_new_file(db, fileName);
    fileWithoutTag = 1;
  } else {
    //printf("je suis le nom du fichier: %s\n", fileName);
    add_new_file(db, fileName);
    }
  return fileWithoutTag;
}

/**
 * returns the tag name and inserts it in the database
 * @Name : file_name
 * @Param : char* - str
 * @Param : struct database* - db
 * @Param : the file name relative to the tags
 * @Return : char* - the name of the tag
 **/

char* tag_name(struct Database* db, char* str, char* fileName) {
  //printf("je suis le tag: %s relatif au fichier : %s\n", str, fileName);
  expose(str);
  add_tag_to_file(db, fileName, str);
  return str;
}


/**
 * Parses a special file
 * @Name : parse
 * @Param : struct database* - db
 * @Param : const char* - filepath
 * @Return : int - 1 if sucess, else 0
 **/

int parse(struct Database* db, const char* filepath) {

  FILE * file;
  char str[100];
  char* tagName;
  int isComment;
  int tagAll = 0;

  int r;
  char * c;
  char fileName[100];
  file = fopen(filepath, "r");
  if (file == NULL)
    {
      perror("Cannot open input file\n");
      exit(-1);
    }

  while((c = fgets(str, MAX_SIZE, file)) != NULL && str[0] != '[') {
    isComment = comment_line(str);
    //if (str[0] != '#' && str[0] != '['){
    if (!isComment) {
      tagAll = 1;
      //printf("tag appliqué à tous les fichiers: %s\n", str);
      expose(str);
      add_new_g_tag(db, str);
        //add_tag_to_all_files(db, str);
    }

  }
  if(c != NULL)

    fseek(file, - strlen(str) , SEEK_CUR);

  if(tagAll) {

    while(fgets(str, MAX_SIZE, file)!= NULL) {
      isComment = comment_line(str);
      if(!isComment) {
	if (str[0] == '[') {

	  r = file_name(db, str, fileName, file);
	  fseek(file, -2, SEEK_CUR); // why ? \n[ ==> fseek -1
	}
	if (r == 0) {
	  //si le fichier a des tags on parcourt ce qui vient après pour connaitre les tags qui y sont relatifs
	  while((c = fgets(str, MAX_SIZE, file)) != NULL && str[0] != '[') {
	    isComment = comment_line(str);
	    if(!isComment)
	      tagName = tag_name(db, str, fileName);
	  }
	  if(c != NULL)
	    fseek(file, -strlen(str), SEEK_CUR);
	}

      }
    }
  }
  else
    {
      rewind(file);
      while(fgets(str, MAX_SIZE, file)!= NULL) {

	isComment = comment_line(str);
	if(!isComment) {
	  if (str[0] == '[') {
	    r = file_name(db, str, fileName, file);
	    fseek(file, -2, SEEK_CUR);
	  }
	  if (r == 0) {
	    //si le fichier a des tags on parcourt ce qui vient après pour connaitre les tags qui y sont relatifs
	    while((c = fgets(str, MAX_SIZE, file)) != NULL && str[0] != '[') {

	      isComment = comment_line(str);
	      if(!isComment)
		    tagName = tag_name(db, str, fileName);
	    }

	    if(c != NULL) {

	      fseek(file, - strlen(str) , SEEK_CUR);


	    }
	  }
	}
      }

    }

  (void) tagName;
  update_g_tags(db);

  clear_g_tags(db);

  return 0;

}

void print(struct Database *db, const char *file) {
  FILE* fp = NULL;
  struct Tag *t;
  struct File *f;
  struct File_tag *ft;

  if ( (fp = fopen(file, "w")) == NULL ) {
        perror("can't open: ");
        exit(-1);
    }

  for(t=db->tags; t != NULL; t=(struct Tag*)(t->hh.next)) {
    if(is_this_tag_general(db, t->t_name))
      fprintf(fp, "%s\n", t->t_name);
  }
  for(f=db->files; f != NULL; f=(struct File*)(f->hh.next)) {
    fprintf(fp, "[%s]\n", f->f_name);
    for(ft=f->f_tags; ft != NULL; ft=(struct File_tag*)(ft->hh.next))
      if(!is_this_tag_general(db, ft->f_t_name))
        fprintf(fp, "%s\n", ft->f_t_name);
  }
      fclose(fp);
}

/*int main() {
  struct Database* db;
  const char* file = "fichier.txt";
  int r = parse(db, file);
  return 0;

}*/
