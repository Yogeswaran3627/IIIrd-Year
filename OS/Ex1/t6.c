#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int n, i;

    printf("Enter the value of n: ");
    scanf("%d", &n);

    printf("P1 (PID=%d): Sum of first %d natural numbers = %d\n",
           getpid(), n, n * (n + 1) / 2);

    pid_t pid = fork();

    if (pid == 0) {
        int oddSum = 0;

        for (i = 1; i <= n; i += 2)
            oddSum += i;

        printf("P2 (PID=%d Parent=%d): Sum of odd numbers = %d\n",
               getpid(), getppid(), oddSum);
    } else {
        pid_t pid2 = fork();

        if (pid2 == 0) {
            int evenSum = 0;

            for (i = 2; i <= n; i += 2)
                evenSum += i;

            printf("P3 (PID=%d Parent=%d): Sum of even numbers = %d\n",
                   getpid(), getppid(), evenSum);
        } else {
            wait(NULL);
            wait(NULL);
        }
    }

    return 0;
}
