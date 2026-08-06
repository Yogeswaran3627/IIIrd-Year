#include <stdio.h>
#include <unistd.h>
int main() 
{
    int n, i, sum = 0;
    pid_t pid;
    printf("Enter n: ");
    scanf("%d", &n);
    pid = fork();
    if (pid == 0){   
       for (i = 1; i <= n; i++)
	  sum += i;
       printf("Child Process\n");
       printf("Sum = %d\n", sum);
    }

    if (pid > 0){    
       printf("Parent Process\n");
       printf("Sequence: ");
       while (n != 1){
	  printf("%d ",n);
	  if (n % 2 == 0)
	     n = n / 2;
	  else
	     n = 3 * n + 1;
       }
       printf("1\n");
    }

    return 0;
}
