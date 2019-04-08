#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char ** argv)
{
	
	char * str = argv[1];
	int n = atoi(str);
	if(errno != 0)
		perror("Mistake is: ");
	int i = 0, k = 0;
	for(i = 0; i < n; i++)
	{                
	pid_t number = fork();
	pid_t ID = getpid();
	pid_t ppid = getppid();
	printf("Pid of parent: %d ", ppid);
	wait(NULL);
	if(number == 0)
	{
	printf("Pid of child: %d ", ID );
	printf("Number of child: %d\n", k);
	break;
	}
	k++;
	}
	return 0;	
}
