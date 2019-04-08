#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>

int z = 0;

int main(int argc, char ** argv)
{
	char * str1 = argv[1];
	char * str2 = argv[2];
	int n = atoi(str1);
	int ch2 = atoi(str2);
	int i = 0, j = 0, k = 0;
	for(i = 0; i < n; i++)
	{                
	pid_t number = fork();
	pid_t ID = getpid();
	if(number == 0)
	{
	printf("Pid of child: %d ", ID );
	printf("Number of child: %d\n", k);
	break;
	}
	for(j = 0; j < ch2; j++)
		z += 1;
	k++;
	}
	wait(NULL);
	printf("%d ", z); 
	return 0;	
}
