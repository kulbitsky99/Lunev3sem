#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char ** argv)
{

	int i = 0, fd = 0;
	struct mymesg
	{
		long mtype;
		long number;
	}DATA;
	struct signal
	{
		long mtype;
	}SIGN;
	DATA.mtype = 1;
	SIGN.mtype = 2;

	if(argc == 2)
	{
		int n = atoi(argv[1]), PID = 0;
		if((fd = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666)) < 0)
		{
			printf("Can't create new message queue\n");
			exit(-1);
		}

		for(i = 0; i < n; i++)
		{
			PID = fork();
			if(PID == 0)
			{
                        	DATA.number = i;
				if((msgsnd(fd, &DATA, sizeof(long), 0)) < 0)
				{
					printf("Can't send type_1 msg to the queue\n");
					exit(-1);
				}
                                if((msgsnd(fd, &SIGN, 0, 0)) == 0)
				{
					//printf("After sending type_2 msg\n");
				}
				else
                                {
                                        printf("Can't send type_2 msg to the queue\n");
                                        exit(-1);
                                }

				exit(0);
			}
                        while(msgrcv(fd, &SIGN, 0 , SIGN.mtype, IPC_NOWAIT) < 0);
			//printf("After receiving type_2 msg\n");

		}
		
		for(i = 0; i < n; i++)
		{
			if((msgrcv(fd, &DATA, sizeof(long), DATA.mtype, 0)) < 0)
			{
				printf("Can't receive msg from the queue\n");
				exit(-1);
			}
			printf("%ld ", DATA.number);
		}

		if((msgctl(fd, IPC_RMID, NULL)) < 0)
		{
			printf("Can't close the message queue\n");
			exit(-1);
		}
	}
	return 0;


}
