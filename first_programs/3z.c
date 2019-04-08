#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>


int main(int argc, char ** argv)
{
	char * name_pr = argv[1];
	char ** arg_pr = &argv[1];
	execv(name_pr, arg_pr);

	return 0;	
}
