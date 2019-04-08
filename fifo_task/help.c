#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
enum CONSTOIDI
{
	SIZEBUF = 8, 
	RADIX = 10	
};
char * create_name(int pid);

int main()
{
	int pid = getpid();
	char * name_of_fifo = create_name(pid);	
	return name_of_fifo;
}
char * create_name(int pid)
{
	char *buff1 = (char*)calloc(SIZEBUF + 6, sizeof(char));
	char *buff2 = (char*)calloc(SIZEBUF, sizeof(char));
	int i = 0, j = 0;
	for(i = 0; i < SIZEBUF - 1; i++)
		{
			buff1[i] = '0';
		}
	sprintf(buff2, "%d", pid);
	printf("buff2 = %s\n", buff2);
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
