#include <stdio.h>
#include <stdlib.h>

#include "test_database.h"

int main(void){
  test_add_new_tag();
  test_add_new_file();
  //test_rename_tag();
  //test_rename_file();
  test_add_tag_to_file();
  test_add_file_to_tag();
  test_add_tag_to_all_files();
  test_add_file_to_all_tags();
  test_is_this_tag_general();
  test_is_this_file_general();
  test_delete_file_from_tag();
  test_delete_tag_from_file();
  test_delete_tag();
  test_delete_file();
  test_delete_all_tags();
  test_delete_all_files();
}
