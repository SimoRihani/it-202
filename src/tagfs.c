
#define _GNU_SOURCE
#define FUSE_USE_VERSION 26

#define DB_FILE ".tags"
#define DB_SAVE ".tags.bak"
#define SUCCESS 0
#define FAILURE -1

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <dirent.h>
#include "comm.h"
#include "parser.h"

static DIR *dir;
static char *dirpath;
static struct Database* db;

#define LOGFILE "tagfs.log"
FILE *mylog;
#define LOG(args...) do { fprintf(mylog, args); fflush(mylog); } while(0)

#define LOGCALL() LOG("%s\n", __FUNCTION__)
#define DBG(x) LOG(#x"\n")

int tagfs_link(const char*, const char*);

char* tagfs_makepath(const char* path, int is_real){
  const char *basename = (is_real)? strrchr(path, '/'): strchr(path, '/');
  char *realpath;
  if (!basename)
    /* in case we call get_realpath() without a starting / (may happen in readdir()) */
    basename = path;
  /* prepend dirpath since the daemon runs in '/' */
  asprintf(&realpath, "%s/%s", dirpath, basename);
  return realpath;
}
char* tagfs_realpath(const char* path){
  return tagfs_makepath(path, 1);
}
char* tagfs_fakepath(const char* path){
  return tagfs_makepath(path, 0);
}

int tagfs_getattr(const char *path, struct stat *stbuf){
  LOGCALL();
  char *realpath = tagfs_realpath(path),
       *fakepath = tagfs_fakepath(path);
  int res;

  /* try to stat the actual file */
  res = stat(fakepath, stbuf);
  if (res < 0 && errno == ENOENT){
    if (comm_isSingleTag(path + 1)){
      res = (exist_tag(db, path + 1))? stat(dirpath, stbuf): -ENOENT;
    } else {
      res = SUCCESS;
      char** tags,** tmp, **tmp2;
      comm_getTags(path, &tags);
      tmp2 = tmp = tags;
      // Sections but last: check tag existence
      while (*(tmp+1) != NULL){
        res = (res == SUCCESS && exist_tag(db, *tmp))? SUCCESS: -ENOENT;
        ++tmp;
      }
      // Last section: check file existence, then tag existence
      if (res == SUCCESS){
        if (stat(realpath, stbuf) == 0){
          // File exists: check tags associations with file
		  while(res == SUCCESS && *tmp2 != *tmp){
     	  	if(is_tag_in_file(db, *tmp, *tmp2))
       		  ++tmp2;
     	  	else
       		  res = -ENOENT;
   		  }
        } else {
          // File does not exist: check existence of last tag
          res = (exist_tag(db, *tmp))? stat(dirpath, stbuf): -ENOENT;
        }
      }
      comm_freeTags(tags);
    }
  }

  LOG("getattr returning %s\n", strerror(-res));
  free(realpath);
  free(fakepath);
  return res;
}

int tagfs_readlink(const char *a, char *b, size_t c){
  LOGCALL();
  (void) a;
  (void) b;
  (void) c;
  return 1;
}

int tagfs_getdir(const char *a, fuse_dirh_t b, fuse_dirfil_t c){
  LOGCALL();
  (void) a;
  (void) b;
  (void) c;
  return 1;
}

int tagfs_mknod(const char *a, mode_t b, dev_t c){
  LOGCALL();
  (void) a;
  (void) b;
  (void) c;
  return 1;
}

int tagfs_mkdir(const char *tag, mode_t b){
  LOGCALL();
  (void) b;

  if(comm_isSingleTag(tag+1)){
    // add tag (function checks the existence of the tag)
    add_new_tag(db, tag+1);
    return SUCCESS;
  }
  return FAILURE;
}

