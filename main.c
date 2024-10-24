#include <stdio.h>
#include "segdef.h"
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int shmid;
segment * segment1;
int semid;

void initializations() {
    shmid = shmget(cle,0,0);
    if (shmid == -1) {
        perror("shmget");
        exit(1);}

    printf("shmid: %d\n" , shmid);

    void * shmaddr = shmat(shmid,0,0);
    if (shmaddr== (char *) -1) {
        perror("shmat");
        exit(1);}

    semid = semget(cle,1,0);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    segment1 = shmaddr;
    init_rand();
}

void serveurRequest(void) {

    printf("coucou2\n");
    static int req = 0;
    //    ** STEP 1 **
    acq_sem(semid,seg_dispo);

    //    ** STEP 2 **
    segment1->pid = getpid();
    segment1->req = req;
    segment1->result = - 1;

    long clientMean = 0;
    for (int i = 0; i<maxval; i++) {
        const long val = getrand(maxval);
        //printf("val= %ld\n", val);
        segment1->tab[i]=val;
        clientMean+= val;
    }

    clientMean /= maxval;
    acq_sem(semid,seg_init);

    //    ** STEP 3 **
    wait_sem(semid,res_ok);

    //    ** STEP 4 **
    long serverMean = segment1->result;
    lib_sem(semid,seg_init);

    //    ** STEP 5 **
    wait_sem(semid,res_ok);

    //    ** STEP 6 **
    lib_sem(semid,seg_dispo);

    req++;

    //    ** STEP 7 **
    printf("client mean: %ld, server mean: %ld\n", clientMean, serverMean);

    if (clientMean == serverMean) { printf("yeahhh we're so goooood !!\n\n");}
    else { printf("nooooo!! damn it !!\n\n");}
}

int main(void) {
    printf("Hello, World!\n");

    for(int i = 0; i < 12; i++) {
        int pidnumber = fork();
    }

    //    ** STEP 0 **
    initializations();

    printf("coucou1\n");


    for (int i = 0; i<2; i++) {
        serveurRequest();
    }

    printf("coucou3\n");




    return 0;
}
