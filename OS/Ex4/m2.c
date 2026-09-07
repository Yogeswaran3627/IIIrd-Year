#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

struct Student
{
    long type;
    char name[50];
    int roll;
    int marks[5];
    int total;
    float average;
    char grade;
};

int main()
{
    int j;
    key_t key = ftok("shared_file.txt", 'G');
    if (key == -1)
    {
        perror("Error: ftok failed");
        exit(1);
    }

    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1)
    {
        perror("Error: msgget failed");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Error: fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        struct Student s;

        if (msgrcv(msgid, &s, sizeof(struct Student) - sizeof(long), 1, 0) == -1)
        {
            perror("Consumer Error: msgrcv failed");
            exit(1);
        }

        printf("\n========== CONSUMER ==========\n");
        printf("\nGRADE SHEET\n");
        printf("--------------------------------------------------------------------------------\n");
        printf("Roll\tName\tS1\tS2\tS3\tS4\tS5\tTotal\tAverage\tGrade\n");
        printf("--------------------------------------------------------------------------------\n");

        printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\t%c\n",
               s.roll,
               s.name,
               s.marks[0],
               s.marks[1],
               s.marks[2],
               s.marks[3],
               s.marks[4],
               s.total,
               s.average,
               s.grade);
        
        printf("--------------------------------------------------------------------------------\n");
        return 0;
    }
    else
    {
        struct Student s;
        s.type = 1;

        printf("\n========== PRODUCER ==========\n");
        printf("\nEnter details of the Student\n");

        printf("Roll Number: ");
        if (scanf("%d", &s.roll) != 1)
        {
            perror("Producer Error: Invalid input for roll number");
            exit(1);
        }

        printf("Name: ");
        if (scanf("%s", s.name) != 1)
        {
            perror("Producer Error: Invalid input for name");
            exit(1);
        }

        printf("Enter marks in 5 subjects:\n");
        s.total = 0;

        for (j = 0; j < 5; j++)
        {
            printf("Subject %d: ", j + 1);
            if (scanf("%d", &s.marks[j]) != 1)
            {
                perror("Producer Error: Invalid input for marks");
                exit(1);
            }
            s.total += s.marks[j];
        }

        s.average = s.total / 5.0;

        if (s.average >= 90) s.grade = 'A';
        else if (s.average >= 80) s.grade = 'B';
        else if (s.average >= 70) s.grade = 'C';
        else if (s.average >= 60) s.grade = 'D';
        else if (s.average >= 50) s.grade = 'E';
        else s.grade = 'F';

        printf("\nProducer: Sending data to queue...\n");
        if (msgsnd(msgid, &s, sizeof(struct Student) - sizeof(long), 0) == -1)
        {
            perror("Producer Error: msgsnd failed");
            exit(1);
        }

        if (wait(NULL) == -1)
        {
            perror("Producer Error: wait failed");
            exit(1);
        }

        if (msgctl(msgid, IPC_RMID, NULL) == -1)
        {
            perror("Producer Error: msgctl (IPC_RMID) failed");
            exit(1);
        }
        printf("\nProducer: Message queue cleaned up and closed.\n");
    }

    return 0;
}