int tagfs_unlink(const char *file){
  LOGCALL();

  char **tagsbuf;
  comm_getTags(file,&tagsbuf);
  char **tmp = tagsbuf, **tmp2;
  int cpt = 0, i;
  tmp2 = tmp;

  while(*tmp != NULL){
    tmp2 = tmp;
    tmp++;
    cpt++;
  }

  cpt--;

  if(!exist_file(db, *tmp2)){
    comm_freeTags(tagsbuf);
    return FAILURE;
  }

  for(i = 0 ; i < cpt ; i++){
    delete_tag_from_file(db,*tmp2,tagsbuf[i]);
  }

  comm_freeTags(tagsbuf);

  return SUCCESS;
}

int tagfs_rmdir(const char *tag){
  LOGCALL();

  // remove tag (function checks the existence of the tag)
  delete_tag(db, tag+1);
  return SUCCESS;
}

int tagfs_symlink(const char *a, const char *b){
  LOGCALL();
  (void) a;
  (void) b;
  return 1;
}

int tagfs_rename(const char *a, const char *b){
  LOGCALL();

  char **tagsbuf;
  comm_getTags(a,&tagsbuf);
  char **tmp = tagsbuf, **tmp2;
  int cpt = 0;
  tmp2 = tmp;

  while(*tmp != NULL){
    tmp2 = tmp;
    tmp++;
    cpt ++;
  }

  cpt--;

  char name[strlen(*tmp2)+5];
  name[0] = '/';
  name[1] = '\0';
  strcat(name,*tmp2);

  if(cpt == 1 && exist_file(db, *tmp2)){
    tagfs_link(name, b);
    tagfs_unlink(a);
    comm_freeTags(tagsbuf);
    return SUCCESS;
  }

  comm_freeTags(tagsbuf);

  return FAILURE;
}

int tagfs_link(const char *file, const char *tags){
  LOGCALL();

  char **tagsbuf;
  comm_getTags(tags, &tagsbuf);
  char **tmp = tagsbuf;
  int cpt = 0, i;

  while(tmp != NULL && strcmp(file + 1, *tmp) != 0){
    tmp++;
    cpt++;
  }

  if(*tmp == NULL || *(tmp + 1) != NULL){
    comm_freeTags(tagsbuf);
    return FAILURE;
  }

  if (!exist_file(db, file + 1))
	add_new_file(db, file + 1);
  for(i = 0; i < cpt; ++i)
    add_tag_to_file(db, file + 1, tagsbuf[i]);

  comm_freeTags(tagsbuf);
  return SUCCESS;
}

int tagfs_chmod(const char *a, mode_t b){
  LOGCALL();
  (void) a;
  (void) b;
  return 1;
}

int tagfs_chown(const char *a, uid_t b, gid_t c){
  LOGCALL();
  (void) a;
  (void) b;
  (void) c;
  return 1;
}

int tagfs_truncate(const char *a, off_t b){
  LOGCALL();
  (void) a;
  (void) b;
  return 1;
}

int tagfs_utime(const char *a, struct utimbuf *b){
  LOGCALL();
  (void) a;
  (void) b;
  return 1;
}

int tagfs_open(const char *a, struct fuse_file_info *b){
  LOGCALL();
  (void) a;
  (void) b;
  return 1;
}

int tagfs_read(const char *a, char *b, size_t c, off_t d, struct fuse_file_info *e){
  LOGCALL();
  (void) a;
  (void) b;
  (void) c;
  (void) d;
  (void) e;
  return 1;
}

int tagfs_write(const char *a, const char *b, size_t c, off_t d, struct fuse_file_info *e){
  LOGCALL();
  (void) a;
  (void) b;
  (void) c;
  (void) d;
  (void) e;
  return 1;
}

int tagfs_statfs(const char *a, struct statvfs *b){
  LOGCALL();
  (void) a;
  (void) b;
  return 1;
}

int tagfs_flush(const char *a, struct fuse_file_info *b){
  LOGCALL();
  (void) a;
  (void) b;
  return 1;
}

int tagfs_release(const char *a, struct fuse_file_info *b){
  LOGCALL();
  (void) a;
  (void) b;
  return 1;
}

