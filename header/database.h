#ifndef DATABASE_H_
#define DATABASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/uthash.h"
#include "../header/utlist.h"

#define LENGTH 50 // The maximum length of file names and tag names

struct G_tag {
  char name[LENGTH];
  struct G_tag *next;
};

struct G_file {
  char name[LENGTH];
  struct G_file *next;
};

struct Tag_file {
  char t_f_name[LENGTH];
  int t_f_id;
  UT_hash_handle hh;
};

//
// Tag
///////

struct Tag {
  char t_name[LENGTH]; //key
  int t_id;

  struct Tag_file * t_files; //hash table of tag files using uthash
  size_t t_files_size;

  UT_hash_handle hh;
};

struct File_tag {
  char f_t_name[LENGTH];
  int f_t_id;
  UT_hash_handle hh;
};

//
// File
/////////

struct File {
  char f_name[LENGTH];
  int f_id;

  struct File_tag * f_tags; //hash table of file tags using uthash
  size_t f_tags_size;

  UT_hash_handle hh;
};

//
// Database
////////////

struct Database {
  struct File *files; //hash table of files using uthash
  size_t f_size;
  struct Tag * tags; //hash table of tags using uthash
  size_t t_size;
  struct G_tag *g_tags; //singly linked list for general tags using utlist
  size_t gt_size;
  struct G_file *g_files; //singly linked list for general files using utlist
  size_t gf_size;
};

/**
* Creates a database without initializing it.
* @Name  : db_create
* @Return: struct Database * - Database
**/
struct Database * db_create();

/**
* Initializes a database.
* @Name  : db_init
* @Param : struct Database * - Database to initialize
**/
void db_init(struct Database *);

/**
 * Adds a new general tag to the data base, that'll be applied to all existing and new files
 * @Name  : add_new_g_tag
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
void add_new_g_tag(struct Database *, const char *);

/**
 * Does this general tag exist ?
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 * @Return: int - 1 if g_tag exists, else 0
 **/
int exist_g_tag(struct Database *, const char *);

int exist_g_tag_bis(struct G_tag *, const char *);

/**
 * Updates a file by adding to it g_tags
 * @Name  : update_tag_g_files
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
void update_file_g_tags(struct Database *, const char *);

/**
 * Updates all files by adding to them g_tags
 * @Name  : update_g_tags
 * @Param : struct Database * - the database
 **/
void update_g_tags(struct Database *);

/**
 * Deletes a g_tag from the database and frees it from the memory
 * @Name  : clear_g_tag
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
void clear_g_tag(struct Database *, const char *);

/**
 * Deletes all g_tags from the database and frees them from the memory
 * @Name  : clear_g_tag
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
void clear_g_tags(struct Database *);

/**
 * Debugs g_files.
 * @Name  : debug
 * @Param : struct Database * - Database to debug
 **/
void debug_g_tags(struct Database *);

/**
 * Adds a new general file to the data base, that'll be applied to all existing and new tags
 * @Name  : add_new_g_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 **/
void add_new_g_file(struct Database *, const char *);

/**
 * Does this general file exist ?
 * @Name  : exist_g_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Return: int - 1 if g_file exists, else 0
 **/
int exist_g_file(struct Database *, const char *);

int exist_g_file_bis(struct G_file *, const char *);

/**
 * Updates a tag by adding to it g_files
 * @Name  : update_tag_g_files
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
void update_tag_g_files(struct Database *, const char *);

/**
 * Updates all tags by adding to them g_files
 * @Name  : update_g_files
 * @Param : struct Database * - the database
 **/
void update_g_files(struct Database *);

/**
 * Deletes a g_file from the database and frees it from the memory
 * @Name  : clear_g_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 **/
void clear_g_file(struct Database *, const char *);

/**
 * Deletes all g_files from the database and frees them from the memory
 * @Name  : clear_g_file
 * @Param : struct Database * - the database
 **/
void clear_g_files(struct Database *);

/**
 * Debugs g_tags of a given database.
 * @Name  : debug_g_tags
 * @Param : struct Database * - Database
 **/
void debug_g_files(struct Database *);

