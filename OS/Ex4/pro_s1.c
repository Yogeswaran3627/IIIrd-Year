// Producer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct Data
{
    char str;
    int ready;
    int done;
};

int main()
{
    key_t key = ftok("shared_file.txt", 'C');
    if (key == -1)
    {
        perror("Producer Error: ftok failed");
        exit(1);
    }

    int shmid = shmget(key, sizeof(struct Data), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("Producer Error: shmget failed");
        exit(1);
    }

    struct Data *data = (struct Data *)shmat(shmid, NULL, 0);
    if (data == (void *)-1)
    {
        perror("Producer Error: shmat failed");
        exit(1);
    }

    printf("Producer: Enter String: ");
    if (fgets(data->str, 100, stdin) == NULL)
    {
        perror("Producer Error: fgets failed");
        exit(1);
    }

    data->str[strcspn(data->str, "\n")] = '\0';

    printf("Producer: String stored in shared memory.\n");

    data->ready = 1;

    while (data->done == 0)
    {
    }

    printf("Producer: Reversed string received = %s\n", data->str);

    if (shmdt(data) == -1)
    {
        perror("Producer Error: shmdt failed");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("Producer Error: shmctl (IPC_RMID) failed");
        exit(1);
    }

    return 0;
}
