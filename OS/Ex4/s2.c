#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

struct Data
{
    int n;
    int a[100];
    int sum[4];
};

int main()
{
    key_t key = ftok("shared_file.txt", 'A');
    if (key == -1)
    {
        perror("Error: ftok failed");
        exit(1);
    }

    int shmid = shmget(key, sizeof(struct Data), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("Error: shmget failed");
        exit(1);
    }

    struct Data *data = (struct Data *)shmat(shmid, NULL, 0);
    if (data == (void *)-1)
    {
        perror("Error: shmat failed");
        exit(1);
    }

    printf("P1: Enter n: ");
    if (scanf("%d", &data->n) != 1)
    {
        perror("Error: Invalid input for n");
        exit(1);
    }

    if (data->n % 4 != 0)
    {
        printf("n must be divisible by 4.\n");

        if (shmdt(data) == -1)
        {
            perror("Error: shmdt failed");
            exit(1);
        }
        if (shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            perror("Error: shmctl (IPC_RMID) failed");
            exit(1);
        }

        return 0;
    }

    printf("P1: Enter %d numbers:\n", data->n);

    for (int i = 0; i < data->n; i++)
    {
        if (scanf("%d", &data->a[i]) != 1)
        {
            perror("Error: Invalid input for numbers");
            exit(1);
        }
    }

    int part = data->n / 4;

    data->sum[0] = 0;

    for (int i = 0; i < part; i++)
    {
        data->sum[0] += data->a[i];
    }

    printf("P1: Sum of first %d elements = %d\n",
           part, data->sum[0]);

    pid_t p2 = fork();
    if (p2 == -1)
    {
        perror("Error: fork p2 failed");
        exit(1);
    }

    if (p2 == 0)
    {
        data->sum[1] = 0;

        for (int i = part; i < 2 * part; i++)
        {
            data->sum[1] += data->a[i];
        }

        printf("P2: Sum of next %d elements = %d\n",
               part, data->sum[1]);

        pid_t p3 = fork();
        if (p3 == -1)
        {
            perror("Error: fork p3 failed");
            exit(1);
        }

        if (p3 == 0)
        {
            data->sum[2] = 0;

            for (int i = 2 * part; i < 3 * part; i++)
            {
                data->sum[2] += data->a[i];
            }

            printf("P3: Sum of next %d elements = %d\n",
                   part, data->sum[2]);

            pid_t p4 = fork();
            if (p4 == -1)
            {
                perror("Error: fork p4 failed");
                exit(1);
            }

            if (p4 == 0)
            {
                data->sum[3] = 0;

                for (int i = 3 * part; i < 4 * part; i++)
                {
                    data->sum[3] += data->a[i];
                }

                printf("P4: Sum of last %d elements = %d\n",
                       part, data->sum[3]);

                if (shmdt(data) == -1)
                {
                    perror("Error: shmdt failed in p4");
                    exit(1);
                }

                return 0;
            }
            else
            {
                if (wait(NULL) == -1)
                {
                    perror("Error: wait failed in p3");
                    exit(1);
                }

                if (shmdt(data) == -1)
                {
                    perror("Error: shmdt failed in p3");
                    exit(1);
                }

                return 0;
            }
        }
        else
        {
            if (wait(NULL) == -1)
            {
                perror("Error: wait failed in p2");
                exit(1);
            }

            if (shmdt(data) == -1)
            {
                perror("Error: shmdt failed in p2");
                exit(1);
            }

            return 0;
        }
    }
    else
    {
        if (wait(NULL) == -1)
        {
            perror("Error: wait failed in p1");
            exit(1);
        }

        printf("\nP1: Results received from all processes\n");

        printf("P1: Sum from P1 = %d\n", data->sum[0]);
        printf("P1: Sum from P2 = %d\n", data->sum[1]);
        printf("P1: Sum from P3 = %d\n", data->sum[2]);
        printf("P1: Sum from P4 = %d\n", data->sum[3]);

        int total = data->sum[0]
                  + data->sum[1]
                  + data->sum[2]
                  + data->sum[3];

        printf("P1: Total Sum = %d\n", total);

        if (shmdt(data) == -1)
        {
            perror("Error: shmdt failed in p1");
            exit(1);
        }

        if (shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            perror("Error: shmctl (IPC_RMID) failed");
            exit(1);
        }
    }

    return 0;
}
