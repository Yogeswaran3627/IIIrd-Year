#include "shared.h"

int main() {
  int shmid, semid, i;
  key_t key;
  struct SharedData *data;
  union semun u;

  key = ftok(SHM_KEY_PATH, PROJ_ID);
  if (key == -1) {
    perror("ftok");
    return 1;
  }

  shmid = shmget(key, sizeof(struct SharedData), IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("shmget");
    return 1;
  }

  data = (struct SharedData *)shmat(shmid, NULL, 0);
  if (data == (struct SharedData *)-1) {
    perror("shmat");
    return 1;
  }

  data->in = 0;
  data->out = 0;

  semid = semget(key, 3, IPC_CREAT | 0666);
  if (semid == -1) {
    perror("semget");
    return 1;
  }

  u.val = 1;
  semctl(semid, MUTEX, SETVAL, u);

  u.val = SIZE;
  semctl(semid, EMPTY, SETVAL, u);

  u.val = 0;
  semctl(semid, FULL, SETVAL, u);

  for (i = 0; i < 10; i++) {
    int num = i + 1;

    wait_sem(semid, EMPTY);
    wait_sem(semid, MUTEX);

    data->buffer[data->in] = num;
    printf("Producer produced: %d\n", num);
    data->in = (data->in + 1) % SIZE;

    signal_sem(semid, MUTEX);
    signal_sem(semid, FULL);
    
    //sleep(1);
  }

  shmdt(data);
  printf("Producer complete. Resources left active for the consumer.\n");

  return 0;
}
