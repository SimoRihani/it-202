#include "../header/database.h"


/**
 * Creates a database without initializing it.
 * @Name  : db_create
 * @Return: struct Database * - Database
 **/
 struct Database * db_create() {
   struct Database *db = malloc(sizeof(*db));
   return db;
 }

/**
 * Initializes a database.
 * @Name  : db_init
 * @Param : struct Database * - Database to initialize
 **/
 void db_init(struct Database *db) {
   db->files = NULL;
   db->tags = NULL;
   db->g_tags = NULL;
   db->g_files = NULL;
   db->t_size = 0;
   db->f_size = 0;
   db->gt_size = 0;
   db->gf_size = 0;
 }

 /**
  * Adds a new general tag to the data base, that'll be applied to all existing and new files
  * @Name  : add_new_g_tag
  * @Param : struct Database * - the database
  * @Param : char * - the tag name
  **/
 void add_new_g_tag(struct Database * db, const char * tag) {
   if(!exist_g_tag(db, tag)) {
     if(!exist_tag(db, tag))
      add_new_tag(db, tag);
     struct G_tag * gt;
     if ((gt = (struct G_tag*)malloc(sizeof(struct G_tag))) == NULL) exit(-1);
     strcpy(gt->name, tag);
     LL_PREPEND(db->g_tags, gt);
     db->gt_size++;
     add_tag_to_all_files(db, tag);
   }
 }

 /**
  * Does this general tag exist ?
  * @Param : struct Database * - the database
  * @Param : char * - the tag name
  * @Return: int - 1 if g_tag exists, else 0
  **/
 int exist_g_tag(struct Database * db, const char * tag) {
   struct G_tag * elt;
   LL_FOREACH(db->g_tags, elt)
     if(!strcmp(elt->name, tag))
      return 1;
  return 0;
 }

 int exist_g_tag_bis(struct G_tag * g, const char * tag) {
   struct G_tag * elt;
   LL_FOREACH(g, elt)
     if(!strcmp(elt->name, tag))
      return 1;
  return 0;
 }

 /**
  * Updates a file by adding to it g_tags
  * @Name  : update_tag_g_files
  * @Param : struct Database * - the database
  * @Param : char * - the tag name
  **/
void update_file_g_tags(struct Database * db, const char * file) {
  struct G_tag * elt;
  LL_FOREACH(db->g_tags, elt) {
    add_tag_to_file(db, file, elt->name);
  }
}

/**
 * Updates all files by adding to them g_tags
 * @Name  : update_g_tags
 * @Param : struct Database * - the database
 **/
 void update_g_tags(struct Database * db) {
   struct G_tag * elt;
   LL_FOREACH(db->g_tags, elt) {
     add_tag_to_all_files(db, elt->name);
   }
 }

 /**
  * Deletes a g_tag from the database and frees it from the memory
  * @Name  : clear_g_tag
  * @Param : struct Database * - the database
  * @Param : char * - the tag name
  **/
 void clear_g_tag(struct Database * db, const char * tag) {
   struct G_tag * elt;
   LL_FOREACH(db->g_tags, elt)
     if(!strcmp(elt->name, tag)) {
       LL_DELETE(db->g_tags, elt);
       free(elt);
     }
 }

 /**
  * Deletes all g_tags from the database and frees them from the memory
  * @Name  : clear_g_tag
  * @Param : struct Database * - the database
  * @Param : char * - the tag name
  **/
 void clear_g_tags(struct Database * db) {
   struct G_tag * elt, * tmp;
   LL_FOREACH_SAFE(db->g_tags, elt, tmp) {
       LL_DELETE(db->g_tags, elt);
       free(elt);
  }
 }

 /**
  * Debugs g_files.
  * @Name  : debug
  * @Param : struct Database * - Database to debug
  **/
 void debug_g_files(struct Database * db) {
   struct G_file * elt;
   LL_FOREACH(db->g_files, elt) {
     printf("%s,", elt->name);
   }
   printf("\n");
 }

 /**
  * Adds a new general file to the data base, that'll be applied to all existing and new tags
  * @Name  : add_new_g_file
  * @Param : struct Database * - the database
  * @Param : char * - the file name
  **/
 void add_new_g_file(struct Database * db, const char * file) {
   if(!exist_file(db, file))
    add_new_file(db, file);
   struct G_file * gf;
   if ((gf = (struct G_file*)malloc(sizeof(struct G_file))) == NULL) exit(-1);
   strcpy(gf->name, file);
   LL_PREPEND(db->g_files, gf);
   db->gf_size++;
   add_file_to_all_tags(db, file);
 }

 /**
  * Does this general file exist ?
  * @Name  : exist_g_file
  * @Param : struct Database * - the database
  * @Param : char * - the file name
  **/
 int exist_g_file(struct Database * db, const char * file) {
   struct G_file * elt;
   LL_FOREACH(db->g_files, elt)
     if(!strcmp(elt->name, file))
      return 1;
  return 0;
 }

 int exist_g_file_bis(struct G_file * g, const char * file) {
   struct G_file * elt;
   LL_FOREACH(g, elt)
     if(!strcmp(elt->name, file))
      return 1;
  return 0;
 }

 /**
  * Updates a tag by adding to it g_files
  * @Name  : update_tag_g_files
  * @Param : struct Database * - the database
  * @Param : char * - the tag name
  **/
