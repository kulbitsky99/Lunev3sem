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

	int fd = 0, fd2 = 0;
	long i = 0;
	struct mymesg
	{
		long mtype;

	};
	struct mymesg SIGN_CHILD;
	struct mymesg SIGN_PAR;

	struct numbers
	{
		long mtype;
		long number;
	}CH_NUM;

	SIGN_CHILD.mtype = 1;
	SIGN_PAR.mtype = 2; 

	if(atoi(argv[1]) == 0)
		exit(0);
	if(argc == 2)
	{
		int n = atoi(argv[1]), PID = getpid();
		if((fd = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666)) < 0)
		{
			printf("Can't create new message queue\n");
			exit(-1);
		}

                if((fd2 = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666)) < 0)
                {
                        printf("Can't create new message queue2\n");
                        exit(-1);
                }


		for(i = 0; i < n; i++)
		{
			PID = fork();
			if(PID != 0)
			{
			while(msgrcv(fd, &SIGN_CHILD, 0 , SIGN_CHILD.mtype, 0) < 0);
			}
			if(PID == 0)
			{
				CH_NUM.mtype = i + 3;
                                CH_NUM.number = i;
                                //printf("I'm the %ld child and my type is %ld\n", CH_NUM.number, CH_NUM.mtype);


				if((msgsnd(fd, &SIGN_CHILD, 0, 0)) == 0)
					break;
				else
				{
					printf("Can't send SIGN_CHILD msg to the queue\n");
					exit(-1);
				}
			}
		}
		if(PID != 0)
		{
                if((msgctl(fd, IPC_RMID, NULL)) < 0)
                {
                        printf("Can't close the message queue\n");
                        exit(-1);
                }

		/*struct numbers CH_NUM_FOR_PAR;
		CH_NUM_FOR_PAR.mtype = 2;
	        CH_NUM_FOR_PAR.number = 0;*/
			if((msgsnd(fd2, &SIGN_PAR, sizeof(long), 0)) < 0)
                                {
                                        printf("Can't send SIGN_PAR msg to the queue\n");
                                        exit(-1);
                                }
		}
		if(PID == 0)
		{

			
			msgrcv(fd2, &SIGN_CHILD, 0, CH_NUM.mtype - 1, MSG_NOERROR);
			fflush(stdout);
			printf("%ld ", CH_NUM.number);
			if(msgsnd(fd2, &CH_NUM, sizeof(long), 0) < 0)
				{
					printf("Can't push the msg about ending the process\n");
					exit(-1);
				}
			//printf("I was glad to exist with you, good bye!\n");
			exit(0);

		}
		printf("pid_parent = %d\n", getpid());
		while((msgrcv(fd2, &CH_NUM, 0, n + 2, MSG_NOERROR)) < 0);
		if((msgctl(fd2, IPC_RMID, NULL)) < 0)
		{
			printf("Can't close the message queue\n");
			exit(-1);
		}
	}
	return 0;


}
