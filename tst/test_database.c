#include "test_database.h"

void test_add_new_tag(){
  struct Database * db = db_create();
  db_init(db);
  assert(db->t_size == 0);

  add_new_tag(db, "tag");
  assert(db->t_size == 1);
  assert(exist_tag(db, "tag"));
  assert(db->f_size == 0);

  struct Tag *t = get_tag(db, "tag");
  assert(!strcmp(t->t_name, "tag"));

  add_new_tag(db, "tag2");
  assert(db->t_size == 2);
  assert(exist_tag(db, "tag2"));
  assert(db->f_size == 0);

  struct Tag *t2 = get_tag(db, "tag2");
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
  assert(db->t_size == 0);
  assert(exist_file(db, "file"));

  struct File *f = get_file(db, "file");
  assert(!strcmp(f->f_name, "file"));

  add_new_file(db, "file2");
  assert(db->f_size == 2);
  assert(db->t_size == 0);
  assert(exist_file(db, "file2"));

  struct File *f2 = get_file(db, "file2");
  assert(!strcmp(f2->f_name, "file2"));


  delete_all(db);

  printf("Test add_new_file passed! \n");
}

void test_rename_tag() {
  struct Database * db = db_create();
  db_init(db);

  add_new_tag(db, "tag");
  struct Tag *t = get_tag(db, "tag");
  assert(!strcmp(t->t_name, "tag"));

  rename_tag(db, "tag", "newTag");
  assert(!strcmp(t->t_name, "newTag"));
  assert(db->t_size == 1);
  assert(!exist_tag(db, "tag"));
  assert(exist_tag(db, "newTag"));

  delete_all(db);

  printf("Test rename_tag passed! \n");

}

void test_rename_file(){
  struct Database * db = db_create();
  db_init(db);

  add_new_file(db, "file");
  struct File *f = get_file(db, "file");
  assert(!strcmp(f->f_name, "file"));

  rename_file(db, "file", "newFile");
  assert(!strcmp(f->f_name, "newFile"));
  assert(db->f_size == 1);
  assert(!exist_file(db, "file"));
  assert(exist_file(db, "newFile"));

  delete_all(db);

  printf("Test rename_file passed! \n");

}

void test_add_tag_to_file(){
  struct Database * db = db_create();
  db_init(db);

  add_new_file(db, "file");
  add_tag_to_file(db, "file", "tag");
  assert(db->t_size == 1);
  assert(exist_tag(db, "tag"));

  struct Tag *t = get_tag(db, "tag");
  assert(!strcmp(t->t_name, "tag"));
  assert(is_tag_in_file(db, "file", "tag"));

  add_tag_to_file(db, "file", "tag2");
  assert(db->t_size == 2);
  assert(exist_tag(db, "tag2"));

  struct Tag *t2 = get_tag(db, "tag2");
  assert(!strcmp(t2->t_name, "tag2"));
  assert(is_tag_in_file(db, "file", "tag2"));

  add_new_file(db, "file2");
  add_tag_to_file(db, "file2", "tag");
  assert(db->t_size == 2);
  assert(exist_tag(db, "tag"));
  assert(is_tag_in_file(db, "file2", "tag"));

  add_tag_to_file(db, "file2", "tag3");
  assert(db->t_size == 3);
  assert(exist_tag(db, "tag3"));
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
  assert(exist_file(db, "file"));

  struct File *f = get_file(db, "file");
  assert(!strcmp(f->f_name, "file"));
  assert(is_file_in_tag(db, "file", "tag"));

  add_file_to_tag(db, "file2", "tag");
  assert(db->f_size == 2);
  assert(exist_file(db, "file2"));

  struct File *f2 = get_file(db, "file2");
  assert(!strcmp(f2->f_name, "file2"));
  assert(is_tag_in_file(db, "file2", "tag"));

  add_new_tag(db, "tag2");
  add_file_to_tag(db, "file", "tag2");
  assert(db->f_size == 2);
  assert(is_file_in_tag(db, "file", "tag2"));

  add_file_to_tag(db, "file3", "tag2");
  assert(db->f_size == 3);
  assert(is_file_in_tag(db, "file3", "tag2"));
  assert(exist_file(db, "file3"));

  assert(!is_file_in_tag(db, "file3", "tag"));

  delete_all(db);

  printf("Test add_file_to_tag passed! \n");
}

  void test_add_tag_to_all_files() {
    struct Database *db = db_create();
    db_init(db);

    add_tag_to_all_files(db, "Tag");
    assert(db->t_size == 0);
    assert(db->f_size == 0);

    add_new_file(db, "file1");
    add_tag_to_all_files(db, "Tag");
    assert(db->t_size == 1);
    assert(is_file_in_tag(db, "file1", "Tag"));
    assert(exist_tag(db, "Tag"));

    add_tag_to_file(db, "file1", "Tag2");
    assert(is_tag_in_file(db, "file1", "Tag2"));

    add_new_file(db, "file2");
    add_tag_to_all_files(db, "Tag");
    assert(db->t_size == 2);
    assert(is_file_in_tag(db, "file2", "Tag"));
    assert(!is_file_in_tag(db, "file2", "Tag2"));
    assert(!is_tag_in_file(db, "file2", "Tag2"));

    delete_all(db);

    printf("Test add_tag_to_all_files passed! \n");
  }

  void test_add_file_to_all_tags() {
    struct Database *db = db_create();
    db_init(db);

    add_file_to_all_tags(db, "File");
    assert(db->t_size == 0);
    assert(db->f_size == 0);

    add_new_tag(db, "tag1");
    add_file_to_all_tags(db, "File");
    assert(db->f_size == 1);
    assert(is_tag_in_file(db, "File", "tag1"));
    assert(exist_file(db, "File"));

    add_file_to_tag(db, "File2", "tag1");
    assert(is_file_in_tag(db, "File2", "tag1"));

    add_new_tag(db, "tag2");
    add_file_to_all_tags(db, "File");
    assert(db->f_size == 2);
    assert(is_tag_in_file(db, "File", "tag2"));
    assert(!is_tag_in_file(db, "File2", "tag2"));
    assert(!is_file_in_tag(db, "File2", "tag2"));

    delete_all(db);

    printf("Test add_tag_to_all_files passed! \n");
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
  assert(is_this_file_general(db, "a.jpg"));
  add_new_file(db, "b.jpg");
  add_tag_to_file(db, "newTag", "b.jpg");
  assert(!is_this_file_general(db, "b.jpg"));
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
  add_file_to_tag(db, "b.jpg", "tag1");
  add_file_to_tag(db, "b.jpg", "tag2");
  assert(is_file_in_tag(db, "a.jpg", "tag1"));
  delete_file_from_tag(db, "a.jpg", "tag1");
  assert(!is_file_in_tag(db, "a.jpg", "tag1"));
  assert(is_file_in_tag(db, "b.jpg", "tag1"));


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
  add_tag_to_file(db, "a.jpg", "tag1");
  add_tag_to_file(db, "a.jpg", "tag2");
  add_tag_to_file(db, "b.jpg", "tag1");
  add_tag_to_file(db, "b.jpg", "tag2");
  assert(is_tag_in_file(db, "a.jpg", "tag1"));
  delete_tag_from_file(db, "a.jpg", "tag1");
  assert(!is_tag_in_file(db, "a.jpg", "tag1"));
  assert(is_tag_in_file(db, "b.jpg", "tag1"));
  assert(is_tag_in_file(db, "a.jpg", "tag2"));


  printf("Test delete_file_from_tag passed! \n");

  delete_all(db);
}