void update_tag_g_files(struct Database * db, const char * tag) {
  struct G_file * elt;
  LL_FOREACH(db->g_files, elt) {
    add_file_to_tag(db, elt->name, tag);
  }
}

/**
 * Updates all tags by adding to them g_files
 * @Name  : update_g_files
 * @Param : struct Database * - the database
 **/
 void update_g_files(struct Database * db) {
   struct G_file * elt;
   LL_FOREACH(db->g_files, elt) {
     add_file_to_all_tags(db, elt->name);
   }
 }

 /**
  * Deletes a g_file from the database and frees it from the memory
  * @Name  : clear_g_file
  * @Param : struct Database * - the database
  * @Param : char * - the file name
  **/
 void clear_g_file(struct Database * db, const char * file) {
   struct G_file * elt;
   LL_FOREACH(db->g_files, elt)
     if(!strcmp(elt->name, file)) {
       LL_DELETE(db->g_files, elt);
       free(elt);
     }
 }

 /**
  * Deletes all g_files from the database and frees them from the memory
  * @Name  : clear_g_file
  * @Param : struct Database * - the database
  **/
 void clear_g_files(struct Database * db) {
   struct G_file * elt, * tmp;
   LL_FOREACH_SAFE(db->g_files, elt, tmp) {
       LL_DELETE(db->g_files, elt);
       free(elt);
     }
 }

 struct G_tag * get_g_tags(struct Database * db) {
   return db->g_tags;
 }

 /**
  * Debugs g_tags of a given database.
  * @Name  : debug_g_tags
  * @Param : struct Database * - Database
  **/
 void debug_g_tags(struct Database * db) {
   struct G_tag * elt;
   LL_FOREACH(db->g_tags, elt) {
     printf("%s,", elt->name);
   }
   printf("\n");
 }

/**
 * Adds a new tag to the data base, if it doesn't exist already
 * @Name  : add_new_tag
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
void add_new_tag(struct Database * db, const char * tag) {
  struct Tag *t;
  HASH_FIND_STR(db->tags, tag, t);
  if(t == NULL) {
    struct Tag * new_t = malloc(sizeof(*new_t));
    if (new_t == NULL) {
        exit(-1);
    }
    new_t->t_files = NULL;
    new_t->t_files_size = 0;
    new_t->t_id = db->t_size;
    strcpy(new_t->t_name, tag);
    HASH_ADD_STR(db->tags, t_name, new_t);
    db->t_size++;
    update_tag_g_files(db, tag);
  }
}

/**
 * Adds a new file to the data base, if it doesn't exist already
 * @Name  : add_new_file
 * @Param : struct Database * - the database
 * @Return: char * - the file name
 **/
void add_new_file(struct Database * db, const char * file) {
    struct File *f;
    HASH_FIND_STR(db->files, file, f);
    if(f == NULL) {
      struct File * new_f = malloc(sizeof(*new_f));
      if (new_f == NULL) {
          exit(-1);
      }
      new_f->f_tags = NULL;
      new_f->f_tags_size = 0;
      new_f->f_id = db->f_size;
      strcpy(new_f->f_name, file);
      HASH_ADD_STR(db->files, f_name, new_f);
      db->f_size++;
      update_file_g_tags(db, file);
  }
}

