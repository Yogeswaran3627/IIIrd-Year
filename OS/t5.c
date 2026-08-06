#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main() 
{
    pid_t pid = fork();
    if (pid == 0)
    {             
       printf("P3: PID=%d Parent=%d\n", getpid(), getppid());
       pid = fork();
       if (pid == 0)
       {
	  printf("P5: PID=%d Parent=%d\n", getpid(), getppid());
          pid = fork();
          if (pid == 0)
             printf("P8: PID=%d Parent=%d\n", getpid(), getppid());
          else{
	     pid = fork();
	     if(pid == 0){
             printf("P7: PID=%d Parent=%d\n", getpid(), getppid());
	     }
	     else wait(NULL);
	  }
       }
       else wait(NULL);
    }
    else if (pid > 0)
    {            
       printf("P1: PID=%d Parent=%d\n", getpid(), getppid());
       pid = fork();
       if (pid == 0)
       {
	  printf("P2: PID=%d Parent=%d\n", getpid(), getppid());
	  pid = fork();
	  if (pid == 0)
          {
	     printf("P4: PID=%d Parent=%d\n", getpid(), getppid());
             pid = fork();
             if (pid == 0)
                printf("P6: PID=%d Parent=%d\n", getpid(), getppid()); 
             else wait(NULL);
          }
	  else wait(NULL);
       }
       else wait(NULL);
    }
    else wait(NULL);
    return 0;
}
