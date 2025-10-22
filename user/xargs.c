#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[512];  // Buffer to store each line
  char *xargv[MAXARG];  // Arguments array for exec
  int i, n;
  char *p;
  
  // Check if we have at least one argument (the command to run)
  if(argc < 2){
    write(2, "usage: xargs command [args...]\n", 32);
    exit(1);
  }
  
  // Copy the command and its arguments to xargv
  for(i = 1; i < argc; i++){
    xargv[i-1] = argv[i];
  }
  
  // Read lines from stdin and execute command for each line
  while(1){
    // Read one line into buf
    p = buf;
    while(1){
      n = read(0, p, 1);
      if(n == 0){  // EOF
        goto done;
      }
      if(n < 0){
        write(2, "xargs: read error\n", 18);
        exit(1);
      }
      if(*p == '\n'){
        *p = 0;  // Null-terminate the line
        break;
      }
      p++;
      if(p - buf >= 512){
        write(2, "xargs: line too long\n", 21);
        exit(1);
      }
    }
    
    // Skip empty lines
    if(buf[0] == 0){
      continue;
    }
    
    // Add the line as the last argument
    xargv[argc-1] = buf;
    xargv[argc] = 0;  // Null-terminate the argument array
    
    // Fork and exec
    if(fork() == 0){
      // Child process
      exec(xargv[0], xargv);
      write(2, "xargs: exec failed\n", 19);
      exit(1);
    } else {
      // Parent process - wait for child
      wait(0);
    }
  }
  
done:
  exit(0);
}