/**
 * Renames a tag in the database
 * @Name  : rename_tag
 * @Param : struct Database * - the database
 * @Param : char * - the tag to rename
 * @Param : char * - the new tag name
 **/
void rename_tag(struct Database *db, const char * old, const char * new) {
  struct Tag *t;
  struct File *f;
  struct Tag_file *tf;
  struct File_tag *ft;
  struct G_tag *gt;
  HASH_FIND_STR(db->tags, old, t);
  if(t != NULL) {
    strcpy(t->t_name, new);
    for(tf=t->t_files; tf != NULL; tf=(struct Tag_file*)(tf->hh.next)){
      HASH_FIND_STR(db->files, tf->t_f_name, f);// assert f != NULL
      HASH_FIND_STR(f->f_tags, old, ft); // assert ft != NULL
      strcpy(ft->f_t_name, new);
    }
    for(gt=db->g_tags; gt != NULL; gt=(struct G_tag*)(gt->next)){
      if(!strcmp(gt->name, old))
        strcpy(gt->name, new);
    }
  }
}

/**
 * Renames a file in the database
 * @Name  : rename_file
 * @Param : struct Database * - the database
 * @Param : char * - the file to rename
 * @Param : char * - the new file name
 **/
void rename_file(struct Database *db, const char * old, const char * new) {
  struct Tag *t;
  struct File *f;
  struct Tag_file *tf;
  struct File_tag *ft;
  struct G_file *gf;
  HASH_FIND_STR(db->files, old, f);
  if(f != NULL) {
    strcpy(f->f_name, new);
    for(ft=f->f_tags; ft != NULL; ft=(struct File_tag*)(ft->hh.next)){
      HASH_FIND_STR(db->tags, ft->f_t_name, t);// assert t != NULL
      HASH_FIND_STR(t->t_files, old, tf); // assert tf != NULL
      strcpy(tf->t_f_name, new);
    }
    for(gf=db->g_files; gf != NULL; gf=(struct G_file*)(gf->next)){
      if(!strcmp(gf->name, old))
        strcpy(gf->name, new);
    }
  }
}

/**
 * Adds a new tag to an existing file, creates the tag if it doesn't exist
 * @Name  : add_tag_to_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 **/
void add_tag_to_file(struct Database * db, const char * file, const char * tag) {
  struct File *f;
  HASH_FIND_STR(db->files, file, f);
  if(f != NULL) {
    struct File_tag *ft;
    HASH_FIND_STR(f->f_tags, tag, ft);
    if (ft == NULL) {
      struct File_tag * new_f_t = (struct File_tag*)malloc(sizeof(*new_f_t));
      struct Tag_file *tf;
      if (new_f_t == NULL) {
          exit(-1);
      }
      strcpy(new_f_t->f_t_name, tag);
      HASH_ADD_STR(f->f_tags, f_t_name, new_f_t);
      new_f_t->f_t_id = f->f_tags_size;
      f->f_tags_size++;

      struct Tag *t;
      HASH_FIND_STR(db->tags, tag, t);
      if(t == NULL) {
        add_new_tag(db, tag);
      }
      HASH_FIND_STR(db->tags, tag, t); //if new tag, t was NULL //assert t != NULL
      HASH_FIND_STR(t->t_files, file, tf);
      if(tf == NULL) {
        add_file_to_tag(db, file, tag);
      }
    }
  }
}

/**
 * Adds a new file to an existing tag, creates the file if it doesn't exist
 * @Name  : add_file_to_tag
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 **/
void add_file_to_tag(struct Database * db, const char * file, const char * tag) {
  struct Tag *t;
  HASH_FIND_STR(db->tags, tag, t);
  if(t != NULL) {
    struct Tag_file *tf;
    HASH_FIND_STR(t->t_files, file, tf);
    if (tf == NULL) {
      struct Tag_file * new_t_f = (struct Tag_file*)malloc(sizeof(*new_t_f));
      struct File_tag *ft;
      if (new_t_f == NULL) {
          exit(-1);
      }
      strcpy(new_t_f->t_f_name, file);
      HASH_ADD_STR(t->t_files, t_f_name, new_t_f);
      new_t_f->t_f_id = t->t_files_size;
      t->t_files_size++;

      struct File *f;
      HASH_FIND_STR(db->files, file, f);
      if(f == NULL) {
        add_new_file(db, file);
      }
      HASH_FIND_STR(db->files, file, f);// assert f != NULL
      HASH_FIND_STR(f->f_tags, tag, ft);
      if(ft == NULL) {
        add_tag_to_file(db, file, tag);
      }
    }
  }
}