int tagfs_fsync(const char *a, int b, struct fuse_file_info *c){
  LOGCALL();
  (void) a;
  (void) b;
  (void) c;
  return 1;
}
/*
int tagfs_setxattr(const char *a, const char *b, const char *c, size_t d, int e){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_getxattr(const char *a, const char *b, char *c, size_t d){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_listxattr(const char *a, char *b, size_t c){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_removexattr(const char *a, const char *b){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_opendir(const char *a, struct fuse_file_info *b){
  LOG("%s\n", __FUNCTION__);
  return 1;
  }*/

int tagfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
  LOGCALL();
  (void) offset;
  (void) fi;
  struct dirent *dirent;

  int res = 0;

  char** tagsbuf;
  comm_getTags(path, &tagsbuf);

  int i;
  // Check tag existence and count tags
  if (tagsbuf != NULL)
    for (i = 0; tagsbuf[i]; ++i)
      if (!exist_tag(db, tagsbuf[i]))
        return FAILURE;

  struct G_file* files = NULL;
  struct G_tags* tags = NULL;

  (void) tags;
  if (i >= 1){
    // One: get files then union tags of files
    if (i == 1){
      struct Tag* t = get_tag(db, *tagsbuf);
      unsigned j;
      for (j = 0; j < t->t_files_size; ++j){
        // Add file to files
        // Send tags in UNION
      }
    } else if (i > 1){
      /*
       Files corresponding to the tags. Intersect.
      */
      files = common_files(db, tagsbuf[0], tagsbuf[1]);
      int j;
      for (j = 2; j < i; ++j)
         files = intersect_files(db, files, tagsbuf[j]);
      /*
       Remaining tags from files.
      */
      if (files != NULL){
         struct G_file *tmp;
         for (tmp = files; tmp != NULL; tmp = tmp->next){
           // Send tags in UNION
         }
      }
    }
      /*
        Cross-check with the request.
      */
      /*
        for (tmp2 = tags; tmp2 != NULL; tmp2 = tmp2->next){
          if (!strcmp(union_tagname, tagsbuf))
            // remove union_tag from union
        }
      */
  }

  /*
  	Show results
  	Files:
    Stat each file in the results then call filler
  */
  rewinddir(dir);
  while ((dirent = readdir(dir)) != NULL) {
    struct stat stbuf;
    res = tagfs_getattr(dirent->d_name, &stbuf);
    /* TODO only files whose contents matches tags in path */
    if (S_ISREG(stbuf.st_mode)){ /* only display files, no directories */
      int ok = exist_file(db, dirent->d_name), i = 0;
      if (tagsbuf != NULL)
	    while (ok && tagsbuf[i] != NULL){
          if (is_file_in_tag(db, dirent->d_name, tagsbuf[i]))
	        ++i;
	      else
	        ok = 0;
	    }
      if (ok)
	    filler(buf, dirent->d_name, NULL, 0);
    }
  }

  /*
    Tags:
  */
  if (tagsbuf != NULL){
    for (i = 0; tagsbuf[i]; ++i)
      filler(buf, tagsbuf[i], NULL, 0);
    comm_freeTags(tagsbuf);
  } else {
    // call filler on each tag
  }

  LOG("readdir returning %s\n", strerror(-res));
  return 0;
}

int tagfs_releasedir(const char *a, struct fuse_file_info *b){
  LOGCALL();
  (void) a;
  (void) b;
  return 1;
}

int tagfs_fsyncdir(const char *a, int b, struct fuse_file_info *c){
  LOGCALL();
  (void) a;
  (void) b;
  (void) c;
  return 1;
}
/*
void tagfs_init(struct fuse_conn_info *conn){
  LOG("%s\n", __FUNCTION__);
}

void tagfs_destroy(void *a){
  LOG("%s\n", __FUNCTION__);
}

int tagfs_access(const char *a, int b){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_create(const char *a, mode_t b, struct fuse_file_info *c){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_ftruncate(const char *a, off_t b, struct fuse_file_info *c){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_fgetattr(const char *a, struct stat *b, struct fuse_file_info *c){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_lock(const char *a, struct fuse_file_info *b, int cmd c, struct flock *d){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_utimens(const char *a, const struct timespec tv[2]){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_bmap(const char *a, size_t blocksizeb, uint64_t *idx){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_ioctl(const char *a, int cmd, void *arg, struct fuse_file_info *b, unsigned int flags, void *data){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_poll(const char *a, struct fuse_file_info *b, struct fuse_pollhandle *ph, unsigned *reventsp){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_write_buf(const char *a, struct fuse_bufvec *buf, off_t off b, struct fuse_file_info *c){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_read_buf(const char *, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_flock(const char *, struct fuse_file_info *, int op){
  LOG("%s\n", __FUNCTION__);
  return 1;
}

int tagfs_fallocate(const char *, int, off_t, off_t, struct fuse_file_info *){
  LOG("%s\n", __FUNCTION__);
  return 1;
}*/

