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

char * Give_some_memory();
int Open_file_for_reading();
int Open_fifo_for_writing();
void Portion_Writing_to_fifo();
void CLose_File_Deskriptors();




int main(int argc, char ** argv)
{
	int size_of_data = 1, size_of_pipe_data = 1, size_buff = 1, OpenFlag = 0;
	char * buff_to_read = (char *)calloc(PORTION, sizeof(char));
	int fd = 0;

	OpenFlag = open(argv[1], O_RDONLY);
	if(OpenFlag < 0)
		{
			printf("Can't open a file\n");
			exit(EXIT_FAILURE);
		}



	
	
	(void)umask(0);
	if(mkfifo(argv[2], S_IFIFO | 0666) < 0)
	{
		printf("Can't create fifo\n");
		exit(EXIT_FAILURE);
	}
	

	if((fd = open(argv[2], O_WRONLY)) < 0)
	{
		printf("Can't open fifo for writing\n");
		exit(EXIT_FAILURE);
	}
//-------------------------------------------------------------

	while((size_buff = read(OpenFlag, buff_to_read, PORTION)) > 0)
	{

	size_of_data = write(fd, buff_to_read, PORTION);


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
