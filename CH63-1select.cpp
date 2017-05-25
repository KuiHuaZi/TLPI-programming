/*
 * CH63-1select.cpp
 *
 *  Created on: May 24, 2017
 *      Author: amapola
 *      just test for select
 *
 */
#include"tlpi_hdr.h"
#include<sys/times.h>
#include<sys/select.h>
#include<fcntl.h>
#include<time.h>
#include<poll.h>
#define BUFSIZE 1024
static void
usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s {timeout|-} fd-num[rw]...\n", progName);
    fprintf(stderr, "    - means infinite timeout; \n");
    fprintf(stderr, "    r = monitor for read\n");
    fprintf(stderr, "    w = monitor for write\n\n");
    fprintf(stderr, "    e.g.: %s - 0rw 1w\n", progName);
    exit(EXIT_FAILURE);
}
void setfdnoblock(int fd)
{
	int flag = fcntl(fd,F_GETFL);
	if(flag==-1)
		errExit("fcntl");
	flag|=O_NONBLOCK;
	if(fcntl(fd,F_SETFL,flag)==-1)
		errExit("fcntl");

}
int selectmain(int argc,char *argv[])
{
	fd_set readfds,writefds;
	int ready,nfds,fd,numRead,j;
	struct timeval timeout;
	struct timeval *pto;
	char buf[10];
	if(argc<2||strcmp(argv[1],"--help")==0)
	{
		usageError(argv[0]);
	}
	if(strcmp(argv[1],"-")==0)
	{
		pto = NULL;
	}
	else
	{
		timeout.tv_sec = atoi(argv[1]);//getLong(argv[1],0,"timeout");
		timeout.tv_usec = 0;
		pto = &timeout;
	}
	nfds =0;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	for(j = 2;j<argc;j++)
	{
		numRead = sscanf(argv[j],"%d%2[rw]",&fd,buf);
		if(numRead!=2)
		{
			usageError(argv[0]);
		}
		if(fd>=FD_SETSIZE)
			cmdLineErr("file descriptor exceeds limit(%d)\n",FD_SETSIZE);
		if(fd>=nfds)
			nfds = fd+1;
		setfdnoblock(fd);
		if(strchr(buf,'r')!=NULL)
			FD_SET(fd,&readfds);
		if(strchr(buf,'w')!=NULL)
			FD_SET(fd,&writefds);
	}
	int num =10;
	while(num--)
	{
		fd_set treadfds = readfds,twritefds = writefds;
		struct timeval ttimeout = timeout;
		ready = select(nfds,&treadfds,&twritefds,NULL,(pto!=NULL)?&ttimeout:pto);
		if(ready == -1)
			errExit("select");
		printf("number of ready:%d\n",ready);
		for(fd=0;fd<nfds;fd++)
		{
			if(FD_ISSET(fd,&treadfds))
			{
				char buf[BUFSIZE];
				int numr = read(fd,&buf,BUFSIZE);
				buf[numr] ='\0';
				printf("%d read:%s\n",fd,buf);
			}
			if(FD_ISSET(fd,&twritefds))
			{
				char buf[7] ="hello!";
				printf("%d write:",fd);
				int numw = write(fd,&buf,7);
				printf("\n");

			}

		}
		if(pto!=NULL)
			printf("timeout after select:%ld.%03ld\n",(long)ttimeout.tv_sec,(long)ttimeout.tv_usec/10000);
	}
	exit(EXIT_SUCCESS);
}
/*just test for poll*/
int pollmain(int argc,char *argv[])
{
	nfds_t nfds;
	int numpipe,numwrite;
	int (*ppipe)[2];
	if(argc<2||strcmp(argv[1],"--help")==0)
	{
		printf("usage:%s num-pipes [num-writes]\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	numpipe = atoi(argv[1]);

	ppipe = (int(*)[2])calloc(numpipe,sizeof(int[2]));
	struct pollfd *fds;
	fds = (struct pollfd*)calloc(numpipe,sizeof(struct pollfd));
	if(ppipe ==NULL)
		errExit("calloc");
	for(int i =0;i<numpipe;i++)
	{
		if(pipe(ppipe[i])!=0)
			errExit("pipe %d",i);
	}
	numwrite = (argc>2)?atoi(argv[2]):1;
	srandom((int)time(NULL));
	for(int j =0;j<numwrite;j++)
	{
		int randPipe = random()%numpipe;
		printf("pipe %d to write (read fd:%d)\n",ppipe[randPipe][1],ppipe[randPipe][0]);
		if(write(ppipe[randPipe][1],"a",1)!=1)
		{
			errExit("write %d",ppipe[randPipe][1]);
		}

	}
	for(int k =0;k<numpipe;k++)
	{
		fds[k].fd = ppipe[k][0];
		fds[k].events = POLLIN;
		//setfdnoblock(fd);

	}
	nfds = numpipe;
	int ready = poll(fds,nfds,-1);
	if(ready==-1)
		errExit("poll");
	printf("number of ready:%d \n",ready);
	for(int i = 0;i<numpipe;i++)
	{
		if(fds[i].revents == POLLIN)
		{
			printf("Readables: %d\n",fds[i].fd);
			ready--;
			if(ready ==0)break;
		}
	}
	return 1;

}
