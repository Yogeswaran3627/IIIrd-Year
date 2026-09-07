#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 5

#define MUTEX 0
#define EMPTY 1
#define FULL 2

#define SHM_KEY_PATH "shared.h"
#define PROJ_ID 65

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

struct SharedData {
  int buffer[SIZE];
  int in;
  int out;
};

void wait_sem(int semid, int semnum) {
  struct sembuf sb;
  sb.sem_num = semnum;
  sb.sem_op = -1;
  sb.sem_flg = 0;
  semop(semid, &sb, 1);
}

void signal_sem(int semid, int semnum) {
  struct sembuf sb;
  sb.sem_num = semnum;
  sb.sem_op = 1;
  sb.sem_flg = 0;
  semop(semid, &sb, 1);
}

#endif