/**
 * Adds a new tag to the data base, if it doesn't exist already
 * @Name  : add_new_tag
 * @Param : struct Database * - the database
 * @Return: char * - the tag name
 **/
void add_new_tag(struct Database *, const char *);

/**
 * Adds a new file to the data base, if it doesn't exist already
 * @Name  : add_new_file
 * @Param : struct Database * - the database
 * @Return: char * - the file name
 **/
void add_new_file(struct Database *, const char *);


/**
 * Renames a tag in the database
 * @Name  : rename_tag
 * @Param : struct Database * - the database
 * @Param : char * - the tag to rename
 * @Param : char * - the new tag name
 **/
void rename_tag(struct Database *, const char *, const char *);

/**
 * Renames a file in the database
 * @Name  : rename_file
 * @Param : struct Database * - the database
 * @Param : char * - the file to rename
 * @Param : char * - the new file name
 **/
void rename_file(struct Database *, const char *, const char *);


/**
 * Adds a new tag to an existing file, creates the tag if it doesn't exist
 * @Name  : add_tag_to_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 **/
void add_tag_to_file(struct Database *, const char *, const char *);

/**
 * Adds a new file to an existing tag, creates the file if it doesn't exist
 * @Name  : add_file_to_tag
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 **/
void add_file_to_tag(struct Database *, const char *, const char *);

/**
 * Adds a tag to all files, creates the tag if it doesn't exist
 * @Name  : add_tag_to_all_files
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
void add_tag_to_all_files(struct Database *, const char *);

/**
 * Adds a file to all tags, creates the file if it doesn't exist
 * @Name  : add_file_to_all_tags
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 **/
void add_file_to_all_tags(struct Database *, const char *);

/**
 * Is this tag general, to all files
 * @Name  : is_this_tag_general
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
int is_this_tag_general(struct Database *, const char *);

/**
 * Is this file general, to all tags
 * @Name  : is_this_file_general
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 **/
int is_this_file_general(struct Database *, const char *);


/**
 * Deletes a tag from a file in the database and frees it form the memory
 * @Name  : delete_tag_from_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 **/
void delete_tag_from_file(struct Database *, const char *, const char *);

/**
 * Deletes a file from a tag in the database and frees it form the memory
 * @Name  : delete_file_from_tag
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 **/
void delete_file_from_tag(struct Database *, const char *, const char *);

/**
 * Deletes a tag and its files in the database and frees it
   form the memory (uses delete_tag_from_file())
 * @Name  : delete_tag
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 **/
void delete_tag(struct Database *, const char *);

/**
 * Deletes a file and its tags in the database and frees it
   form the memory (uses delete_file_from_tag())
 * @Name  : delete_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 **/
void delete_file(struct Database *, const char *);

/**
 * Deletes all tags in the database and frees them
   form the memory (uses delete_tag())
 * @Name  : delete_all_tags
 * @Param : struct Database * - the database
 **/
void delete_all_tags(struct Database *);

/**
 * Deletes all files in the database and frees them
   form the memory (uses delete_file())
 * @Name  : delete_all_files
 * @Param : struct Database * - the database
 **/
void delete_all_files(struct Database *);

/**
 * Deletes all files and all tags in the database and frees them
   form the memory (uses delete_file() and delete_tag())
 * @Name  : delete_all
 * @Param : struct Database * - the database
 **/
void delete_all(struct Database *);

/**
 * Gets a file.
 * @Name  : get_file
 * @Param : struct Database * - the database
 * @Param : char * - file
 * @Return: struct File * - the file or null of it doesn't exist
 **/
struct File * get_file(struct Database *, const char *);

/**
 * Gets all files.
 * @Name  : get_files
 * @Param : struct Database * - the database
 * @Return: struct G_file - simple linked list of all files
 **/
struct G_file * get_files(struct Database *);

/**
 * Gets a tag.
 * @Name  : get_tag
 * @Param : struct Database * - the database
 * @Param : char * - tag
 * @Return: struct Tag * - the tag or null if it doesn't exist
 **/
struct Tag * get_tag(struct Database *, const char *);

/**
 * Gets all tags.
 * @Name  : get_tags
 * @Param : struct Database * - the database
 * @Return: struct G_tag - simple linked list of all tags
 **/
