﻿#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p1[2];
    pipe(p1);
    int p2[2];
    pipe(p2);
    if(fork()!=0){
        close(p1[0]);
        close(p2[1]);
        int send = 10;
        int rec = 0;
        write(p1[1], &send, sizeof(send));
        read(p2[0], &rec, sizeof(rec));
        if(rec == 10){
            printf("%d: received pong\n", getpid());
        }
        close(p1[1]);
        close(p2[0]);
    }
    else{
        close(p1[1]);
        close(p2[0]);
        int send = 10;
        int rec = 0;
        read(p1[0], &rec, sizeof(rec));
        if(rec == 10){
            printf("%d: received ping\n", getpid());
        }
        write(p2[1], &send, sizeof(send));
        close(p1[0]);
        close(p2[1]);
    }
    exit(0);
}