/**
 * Adds a tag to all files, creates the tag if it doesn't exist
 * @Name  : add_tag_to_all_files
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
void add_tag_to_all_files(struct Database * db, const char * tag) {
  struct File *file;
  for(file=db->files; file != NULL; file=(struct File*)(file->hh.next))
    add_tag_to_file(db, file->f_name, tag);
}

/**
 * Adds a file to all tags, creates the file if it doesn't exist
 * @Name  : add_file_to_all_tags
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 **/
void add_file_to_all_tags(struct Database * db, const char * file) {
  struct Tag *tag;
  for(tag=db->tags; tag != NULL; tag=(struct Tag*)(tag->hh.next))
    add_file_to_tag(db, file, tag->t_name);
}

/**
 * Is this tag general, to all files
 * @Name  : is_this_tag_general
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
int is_this_tag_general(struct Database * db, const char * tag) {
  struct Tag *t;
  HASH_FIND_STR(db->tags, tag, t);
  if(t != NULL) {
    return t->t_files_size == db->f_size;
  }
  return 0;
}

/**
 * Is this file general, to all tags
 * @Name  : is_this_file_general
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 **/
int is_this_file_general(struct Database * db, const char * file) {
  struct File *f;
  HASH_FIND_STR(db->files, file, f);
  if(f != NULL) {
    return f->f_tags_size == db->t_size;
  }
  return 0;
}

/**
 * Deletes a tag from a file in the database and frees it form the memory
 * @Name  : delete_tag_from_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 **/
void delete_tag_from_file(struct Database *db, const char * file , const char * tag) {
  struct File *f;
  HASH_FIND_STR(db->files, file, f);
  if(f != NULL) {
    struct File_tag *ft;
    HASH_FIND_STR(f->f_tags, tag, ft);
    if (ft != NULL) {
      HASH_DEL(f->f_tags, ft);
      f->f_tags_size--;
      struct Tag *t;
      struct Tag_file *tf;
      HASH_FIND_STR(db->tags, tag, t); // assert t != NULL
      if(t != NULL) {
        HASH_FIND_STR(t->t_files, file, tf);
        if(tf != NULL) {
          delete_file_from_tag(db, file, tag);
        }
      }
      if(ft) free(ft);
    }
  }
}


/**
 * Deletes a file from a tag in the database and frees it form the memory
 * @Name  : delete_file_from_tag
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 **/
void delete_file_from_tag(struct Database *db, const char * file , const char * tag) {
  struct Tag *t;
  HASH_FIND_STR(db->tags, tag, t);
  if(t != NULL) {
    struct Tag_file *tf;
    HASH_FIND_STR(t->t_files, file, tf);
    if (tf != NULL) {
      HASH_DEL(t->t_files, tf);
      t->t_files_size--;
      struct File *f;
      struct File_tag *ft;
      HASH_FIND_STR(db->files, file, f);// assert f != NULL
      HASH_FIND_STR(f->f_tags, tag, ft);
      if(ft != NULL) {
        delete_tag_from_file(db, file, tag);
      }
      if(tf) free(tf);
    }
  }
}

/**
 * Deletes a tag and its files in the database and frees it
   form the memory (uses delete_tag_from_file())
 * @Name  : delete_tag
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
void delete_tag(struct Database *db, const char *tag) {
  struct Tag *t, *tB;
  struct Tag_file *tf;
  HASH_FIND_STR(db->tags, tag, t);
  if(t != NULL) {
    for(tB=t; tB != NULL; tB=(struct Tag*)(tB->hh.next))
      tB->t_id--;

    for(tf=t->t_files; tf != NULL; tf=(struct Tag_file*)(tf->hh.next)){
      delete_file_from_tag(db, tf->t_f_name, t->t_name);
    }
  HASH_DEL(db->tags, t);
  if(t) free(t);
  db->t_size--;
  clear_g_tag(db, tag);
  }
}

/**
 * Deletes a file and its tags in the database and frees it
   form the memory (uses delete_file_from_tag())
 * @Name  : delete_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 **/
