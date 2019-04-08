#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

enum CONSTOIDI
{
	PORTION = 25
};


int main(int argc, char ** argv)
{
	int size_of_data = 1, size_of_pipe_data = 1, size_buff = 1, OpenFlag = 0;
	char * buff_to_read = (char *)calloc(PORTION, sizeof(char));
	int fd = 0;

	//printf("argv[1] = %s, argv[2] = %s\n", argv[1], argv[2]);

	OpenFlag = open(argv[1], O_RDONLY);
	if(OpenFlag < 0)
		{
			printf("Can't open a file\n");
			exit(EXIT_FAILURE);
		}


	if((fd = open(argv[2], O_WRONLY)) < 0)
	{
		printf("Can't open fifo for writing\n");
		exit(EXIT_FAILURE);
	}
	//printf("FD = %d\n", fd);
//-------------------------------------------------------------

	//printf("Before write pid = %d\n", getpid());

	while((size_buff = read(OpenFlag, buff_to_read, PORTION)) > 0)
	{
        //printf("%s\n", buff_to_read);

	size_of_data = write(fd, buff_to_read, PORTION);

	//printf("After write!\n");


	if(size_of_data < 0)
		{
			printf("Can't write to fifo-pipe\n");
			exit(EXIT_FAILURE);
		}
	
	for(int i = 0; i < PORTION; i++)
	{
		buff_to_read[i] = 0;
	}
	}
	//printf("After write pid = %d\n", getpid());


	if(close(fd) < 0)
		{
			printf("Can't close fifo's writing stream");
			exit(-1);
		}	
	if(close(OpenFlag) < 0)
		{
			printf("Can't close file stream");
			exit(-1);
		}
	
	free(buff_to_read);
	return 0;	
}
