//Consumer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

struct Data
{
    char str[100];
    int ready;
    int done;
};

int main()
{
    key_t key = ftok("shared_file.txt", 'C');
    if (key == -1)
    {
        perror("Consumer Error: ftok failed");
        exit(1);
    }

    int shmid = shmget(key, sizeof(struct Data), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("Consumer Error: shmget failed");
        exit(1);
    }

    struct Data *data = (struct Data *)shmat(shmid, NULL, 0);
    if (data == (void *)-1)
    {
        perror("Consumer Error: shmat failed");
        exit(1);
    }

    printf("Consumer: Waiting for string...\n");

    while (data->ready == 0)
    {
    }

    printf("Consumer: String received = %s\n", data->str);

    int n = strlen(data->str);

    for (int i = 0; i < n / 2; i++)
    {
        char temp = data->str[i];
        data->str[i] = data->str[n - i - 1];
        data->str[n - i - 1] = temp;
    }

    printf("Consumer: Reverse String = %s\n", data->str);

    data->done = 1;

    if (shmdt(data) == -1)
    {
        perror("Consumer Error: shmdt failed");
        exit(1);
    }

    return 0;
}
