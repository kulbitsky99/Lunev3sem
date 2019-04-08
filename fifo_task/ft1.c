#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
enum BASIC_CONSTANTS
{
	SIZEBUF = 8,	
	RADIX = 10,
	SIZENAME = 14,	
	PORTION = 25,
	WAITING_TIME = 5
};

char * Give_some_memory();
char * Create_name_of_fifo();
int Open_file_for_reading();
int Open_fifo_for_reading();
int Open_fifo_for_writing();
void Portion_reading_from_fifo();
void Portion_writing_to_fifo();
void Close_fds();


char name_of_fifo[] = "connection.fifo";

int main(int argc, char ** argv)
{

	if(argc == 2)
	{

		int OpenFlag = 0, fd = 0, pid = getpid(), conn_fd = 0;
		char * name_of_file = argv[1];
		/*char * name_of_fifo = Create_name_of_fifo(pid);
		printf("Name of future fifo = %s\n", name_of_fifo);

		conn_fd = Open_fifo_for_writing(conn_fd, conn_fifo);
		if((write(conn_fd, name_of_fifo, SIZENAME)) != SIZENAME)
		{
			printf("Can't push fifo's name into the buffer\n");
			exit(-1);
		}*/

//raise(SIGKILL);

		char * buff_to_read = Give_some_memory();//the beginning of exchanging data
		OpenFlag = Open_file_for_reading(OpenFlag, name_of_file);
		fd = Open_fifo_for_writing(fd, name_of_fifo);
		Portion_writing_to_fifo(fd, OpenFlag, buff_to_read);
		Close_fds(fd);
		Close_fds(OpenFlag);
		Close_fds(conn_fd);
		if((unlink(name_of_fifo)) < 0)
		   printf("UNLINK FAULT\n");

		free(buff_to_read);

	}
	else if(argc == 1)
	{
		int fd = 0, conn_fd = 0;
		/*char * name_of_fifo = (char *)calloc(SIZENAME, sizeof(char));

		conn_fd = Open_fifo_for_reading(conn_fd, conn_fifo);
		printf("conn_fd = %d\n", conn_fd);
		printf("SIZE = %ld\n", read(conn_fd, name_of_fifo, SIZENAME));
		printf("After reading name_of_fifo\n");
		   }*/

		char * buff_to_write = Give_some_memory();//the beginning of exchanging data
		fd = Open_fifo_for_reading(fd, name_of_fifo);
		printf("FD for reading = %d\n", fd);
		Portion_reading_from_fifo(fd, buff_to_write);
		Close_fds(fd);
		Close_fds(conn_fd);
		if((unlink(name_of_fifo)) < 0)
		   printf("UNLINK FAULT\n");

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
	char * buff_to_read = NULL;
	if((buff_to_read = (char *)calloc(PORTION, sizeof(char))) == NULL)
	{
		printf("Can't give memory!\n");
		exit(-1);
	}
	return buff_to_read;

}

char * Create_name_of_fifo(int pid)
{
	char *buff1 = (char*)calloc(SIZEBUF + 6, sizeof(char));
	char *buff2 = (char*)calloc(SIZEBUF, sizeof(char));
	int i = 0, j = 0;
	for(i = 0; i < SIZEBUF - 1; i++)
	{
		buff1[i] = '0';
	}
	sprintf(buff2, "%d", pid);
	int buff2_size = strlen(buff2);
	//itoa(pid, buff2, RADIX)
	for(i = SIZEBUF - buff2_size; i < SIZEBUF ; i++)
	{
		buff1[i] = buff2[j];
		j++;
	}
	buff1[SIZEBUF + 5] = '\0';
	strcat(buff1, ".fifo");
	return buff1;

}


int Open_file_for_reading(int OpenFlag, char *name_of_file)
{
	OpenFlag = open(name_of_file, O_RDONLY);
	if(OpenFlag < 0)
	{
		printf("Can't open a file\n");
		exit(EXIT_FAILURE);
	}
	return OpenFlag;
}

int Open_fifo_for_reading(int fd, char *name_of_fifo)
{

	if((fd = open(name_of_fifo, O_RDONLY | O_NDELAY)) < 0)
	{

		(void)umask(0);
		if(mkfifo(name_of_fifo, 0666) < 0)
		{
			printf("Can't create fifo\n");
			exit(EXIT_FAILURE);
		}


		if((fd = open(name_of_fifo, O_RDONLY | O_NDELAY)) < 0)
		{
			printf("Can't open fifo for writing\n");
			exit(EXIT_FAILURE);
		}
	}
	return fd;

}


int Open_fifo_for_writing(int fd, char *name_of_fifo)
{
	int i = 0;
	while((fd = open(name_of_fifo, O_WRONLY | O_NDELAY)) < 0)
	{
		sleep(1);
		if(i >= WAITING_TIME)
		{
			printf("Time of waiting ended. Please, try again.\n");
			exit(0);
		}
		i++;
	}	

/*	{
		(void)umask(0);
		if(mkfifo(name_of_fifo, 0666) < 0)   
		{
			printf("Can't create fifo\n");
			exit(EXIT_FAILURE);
		}

		if((fd = open(name_of_fifo, O_WRONLY)) < 0)
		{
			printf("Can't open fifo for writing\n");
			exit(EXIT_FAILURE);
		}
	}*/
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


void Portion_writing_to_fifo(int fd, int OpenFlag, char * buff_to_read)
{
	int size_of_data = 0;
	while( read(OpenFlag, buff_to_read, PORTION) > 0)
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

}
void Close_fds(int fd)
{
	if(close(fd) < 0)
	{
		printf("Can't close fifo's writing stream");
		exit(-1);
	}

}
