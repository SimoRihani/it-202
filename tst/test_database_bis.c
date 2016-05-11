#include "test_database_bis.h"

void test_add_new_tag(){
  struct Database * db = db_create();
  db_init(db);
  assert(db->t_size == 0);
  
  add_new_tag(db, "tag");
  assert(db->t_size == 1);
  struct Tag *t = get_tag(db, "tag");
  assert(!strcmp(t->t_name, "tag"));

  add_new_tag(db, "tag2");
  assert(db->t_size == 2);
  struct Tag *t = get_tag(db, "tag2");
  assert(!strcmp(t2->t_name, "tag2"));
  
  delete_all(db);

  printf("Test add_new_tag passed! \n");
}

void test_add_new_file() {
  struct Database * db = db_create();
  db_init(db);
  assert(db->f_size == 0);

  add_new_file(db, "file");
  assert(db->f_size == 1);
  struct Tag *f = get_file(db, "file");
  assert(!strcmp(f->f_name, "file"));

  add_new_file(db, "file2");
  assert(db->f_size == 2);
  struct Tag *f2 = get_tag(db, "file2");
  assert(!strcmp(f2->f_name, "file2"));
  

  delete_all(db);

  printf("Test add_new_file passed! \n");
}

void test_rename_tag() {
  struct Database * db = db_create();
  db_init(db);

  add_new_tag(db, "tag");
  Struct Tag *t = get_tag(db, "tag"); 
  assert(!strcmp(t->t_name, "tag"));

  rename_tag(db, "tag", "newTag");
  assert(!strcmp(t->t_name, "newTag"));

  delete_all(db);

  printf("Test rename_tag passed! \n");
  
}

void test_rename_file(){
  struct Database * db = db_create();
  db_init(db);

  add_new_file(db, "file");
  Struct File *f = get_file(db, "file"); 
  assert(!strcmp(f->f_name, "file"));

  rename_file(db, "file", "newFile");
  assert(!strcmp(f->f_name, "newFile"));

  delete_all(db);

  printf("Test rename_file passed! \n");
  
}

void test_add_tag_to_file(){
  struct Database * db = db_create();
  db_init(db);

  add_new_file(db, "file");
  add_tag_to_file(db, "file", "tag");
  assert(db->t_size == 1);
  Struct Tag *t = get_tag(db, "tag");
  assert(!strcmp(t->t_name, "tag"));
  assert(is_tag_in_file(db, "file", "tag"));

  add_tag_to_file(db, "file", "tag2");
  assert(db->t_size == 2);
  Struct Tag *t2 = get_tag(db, "tag2");
  assert(!strcmp(t2->t_name, "tag2"));
  assert(is_tag_in_file(db, "file", "tag2"));

  add_new_file(db, "file2");
  add_tag_to_file(db, "file2", "tag");
  assert(db->t_size == 2);
  assert(is_tag_in_file(db, "file2", "tag"));

  add_tag_to_file(db, "file2", "tag3");
  assert(db->t_size == 3);
  assert(is_tag_in_file(db, "file2", "tag3"));

  assert(!is_tag_in_file(db, "file", "tag3"));

  delete_all(db);
  
  printf("Test add_tag_to_file passed! \n");
}

void test_add_file_to_tag(){
  struct Database * db = db_create();
  db_init(db);

  add_new_tag(db, "tag");
  add_file_to_tag(db, "file", "tag");
  assert(db->f_size == 1);
  Struct File *f = get_file(db, "file");
  assert(!strcmp(f->f_name, "file"));
  assert(is_file_in_tag(db, "file", "tag"));

  add_file_to_tag(db, "file2", "tag");
  assert(db->f_size == 2);
  Struct File *f2 = get_file(db, "file2");
  assert(!strcmp(f2->f_name, "file2"));
  assert(is_tag_in_file(db, "file2", "tag"));

  add_new_tag(db, "tag2");
  add_file_to_tag(db, "file", "tag2");
  assert(db->f_size == 2);
  assert(is_file_in_tag(db, "file", "tag2"));

  add_file_to_tag(db, "file3", "tag2");
  assert(db->f_size == 3);
  assert(is_file_in_tag(db, "file3", "tag2"));

  assert(!is_file_in_tag(db, "file3", "tag"));

  delete_all(db);
  
  printf("Test add_file_to_tag passed! \n");
}