void delete_file(struct Database *db, const char *file) {
  struct File *f, *fB;
  struct File_tag *ft, *tmp;
  HASH_FIND_STR(db->files, file, f);
  if(f != NULL) {
    for(fB=f; fB != NULL; fB=(struct File*)(fB->hh.next))
      fB->f_id--;

    //for(ft=f->f_tags; ft != NULL; ft=(struct File_tag*)(ft->hh.next)){
    HASH_ITER(hh, f->f_tags, ft, tmp) {
      delete_tag_from_file(db, f->f_name, ft->f_t_name);
      //delete_file_from_tag(db, f->f_name, ft->f_t_name);
    }
  HASH_DEL(db->files, f);
  if(f) free(f);
  db->f_size--;
  clear_g_file(db, file);
  }
}

/**
 * Deletes all tags in the database and frees them
   form the memory (uses delete_tag())
 * @Name  : delete_all_tags
 * @Param : struct Database * - the database
 **/
void delete_all_tags(struct Database *db) {
  struct Tag *tag;
  for(tag=db->tags; tag != NULL; tag=(struct Tag*)(tag->hh.next))
    delete_tag(db, tag->t_name);
}

/**
 * Deletes all files in the database and frees them
   form the memory (uses delete_file())
 * @Name  : delete_all_files
 * @Param : struct Database * - the database
 **/
void delete_all_files(struct Database *db) {
  struct File *file;
  for(file=db->files; file != NULL; file=(struct File*)(file->hh.next))
    delete_file(db, file->f_name);
}

/**
 * Deletes all files and all tags in the database and frees them
   form the memory (uses delete_file() and delete_tag())
 * @Name  : delete_all
 * @Param : struct Database * - the database
 **/
void delete_all(struct Database *db) {
  struct File *file, *tmp;
  struct Tag *tag, *tmp2;
  //for(file=db->files; file != NULL; file=(struct File*)(file->hh.next))
  HASH_ITER(hh, db->files, file, tmp)
    delete_file(db, file->f_name);
  //for(tag=db->tags; tag != NULL; tag=(struct Tag*)(tag->hh.next))
  HASH_ITER(hh, db->tags, tag, tmp2)
    delete_tag(db, tag->t_name);
  clear_g_tags(db);
  clear_g_files(db);
  free(db);
}

/*
            TODO

int name_sort(void *a, void *b) {
    return strcmp(a->name,b->name);
}

int id_sort(void *a, void *b) {
    return (a->id - b->id);
}

void sort_by_name(db) {
    HASH_SORT(db->files, name_sort);
    HASH_SORT(db->tags, name_sort);
    HASH_SORT(db->files->f_tags, name_sort);
    HASH_SORT(db->tags->t_files, name_sort);
}

void sort_by_id() {
    HASH_SORT(db->files, id_sort);
    ..
}*/

/*void for_debug(char * str) {
  int i = 0;
  for(i = 0; i < 15 - strlen(str); i++)
    str += 'L';
}*/

/**
 * Gets a file.
 * @Name  : get_file
 * @Param : struct Database * - the database
 * @Param : char * - file
 * @Return: struct File * - the file or null of it doesn't exist
 **/
struct File * get_file(struct Database * db, const char * file) {
  struct File *f;
  HASH_FIND_STR(db->files, file, f);
  return f;
}

/**
 * Gets all files.
 * @Name  : get_files
 * @Param : struct Database * - the database
 * @Return: struct G_file - simple linked list of all files
 **/
struct G_file * get_files(struct Database * db) {
  struct G_file * cf = NULL;//Simple linked list of all files
  struct G_file *gf;
  struct File * file;
  for(file=db->files; file != NULL; file=(struct File*)(file->hh.next)) {
    if ((gf = (struct G_file*)malloc(sizeof(struct G_file))) == NULL) exit(-1);
    strcpy(gf->name, file->f_name);
    LL_PREPEND(cf, gf);
    printf("%s ", gf->name);
  }
  return cf;
}

