#include "shared.h"

int main() {
  int shmid, semid, i;
  key_t key;
  struct SharedData *data;

  key = ftok(SHM_KEY_PATH, PROJ_ID);
  if (key == -1) {
    perror("ftok");
    return 1;
  }

  shmid = shmget(key, sizeof(struct SharedData), 0666);
  if (shmid == -1) {
    perror("shmget (Make sure producer is running first)");
    return 1;
  }

  data = (struct SharedData *)shmat(shmid, NULL, 0);
  if (data == (struct SharedData *)-1) {
    perror("shmat");
    return 1;
  }

  semid = semget(key, 3, 0666);
  if (semid == -1) {
    perror("semget");
    return 1;
  }

  for (i = 0; i < 10; i++) {
    int num;

    wait_sem(semid, FULL);
    wait_sem(semid, MUTEX);

    num = data->buffer[data->out];
    data->out = (data->out + 1) % SIZE;

    signal_sem(semid, MUTEX);
    signal_sem(semid, EMPTY);

    printf("Consumer read: %d\tSquare: %d\n", num, num * num);
  }

  shmdt(data);

  shmctl(shmid, IPC_RMID, NULL);
  semctl(semid, 0, IPC_RMID);
  printf("Consumer complete. System memory and semaphore resources cleaned up.\n");

  return 0;
}
