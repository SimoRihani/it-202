#include "../header/parser.h"

int main(void) {
  int running = 1;
  char in[LENGTH];
  char inB[LENGTH];

  struct Database *db = db_create();
  db_init(db);

  debug(db);

  printf("\t  ==> A random set of operations\n");
  printf("\t \033[31m + tag1\n");
  add_new_tag(db, "tag1");
  add_new_tag(db, "tag1"); //just to see if it will readd the same tag. But it won't..

  printf("\t \033[31m + tag2\n");
  add_new_tag(db, "tag2");

  printf("\t  + file1 to tag1 (+ file1)\n");
  add_file_to_tag(db, "file1", "tag1");
  add_file_to_tag(db, "file1", "tag1"); //for the same reason

  printf("\t  + file1 to tag2\n");
  add_file_to_tag(db, "file1", "tag2");

  printf("\t  + file2\n");
  add_new_file(db, "file2");

  printf("\t  + tag2 to file2\n");
  add_tag_to_file(db, "file2", "tag2");
  add_tag_to_file(db, "file2", "tag2"); // for the same reason

  printf("\t  + tag3 to file2 (+ tag3)\n");
  add_tag_to_file(db, "file2", "tag3");

  printf("\t  + file3\n");
  add_new_file(db, "file3");
  add_new_file(db, "file3"); // for the same reason

  printf("\t  + file4\n");
  add_new_file(db, "file4");

  printf("\t  + tag4 to file3 (+ tag4)\n");
  add_tag_to_file(db, "file3", "tag4");

  printf("\t  + tag5 to file3 (+ tag5)\n");
  add_tag_to_file(db, "file3", "tag5");

  printf("\t  + tag1 to file4\n");
  add_file_to_tag(db, "file4", "tag1");

  debug(db);

  while (running) {
        printf("\n\033[37m\t========================TAKE CONTROL========================\n");
        printf("\t| 1.  Add file\n");
        printf("\t| 2.  Add tag ($mkdir)\n");
        printf("\t| 3.  Rename file\n");
        printf("\t| 4.  Rename tag ($mv)\n");
        printf("\t| 5.  Add file to tag\n");
        printf("\t| 6.  Add tag to file ($ln)\n");
        printf("\t| 7.  Add file to all tags\n");
        printf("\t| 8.  Add tag to all files\n");
        printf("\t| 9.  Is this file general?\n");
        printf("\t| 10. Is this tag general?\n");
        printf("\t| 11. Delete file\n");
        printf("\t| 12. Delete tag ($rmdir tag)\n");
        printf("\t| 13. Delete file from tag ($rm)\n");
        printf("\t| 14. Delete tag from file\n");
        printf("\t| 15. Delete all files\n");
        printf("\t| 16. Delete all tags\n");
        printf("\t| 17. Parse a print\n");
        printf("\t| 18. Generate the current print of database\n");
        printf("\t| 19. Find file\n");
        printf("\t| 20. Add G_tag\n");
        printf("\t| 21. Add G_file\n");
        printf("\t| 22. Common files?\n");
        printf("\t| 23. Common tags?\n");
        printf("\t| 24. Intersect tags?\n");
        printf("\t| 25. Union tags files?\n");
        printf("\t| 26. Quit\n");
        printf("\033[37m\t=============================================================\n");
        gets(in);
        switch(atoi(in)) {
            case 1:
              printf("File name?\n");
              add_new_file(db, gets(in));
              debug(db);
              break;
            case 2:
              printf("Tag name?\n");
              add_new_tag(db, gets(in));
              debug(db);
              break;
            case 3:
              printf("Old name?\n");
              gets(inB);
              printf("New name\n");
              rename_file(db, inB, gets(in));
              debug(db);
              break;
            case 4:
              printf("Old name?\n");
              gets(inB);
              printf("New name\n");
              rename_tag(db, inB, gets(in));
              debug(db);
              break;
            case 5:
                printf("File name?\n");
                gets(inB);
                printf("Tag name?\n");
                add_file_to_tag(db, inB, gets(in));
                debug(db);
                break;
            case 6:
              printf("Tag name?\n");
              gets(inB);
              printf("File name?\n");
              add_tag_to_file(db, gets(in), inB);
              debug(db);
              break;
            case 7:
              printf("File name?\n");
              add_file_to_all_tags(db, gets(in));
              debug(db);
              break;
            case 8:
              printf("Tag name?\n");
              add_tag_to_all_files(db, gets(in));
              debug(db);
              break;
            case 9:
              printf("File name?\n");
              int a = is_this_file_general(db, gets(in));
              printf("\t\t%s\n", a ? "YES" : "NO");
              debug(db);
              break;
            case 10:
              printf("Tag name?\n");
              int b = is_this_tag_general(db, gets(in));
              printf("\t\t%s\n", b ? "YES" : "NO");
              debug(db);
              break;
            case 11:
              printf("File name?\n");
              delete_file(db, gets(in));
              debug(db);
              break;
            case 12:
              printf("Tag name?\n");
              delete_tag(db, gets(in));
              debug(db);
              break;
            case 13:
              printf("File name?\n");
              gets(inB);
              printf("Tag name?\n");
              delete_file_from_tag(db, inB, gets(in));
              debug(db);
              break;
            case 14:
              printf("Tag name?\n");
              gets(inB);
              printf("File name?\n");
              delete_tag_from_file(db, gets(in), inB);
              debug(db);
              break;
            case 15:
              delete_all_files(db);
              debug(db);
              break;
            case 16:
              delete_all_tags(db);
              debug(db);
              break;
            case 17:
              printf("Print name?\n");
              parse(db, gets(in));
              debug(db);
              break;
            case 18:
              printf("Print name?\n");
              print(db, gets(in));
              debug(db);
              break;
            case 19:
              printf("File name?\n");
              printf("%d\n", exist_file(db, gets(in)));
              break;
            case 20:
              printf("Tag name?\n");
              add_new_g_tag(db, gets(in));
              debug(db);
              break;
            case 21:
              printf("file name?\n");
              add_new_g_file(db, gets(in));
              debug(db);
              break;
            case 22:
              printf("First tag name?\n");
              gets(inB);
              printf("Second tag name?\n");
              struct G_file * gf = common_files(db, gets(in), inB);
              debug_common(gf);
              debug(db);
              clear_list_files(gf);
              break;
            case 23:
              printf("First file name?\n");
              gets(inB);
              printf("Second file name?\n");
              common_tags(db, gets(in), inB);
              debug(db);
              break;
            case 24:
              printf("File name?\n");
              gets(inB);
              struct G_tag * cf = NULL, *gt, *gt2;
              if ((gt = (struct G_tag*)malloc(sizeof(struct G_tag))) == NULL) exit(-1);
              strcpy(gt->name, "TAG");
              LL_PREPEND(cf, gt);
              if ((gt2 = (struct G_tag*)malloc(sizeof(struct G_tag))) == NULL) exit(-1);
              strcpy(gt2->name, "foo");
              LL_PREPEND(cf, gt2);
              intersect_tags(db, cf, inB);
              clear_list_tags(cf);
              break;
            case 25:
              printf("First tag name?\n");
              gets(inB);
              printf("Second tag name?\n");
              gf = union_files(db, gets(in), inB);
              debug_common(gf);
              debug(db);
              clear_list_files(gf);
              break;
            case 26:
              printf("\t\t\t See you !\n\n");
              running=0;
              break;
        }
    }

  delete_all(db);

  return 0;
}