/**
 * Gets a tag.
 * @Name  : get_tag
 * @Param : struct Database * - the database
 * @Param : char * - tag
 * @Return: struct Tag * - the tag or null if it doesn't exist
 **/
struct Tag * get_tag(struct Database * db, const char * tag) {
  struct Tag *t;
  HASH_FIND_STR(db->tags, tag, t);
  return t;
}

/**
 * Gets all tags.
 * @Name  : get_tags
 * @Param : struct Database * - the database
 * @Return: struct G_tag - simple linked list of all tags
 **/
struct G_tag * get_tags(struct Database * db) {
  struct G_tag * ct = NULL;//Simple linked list of all tags
  struct G_tag *gt;
  struct Tag * tag;
  for(tag=db->tags; tag != NULL; tag=(struct Tag*)(tag->hh.next)) {
    if ((gt = (struct G_tag*)malloc(sizeof(struct G_tag))) == NULL) exit(-1);
    strcpy(gt->name, tag->t_name);
    LL_PREPEND(ct, gt);
    //printf("%s ", gt->name);
  }
  return ct;
}

/**
 * Deletes a simple linked list of G_file from memory
 * @Name  : clear__list_files
 * @Param : struct G_file * - the list to delete
 **/
void clear_list_files(struct G_file * gf) {
  struct G_file * elt, * tmp;
  LL_FOREACH_SAFE(gf, elt, tmp) {
      LL_DELETE(gf, elt);
      free(elt);
 }
}

/**
 * Deletes a simple linked list of G_tag from memory
 * @Name  : clear_list_tags
 * @Param : struct G_tag * - the list to delete
 **/
void clear_list_tags(struct G_tag * gt) {
  struct G_tag * elt, * tmp;
  LL_FOREACH_SAFE(gt, elt, tmp) {
      LL_DELETE(gt, elt);
      free(elt);
  }
}

/**
 * Gets a file tag.
 * @Name  : get_file_tag
 * @Param : struct Database * - the database
 * @Param : char * - file
 * @Param : char * - tag
 * @Return: struct File_tag * - the file tag or null if it doesn't exist
 **/
struct File_tag * get_file_tag(struct Database * db, const char * file, const char * tag) {
  struct File *f = get_file(db, file);// assert f != NULL
  struct File_tag *ft = NULL;
  if(f != NULL)
    HASH_FIND_STR(f->f_tags, tag, ft);
  return ft;
}

/**
 * Gets a tag file.
 * @Name  : get_tag_file
 * @Param : struct Database * - the database
 * @Param : char * - file
 * @Param : char * - tag
 * @Return: struct Tag_file * - the tag file or null if it doesn't exist
 **/
struct Tag_file * get_tag_file(struct Database * db, const char * file, const char * tag) {
  struct Tag *t = get_tag(db, tag);
  struct Tag_file * tf = NULL;
  if(t != NULL)
    HASH_FIND_STR(t->t_files, file, tf);
  return tf;
}

/**
 * Does this file exist ?
 * @Name  : exist_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Return: int - 1 if the file exist, else 0
 **/
int exist_file(struct Database * db, const char * file) {
  struct File *f = get_file(db, file);
  return (f != NULL);
}

/**
 * Does this tag exist ?
 * @Name  : exist_tag
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 * @Return: int - 1 if the tag exist, else 0
 **/
int exist_tag(struct Database * db, const char * tag) {
  struct Tag *t = get_tag(db, tag);
  return (t != NULL);
}

/**
 * Is this tag belonging to this file ?
 * @Name  : is_tag_in_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 * @Return: int - 1 if the tag belongs to the file, else 0
 **/
int is_tag_in_file(struct Database * db, const char * file, const char * tag) {
  struct File_tag *ft = get_file_tag(db, file, tag);
  return (ft != NULL);
}

/**
 * Is this file belonging to this tag ?
 * @Name  : is_file_in_tag
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 * @Return: int - 1 if the file belongs to the tag, else 0
 **/
int is_file_in_tag(struct Database * db, const char * file, const char * tag) {
  struct Tag_file *tf = get_tag_file(db, file, tag);
  return (tf != NULL);
}

