#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/param.h"


void
find(char* path, char* name, int execArgc, char **execArgv);

int
main(int argc, char *argv[])
{
  if(argc < 3) {
    fprintf(2, "usage: find path name [-exec cmd ...]\n");
    exit(1);
  }

  int execArgc = 0;
  char **execArgv = 0;

  if(argc >= 5) {
    if(strcmp(argv[3], "-exec") == 0) {
      execArgc = argc - 4;
      execArgv = &argv[4];
    }
  }
  find(argv[1], argv[2], execArgc, execArgv);
  exit(0);
}

void
find(char* path, char* name, int execArgc, char **execArgv)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
          printf("find: cannot stat %s\n", buf);
          continue;
        }
        if(strcmp(p, name) == 0) {
	        if(execArgc > 0) {
	          int pid = fork();
            if (pid == 0) {
              char *argv[MAXARG];
              int i;

              for(i = 0; i < execArgc; i++) {
                argv[i] = execArgv[i];
              }

              argv[execArgc] = buf;
              argv[execArgc + 1] = 0;

              exec(argv[0], argv); 
              exit(0);
            }
            wait(0);
	        } else {
	          printf("%s\n", buf); 
	        }
	      }
        if (st.type == T_DIR) {
          if((strcmp(p, ".")) != 0 && (strcmp(p, "..") != 0)) {
            find(buf, name, execArgc, execArgv);
          }
        }
      }
      break;
    }
  close(fd);
}
