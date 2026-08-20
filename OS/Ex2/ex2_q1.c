#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    int num, temp;

    pipe(fd);

    if (fork() == 0)
    {
        printf("Child Process\n");
        printf("Enter a number: ");
        scanf("%d", &num);

        write(fd[1], &num, sizeof(num));
    }
    else
    {
        wait(NULL);

        read(fd[0], &num, sizeof(num));

        temp = num;

        while (temp > 1)
        {
            if (temp % 3 != 0)
                break;

            temp = temp / 3;
        }

        if (temp == 1){
            printf("\nParent: %d is a Power of 3.\n", num);
	    printf("\n");
	}
        else{
            printf("\nParent: %d is NOT a Power of 3.\n", num);
	    printf("\n");
	}
    }

    return 0;
}