/**
 * What are the common files between the two tags given?
 * @Name  : common_files
 * @Param : struct Database * - the database
 * @Param : char * - the first tag name
 * @Param : char * - the second tag name
 * @Return: struct G_file - simple linked list of common files
 **/
struct G_file * common_files(struct Database * db, const char * tag1, const char * tag2) {
  struct G_file * cf = NULL;//Simple linked list of common files
  struct G_file * gf;
  struct Tag *t1, *t2;
  struct Tag_file *tf, *tf2;
  HASH_FIND_STR(db->tags, tag1, t1);
  HASH_FIND_STR(db->tags, tag2, t2);
  if(t1 != NULL && t2 != NULL) {
    for(tf=t1->t_files; tf != NULL; tf=(struct Tag_file*)(tf->hh.next)){
      HASH_FIND_STR(t2->t_files, tf->t_f_name, tf2);
      if(tf2 != NULL) {
        if ((gf = (struct G_file*)malloc(sizeof(struct G_file))) == NULL) exit(-1);
        strcpy(gf->name, tf2->t_f_name);
        LL_PREPEND(cf, gf);
        //printf("%s, ", tf2->t_f_name);
      }
    }
    return cf;
  }
  return cf;
}

/**
 * What are the common tags between the two files given?
 * @Name  : common_tags
 * @Param : struct Database * - the database
 * @Param : char * - the first file name
 * @Param : char * - the second file name
 * @Return: struct G_tag - simple linked list of common tags
 **/
struct G_tag * common_tags(struct Database * db, const char * file1, const char * file2) {
  struct G_tag * cf = NULL;//Simple linked list of common tags
  struct G_tag * gt;
  struct File *f1, *f2;
  struct File_tag *ft, *ft2;
  HASH_FIND_STR(db->files, file1, f1);
  HASH_FIND_STR(db->files, file2, f2);
  if(f1 != NULL && f2 != NULL) {
    for(ft=f1->f_tags; ft != NULL; ft=(struct File_tag*)(ft->hh.next)){
      HASH_FIND_STR(f2->f_tags, ft->f_t_name, ft2);
      if(ft2 != NULL) {
        if ((gt = (struct G_tag*)malloc(sizeof(struct G_tag))) == NULL) exit(-1);
        strcpy(gt->name, ft2->f_t_name);
        LL_PREPEND(cf, gt);
        //printf("%s, ", ft2->f_t_name);
      }
    }
    return cf;
  }
  return cf;
}

/**
 * What are the common files between the tag given files and the simple linked list of files given?
 * @Name  : intersect_files
 * @Param : struct Database * - the database
 * @Param : struct G_file - simple linked list files
 * @Param : char * - the tag name
 * @Return: struct G_file - simple linked list of common files
 **/
struct G_file * intersect_files(struct Database * db, struct G_file *toCompare, const char * tag) {
  struct G_file * cf = NULL;//Simple linked list of common files between tag given files and toCompare
  struct Tag *t;
  struct G_file * elt, *gf;
  HASH_FIND_STR(db->tags, tag, t);
  if(t != NULL) {
    LL_FOREACH(toCompare, elt) {
      if(is_file_in_tag(db, elt->name, tag)) {
      if ((gf = (struct G_file*)malloc(sizeof(struct G_file))) == NULL) exit(-1);
      strcpy(gf->name, elt->name);
      LL_PREPEND(cf, gf);
      //printf("%s ", gf->name);
      }
    }
    return cf;
  }
  return cf;
}

/**
 * What are the common tags between the file given tags and the simple linked list of tags given?
 * @Name  : intersect_tags
 * @Param : struct Database * - the database
 * @Param : struct G_tag - simple linked list tags
 * @Param : char * - the file name
 * @Return: struct G_tag - simple linked list of common tags
 **/
struct G_tag * intersect_tags(struct Database * db, struct G_tag *toCompare, const char * file) {
  struct G_tag * ct = NULL;//Simple linked list of common tags between file given tags and toCompare
  struct File *f;
  struct G_tag * elt, *gt;
  HASH_FIND_STR(db->files, file, f);
  if(f != NULL) {
    LL_FOREACH(toCompare, elt) {
      if(is_tag_in_file(db, file, elt->name)) {
      if ((gt = (struct G_tag*)malloc(sizeof(struct G_tag))) == NULL) exit(-1);
      strcpy(gt->name, elt->name);
      LL_PREPEND(ct, gt);
      //printf("%s ", gt->name);
      }
    }
    return ct;
  }
  return ct;
}

