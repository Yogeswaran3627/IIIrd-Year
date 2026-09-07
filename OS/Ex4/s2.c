//Consumer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg
{
    long type;
    char text[100];
};

int main()
{
    key_t key = ftok("shared_file.txt", 10);
    if (key == -1)
    {
        perror("Consumer Error: ftok failed");
        exit(1);
    }
    
    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1)
    {
        perror("Consumer Error: msgget failed");
        exit(1);
    }

    struct msg m;

    printf("Consumer: Waiting for message...\n");
    
    if (msgrcv(msgid, &m, sizeof(m.text), 1, 0) == -1)
    {
        perror("Consumer Error: msgrcv failed");
        exit(1);
    }

    printf("Consumer: Message received = %s", m.text);

    int count = 0;
    for (int i = 0; m.text[i] != '\0'; i++)
    {
        if (m.text[i] == ' ')
            count++;
    }

    printf("Consumer: Words = %d\n", count + 1);

    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("Consumer Error: msgctl (IPC_RMID) failed");
        exit(1);
    }

    return 0;
}
