#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int num = 0;
  for(int i=0;i<strlen(argv[1]);i++){
    num = num*10 + (argv[1][i]-'0');
  }
  sleep(num);
  exit(0);
}