void test_delete_tag() {

  struct Database * db = db_create();
  db_init(db);
  add_new_file(db, "a.jpg");
  add_new_file(db, "b.jpg");
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_tag_to_file(db, "tag1", "a.jpg");
  add_tag_to_file(db, "tag2", "b.jpg");
  assert(exist_tag(db, "tag1"));
  delete_tag(db, "tag1");
  assert(!exist_tag(db, "tag1"));
  assert(exist_tag(db, "tag2"));
  assert(exist_file(db, "a.jpg"));
  assert(exist_file(db, "b.jpg"));


  printf("Test delete_tag passed! \n");

  delete_all(db);

}

void test_delete_file() {

  struct Database * db = db_create();
  db_init(db);
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_file_to_tag(db, "a.jpg", "tag1");
  add_file_to_tag(db, "b.jpg", "tag2");
  assert(exist_file(db, "a.jpg"));
  delete_file(db, "a.jpg");
  assert(!exist_file(db, "a.jpg"));
  assert(exist_file(db, "b.jpg"));
  assert(exist_tag(db, "tag1"));
  assert(exist_tag(db, "tag2"));

  printf("Test delete_tag passed! \n");

  delete_all(db);
  }


void test_delete_all_tags() {
  struct Database * db = db_create();
  db_init(db);
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_file_to_tag(db, "a.jpg", "tag1");
  add_file_to_tag(db, "b.jpg", "tag2");
  add_tag_to_file(db, "tag1", "a.jpg");
  add_tag_to_file(db, "tag2", "b.jpg");
  delete_all_tags(db);
  assert(db->t_size == 0);
  assert(db->f_size == 2);

  printf("Test delete_all_tags passed! \n");

  delete_all(db);
}

void test_delete_all_files() {
  struct Database * db = db_create();
  db_init(db);
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag2");
  add_file_to_tag(db, "a.jpg", "tag1");
  add_file_to_tag(db, "b.jpg", "tag2");
  add_tag_to_file(db, "tag1", "a.jpg");
  add_tag_to_file(db, "tag2", "b.jpg");
  delete_all_files(db);
  assert(db->f_size == 0);
  assert(db->t_size == 2);

  printf("Test delete_all_files passed! \n");

  delete_all(db);
}