struct G_tag * get_tags(struct Database *);

/**
 * Deletes a simple linked list of G_file from memory
 * @Name  : clear__list_files
 * @Param : struct G_file * - the list to delete
 **/
void clear_list_files(struct G_file *);

/**
 * Deletes a simple linked list of G_tag from memory
 * @Name  : clear_list_tags
 * @Param : struct G_tag * - the list to delete
 **/
void clear_list_tags(struct G_tag *);

/**
 * Gets a tag file.
 * @Name  : get_tag_file
 * @Param : struct Database * - the database
 * @Param : char * - file
 * @Param : char * - tag
 * @Return: struct Tag_file * - the tag file or null if it doesn't exist
 **/
struct Tag_file * get_tag_file(struct Database *, const char *, const char *);

/**
 * Gets a file tag.
 * @Name  : get_file_tag
 * @Param : struct Database * - the database
 * @Param : char * - file
 * @Param : char * - tag
 * @Return: struct File_tag * - the file tag or null if it doesn't exist
 **/
struct File_tag * get_file_tag(struct Database *, const char *, const char *);

/**
 * Does this file exist ?
 * @Name  : exist_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Return: int - 1 if the file exist, else 0
 **/
int exist_file(struct Database *, const char *);

/**
 * Does this tag exist ?
 * @Name  : exist_tag
 * @Param : struct Database * - the database
 * @Param : char * - the tag name
 * @Return: int - 1 if the tag exist, else 0
 **/
int exist_tag(struct Database *, const char *);

/**
 * Is this tag belonging to this file ?
 * @Name  : is_tag_in_file
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 * @Return: int - 1 if the tag belongs to the file, else 0
 **/
int is_tag_in_file(struct Database *, const char *, const char *);

/**
 * Is this file belonging to this tag ?
 * @Name  : is_file_in_tag
 * @Param : struct Database * - the database
 * @Param : char * - the file name
 * @Param : char * - the tag name
 * @Return: int - 1 if the file belongs to the tag, else 0
 **/
int is_file_in_tag(struct Database *, const char *, const char *);

/**
 * What are the common files between the two tags given?
 * @Name  : common_files
 * @Param : struct Database * - the database
 * @Param : char * - the first tag name
 * @Param : char * - the second tag name
 * @Return: struct G_file - simple linked list of common files
 **/
struct G_file * common_files(struct Database *, const char *, const char *);


/**
 * What are the common tags between the two files given?
 * @Name  : common_tags
 * @Param : struct Database * - the database
 * @Param : char * - the first file name
 * @Param : char * - the second file name
 * @Return: struct G_tag - simple linked list of common tags
 **/
struct G_tag * common_tags(struct Database *, const char *, const char *);

/**
 * What are the common files between the tag given files and the simple linked list of files given?
 * @Name  : intersect_files
 * @Param : struct Database * - the database
 * @Param : struct G_file - simple linked list files
 * @Param : char * - the tag name
 * @Return: struct G_file - simple linked list of common files
 **/
struct G_file * intersect_files(struct Database *, struct G_file *, const char *);

/**
 * What are the common tags between the file given tags and the simple linked list of tags given?
 * @Name  : intersect_tags
 * @Param : struct Database * - the database
 * @Param : struct G_tag - simple linked list tags
 * @Param : char * - the file name
 * @Return: struct G_tag - simple linked list of common tags
 **/
struct G_tag * intersect_tags(struct Database *, struct G_tag *, const char *);

/**
 * What are the tags of the two files given?
 * @Name  : union_tags
 * @Param : struct Database * - the database
 * @Param : char * - the first tag name
 * @Param : char * - the second tag name
 * @Return: struct G_file - simple linked list of union files
 **/
 struct G_file * union_files(struct Database *, const char *, const char *);

 /**
  * Debugs a G_file.
  * @Name  : debug_common
  * @Param : struct G_file * - G_file to debug
  **/

void debug_common(struct G_file *);

/**
 * Debugs a database.
 * @Name  : debug
 * @Param : struct Database * - Database to debug
 **/
void debug(struct Database *);

#endif