void test_is_this_tag_general() {

  struct Database * db = db_create();
  db_init(db);
  add_new_file(db, "b.jpg");
  add_new_file(db, "c.jpg");
  add_tag_to_all_files(db, "tagAll");
  int a = is_this_tag_general(db, "tagAll");
  assert(a);
  add_new_tag(db, "newTag");
  add_file_to_tag(db, "a.jpg", "newTag");
  int b = is_this_tag_general(db, "newTag");
  assert(!b);
  printf("Test is_this_tag_general passed! \n");

  delete_all(db);
}

void test_is_this_file_general() {

  struct Database * db = db_create();
  db_init(db);
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_file_to_all_tags(db, "a.jpg");
  int a = is_this_file_general(db, "a.jpg");
  assert(a);
  add_new_file(db, "b.jpg");
  add_tag_to_file(db, "newTag", "b.jpg");
  int b = is_this_file_general(db, "b.jpg");
  assert(!b);
  printf("Test is_this_file_general passed! \n");

  delete_all(db);
}

void test_delete_file_from_tag() {

  struct Database * db = db_create();
  db_init(db);
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_new_file(db, "a.jpg");
  add_new_file(db, "b.jpg");
  add_file_to_tag(db, "a.jpg", "tag1");
  add_file_to_tag(db, "b.jpg", "tag2");
  add_tag_to_file(db, "tag1", "a.jpg");
  add_tag_to_file(db, "tag2", "b.jpg");
  delete_file_from_tag(db, "a.jpg", "tag1");
  int a = is_file_related_to_tag(db, "a.jpg", "tag1");
  assert(a);

  printf("Test delete_file_from_tag passed! \n");

  delete_all(db);
}

void test_delete_tag_from_file() {

  struct Database * db = db_create();
  db_init(db);
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_new_file(db, "a.jpg");
  add_new_file(db, "b.jpg");
  add_file_to_tag(db, "a.jpg", "tag1");
  add_file_to_tag(db, "b.jpg", "tag2");
  add_tag_to_file(db, "tag1", "a.jpg");
  add_tag_to_file(db, "tag2", "b.jpg");
  int a = is_tag_related_to_file(db, "a.jpg", "taga");
  assert(a);
  delete_tag_from_file(db, "a.jpg", "tag1");
  a = is_tag_related_to_file(db, "a.jpg", "tag1");
  assert(!a);
 
  printf("Test delete_file_from_tag passed! \n");

  delete_all(db);
}

void test_delete_tag() {

  struct Database * db = db_create();
  db_init(db);
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_new_file(db, "a.jpg");
  add_new_file(db, "b.jpg");
  add_file_to_tag(db, "a.jpg", "tag1");
  add_file_to_tag(db, "b.jpg", "tag2");
  add_tag_to_file(db, "tag1", "a.jpg");
  add_tag_to_file(db, "tag2", "b.jpg");
  int a = exist_tag(db, "tag1");
  assert(a);
  delete_tag(db, "tag1");
  a = exist_tag(db, "tag1");
  assert(!a);

  printf("Test delete_tag passed! \n");

  delete_all(db);

}

void test_delete_file() {

  struct Database * db = db_create();
  db_init(db);
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_new_file(db, "a.jpg");
  add_new_file(db, "b.jpg");
  add_file_to_tag(db, "a.jpg", "tag1");
  add_file_to_tag(db, "b.jpg", "tag2");
  add_tag_to_file(db, "tag1", "a.jpg");
  add_tag_to_file(db, "tag2", "b.jpg");
  int a = exist_file(db, "a.jpg");
  assert(a);
  delete_file(db, "a.jpg");
  a = exist_file(db, "a.jpg");
  assert(!a);

  printf("Test delete_tag passed! \n");

  delete_all(db);
  }


void test_delete_all_tags() {
  struct Database * db = db_create();
  db_init(db);
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_new_file(db, "a.jpg");
  add_new_file(db, "b.jpg");
  add_file_to_tag(db, "a.jpg", "tag1");
  add_file_to_tag(db, "b.jpg", "tag2");
  add_tag_to_file(db, "tag1", "a.jpg");
  add_tag_to_file(db, "tag2", "b.jpg");
  delete_all_tags(db);
  assert(db->t_size == 0);

  printf("Test delete_all_tags passed! \n");

  delete_all(db);
}

void test_delete_all_files() {
  struct Database * db = db_create();
  db_init(db);
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_new_file(db, "a.jpg");
  add_new_file(db, "b.jpg");
  add_file_to_tag(db, "a.jpg", "tag1");
  add_file_to_tag(db, "b.jpg", "tag2");
  add_tag_to_file(db, "tag1", "a.jpg");
  add_tag_to_file(db, "tag2", "b.jpg");
  delete_all_files(db);
  assert(db->f_size == 0);

  printf("Test delete_all_files passed! \n");

  delete_all(db);
}



