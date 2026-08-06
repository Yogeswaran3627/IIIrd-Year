#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pipe1[2], pipe2[2];
    char str[100];
    char cstatus[200]; // Status variable for the Child Process
    char pstatus[200]; // Status variable for the Parent Process
    int i;

    pipe(pipe1);
    pipe(pipe2);

    if (fork() == 0)
    {
        int number = 0;
        int special = 0;

        read(pipe1[0], str, sizeof(str));

        for (i = 0; str[i] != '\0'; i++)
        {
            if (isdigit(str[i]))
                number = 1;

            if (!isalnum(str[i]))
                special = 1;
        }

        strcpy(cstatus, "");

        if (strlen(str) > 7 && number == 1 && special == 1)
        {
            strcpy(cstatus, "\nVALID: The entered string satisfies all the conditions.\n");
        }
        else
        {
            strcpy(cstatus, "\nINVALID:");

	    printf("\n");

            if (strlen(str) <= 7)
                strcat(cstatus, "\n- Length should be greater than 7.\n");

            if (number == 0)
                strcat(cstatus, "\n- At least one number is required.\n");

            if (special == 0)
                strcat(cstatus, "\n- At least one special character is required.\n");

	    printf("\n");
        }

        write(pipe2[1], cstatus, sizeof(cstatus));
    }
    else
    {
        printf("PARENT PROCESS\n");
        printf("Enter a string: ");
        scanf("%s", str);

        write(pipe1[1], str, sizeof(str));

        wait(NULL);

        read(pipe2[0], pstatus, sizeof(pstatus));

        printf("\nResult received from Child Process:\n");
        printf("%s\n", pstatus);
    }

    return 0;
}
