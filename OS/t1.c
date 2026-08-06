#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(){
   pid_t pid = fork();
   if (pid == 0){
      printf("P2:   PID = %d   Parent = %d\n", getpid(),getppid());
   }

   else if (pid > 0){
      printf("P1:   PID = %d   Parent = %d\n", getpid(), getppid());
   }

   else
      wait(NULL);

   return 0;
}
