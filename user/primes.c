#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int p, int root){
    int k;
    int first;
    
    if(read(p, &k, sizeof(k)) == sizeof(k)){
        printf("prime %d\n", k);
    }else{
        close(p);
        exit(0);
    }

    first = k;
    
    int p2[2];
    pipe(p2);
    if(fork() != 0){
        close(p2[0]);
        while(read(p, &k, sizeof(k)) == sizeof(k)){
            if(k % first!=0)
                write(p2[1], &k, sizeof(k));
        }
        close(p2[1]);
        close(p);
        wait(0);
    }
    else{
        close(p2[1]);
        close(p);
        primes(p2[0], 0);
        close(p2[0]);
    }
    if(root){
        return;
    }else{
    exit(0);

    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    int pid = fork();
    if(pid == 0){
        close(p[0]);
        for(int i=2;i<=280;i++){
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
    }else{
        close(p[1]);
        primes(p[0],1);
        close(p[0]);
        wait(0);
    }
    exit(0);
}