/**
 * What are the tags of the two files given?
 * @Name  : union_tags
 * @Param : struct Database * - the database
 * @Param : char * - the first tag name
 * @Param : char * - the second tag name
 * @Return: struct G_file - simple linked list of union files
 **/
 struct G_file * union_files(struct Database * db, const char * tag1, const char * tag2) {
  struct G_file * cf = NULL;//Simple linked list of union files
  struct G_file * gf;
  struct Tag *t1, *t2;
  struct Tag_file *tf;
  HASH_FIND_STR(db->tags, tag1, t1);
  HASH_FIND_STR(db->tags, tag2, t2);
  if(t1 != NULL && t2 != NULL) {
    for(tf=t1->t_files; tf != NULL; tf=(struct Tag_file*)(tf->hh.next)){
      if ((gf = (struct G_file*)malloc(sizeof(struct G_file))) == NULL) exit(-1);
      strcpy(gf->name, tf->t_f_name);
      LL_PREPEND(cf, gf);
    }
    for(tf=t2->t_files; tf != NULL; tf=(struct Tag_file*)(tf->hh.next)) {
      if(!exist_g_file_bis(cf, tf->t_f_name)) {
        if ((gf = (struct G_file*)malloc(sizeof(struct G_file))) == NULL) exit(-1);
        strcpy(gf->name, tf->t_f_name);
        LL_PREPEND(cf, gf);
      }
    }
    return cf;
  }
  return cf;
}


void debug_common(struct G_file * gf) {
  struct G_file * elt;
  LL_FOREACH(gf, elt) {
    printf("%s,", elt->name);
  }
  printf("\n");
}


/**
 * Debugs a database.
 * @Name  : debug
 * @Param : struct Database * - Database to debug
 **/
void debug(struct Database * db) {
  int i = 0, j = 0;
  struct File *file;
  struct Tag *tag;
  struct File_tag *file_tag;
  struct Tag_file *tag_file;
  printf("\n\033[37m\t===========================DATABASE==========================\n");
  printf("\033[32m\t|---------------------------%zu Files-------------------------|\n", db->f_size);
  printf("\t|\t\t\t\t\t\t\t    |\n");
  for(file=db->files; file != NULL; file=(struct File*)(file->hh.next)) {
    //for_debug(file->f_name);
    printf("\t|\t | %d | %s%c| Matches %zu %s tag-s \t\t    |\n", file->f_id, file->f_name, (strlen(file->f_name) < 6) ? '\t' : ' ' , file->f_tags_size, (file->f_tags_size == db->t_size && db->t_size) ? "(all)" : "");
    i++;
    for(file_tag=file->f_tags; file_tag != NULL; file_tag=(struct File_tag*)(file_tag->hh.next))
      printf("\t|\t\t\t|   %d : %s    \t\t    |\n", file_tag->f_t_id, file_tag->f_t_name);
    printf("\t|\t\t\t\t\t\t\t    |\n");
  }

  printf("\033[36m\t|---------------------------%zu Tags--------------------------|\n", db->t_size);
  printf("\t|\t\t\t\t\t\t\t    |\n");
  for(tag=db->tags; tag != NULL; tag=(struct Tag*)(tag->hh.next)) {
    //for_debug(tag->t_name);
    printf("\t|\t | %d | %s\t| Matches %zu %s file-s \t\t    |\n", tag->t_id, tag->t_name, tag->t_files_size, (tag->t_files_size == db->f_size && db->f_size) ? "(all)" : "");
    j++;
    for(tag_file=tag->t_files; tag_file != NULL; tag_file=(struct Tag_file*)(tag_file->hh.next))
      printf("\t|\t\t\t|   %d : %s    \t\t    |\n", tag_file->t_f_id, tag_file->t_f_name);
    printf("\t|\t\t\t\t\t\t\t    |\n");
  }
  printf("\033[37m\t=============================================================\n");
  printf("\n");

}
