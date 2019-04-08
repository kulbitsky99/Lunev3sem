#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
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
int Open_fifo_for_reading();
void Portion_reading_from_fifo();
void Close_fds();

int main(int argc, char ** argv)
{
	if(argc == 2)
	{
	char * buff_to_write = Give_some_memory();
	char * name_of_fifo = argv[1];
	int fd = 0;
	fd = Open_fifo_for_reading(fd, name_of_fifo);
	Portion_reading_from_fifo(fd, buff_to_write);
	Close_fds(fd);
	free(buff_to_write);	
	}
	else
	{
		printf("Invalid quantity of input parameters\n");
		exit(-1);
	}

	return 0;	
}






char * Give_some_memory()
{
	char * buff_to_write = (char *)calloc(PORTION, sizeof(char));
	return buff_to_write;
}

int Open_fifo_for_reading(int fd, char * name_of_fifo)
{
	if((fd = open(name_of_fifo, O_RDONLY)) < 0)
        {
                printf("Can't open fifo for reading");
                exit(-1);
        }
	return fd;

}

void Portion_reading_from_fifo(int fd, char * buff_to_write)
{
        while(read(fd, buff_to_write, PORTION) > 0)
        {
        printf("%s", buff_to_write);
        for(int i = 0; i < PORTION; i++)
        {
                buff_to_write[i] = 0;
        }
        }

}
void Close_fds(int fd)
{
        if(close(fd) < 0)
        {
                printf("Can't close fifo's reading stream");
                exit(-1);
        }
	
}
