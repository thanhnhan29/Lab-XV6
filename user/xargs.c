#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[512];
  char *xargv[MAXARG];
  int i, n;
  char *p;
  if(argc < 2){
    write(2, "usage: xargs command [args...]\n", 32);
    exit(1);
  }
  for(i = 1; i < argc; i++){
    xargv[i-1] = argv[i];
  }
  while(1){
    p = buf;
    while(1){
      n = read(0, p, 1);
      if(n == 0){
        exit(0);
      }
      if(n < 0){
        write(2, "xargs: read error\n", 18);
        exit(1);
      }
      if(*p == '\n'){
        *p = 0;
        break;
      }
      p++;
      if(p - buf >= 512){
        write(2, "xargs: line too long\n", 21);
        exit(1);
      }
    }
    if(buf[0] == 0){
      continue;
    }
    xargv[argc-1] = buf;
    xargv[argc] = 0;  
    if(fork() == 0){
      exec(xargv[0], xargv);
      write(2, "xargs: exec failed\n", 19);
      exit(1);
    } else {
      wait(0);
    }
  }
}