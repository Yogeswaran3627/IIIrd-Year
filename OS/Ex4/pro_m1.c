// Producer

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
        perror("Producer Error: ftok failed");
        exit(1);
    }
    
    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1)
    {
        perror("Producer Error: msgget failed");
        exit(1);
    }

    struct msg m;
    m.type = 1;

    printf("Producer: Enter Message: ");
    if (fgets(m.text, sizeof(m.text), stdin) == NULL)
    {
        perror("Producer Error: fgets failed");
        exit(1);
    }

    printf("Producer: Sending message...\n");
    if (msgsnd(msgid, &m, sizeof(m.text), 0) == -1)
    {
        perror("Producer Error: msgsnd failed");
        exit(1);
    }
    printf("Producer: Message sent successfully.\n");

    return 0;
}