static struct fuse_operations tagfs_oper = {
  .getattr = tagfs_getattr,
  .readlink = tagfs_readlink,
  .getdir = tagfs_getdir,
  .mknod = tagfs_mknod,
  .mkdir = tagfs_mkdir,
  .unlink = tagfs_unlink,
  .rmdir = tagfs_rmdir,
  .symlink = tagfs_symlink,
  .rename = tagfs_rename,
  .link = tagfs_link,
  .chmod = tagfs_chmod,
  .chown = tagfs_chown,
  .truncate = tagfs_truncate,
  .utime = tagfs_utime,
  .open = tagfs_open,
  .read = tagfs_read,
  .write = tagfs_write,
  .statfs = tagfs_statfs,
  .flush = tagfs_flush,
  .release = tagfs_release,
  .fsync = tagfs_fsync,
  // .setxattr = tagfs_setxattr,
  // .getxattr = tagfs_getxattr,
  // .listxattr = tagfs_listxattr,
  // .removexattr = tagfs_listxattr,
  //.opendir = tagfs_opendir,
  .readdir = tagfs_readdir,
  //.releasedir = tagfs_releasedir,
  .fsyncdir = tagfs_fsyncdir,
  // .init = tagfs_init,
  /*.destroy = tagfs_destroy,
  .access = tagfs_access,
  .create = tagfs_create,
  .ftruncate = tagfs_ftruncate,
  .fgetattr = tagfs_fgetattr,
  .lock = tagfs_lock,
  .untimens = tagfs_untimens,
  .bmap = tagfs_bmap,
  .ioctl = tagfs_ioctl,
  .poll = tagfs_poll,
  .write_buf = tagfs_write_buf,
  .read_buf = tagfs_read_buf,
  .flock = tagfs_flock,
  .fallocate = tagfs_fallocate,*/
};


int main(int argc, char *argv[])
{
  int err;

  if (argc < 2) {
    fprintf(stderr, "missing destination directory\n");
    exit(EXIT_FAILURE);
  }
  /* find the absolute directory because fuse_main()
   * doesn't launch the daemon in the same current directory.
   */
  dirpath = realpath(argv[1], NULL);
  dir = opendir(dirpath);
  if (!dir) {
    fprintf(stderr, "couldn't open directory %s\n", dirpath);
    exit(EXIT_FAILURE);
  }
  argv++;
  argc--;

  mylog = fopen(LOGFILE, "a"); /* append logs to previous executions */
  LOG("\n");
  char* db_path,* db_savepath;
  asprintf(&db_path, "%s/%s", dirpath, DB_FILE);
  asprintf(&db_savepath, "%s/%s", dirpath, DB_SAVE);

  db = db_create();
  db_init(db);
  parse(db, db_path);

  LOG("Starting in %s\n", dirpath);
  err = fuse_main(argc, argv, &tagfs_oper, NULL);
  LOG("Stopped with return code %d\n", err);

  if (!rename(db_path, db_savepath)){
  	print(db, db_path);
    LOG("Old DB saved in: %s\nNew DB saved in: %s\n", db_savepath, db_path);
  } else {
    LOG("Error while saving DB\n");
  }

  delete_all(db);
  closedir(dir);
  free(dirpath);
  free(db_path);
  free(db_savepath);

  LOG("\n");
  return err;
}
