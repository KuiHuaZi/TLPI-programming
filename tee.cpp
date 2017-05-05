/*

 * tee.c
 *
 *  Created on: 2017-4-27
 *      Author: amapola
 */
/*

 * tee.c
 *
 *  Created on: 2017-4-27
 *      Author: amapola
 */

/*
 * tee.cpp
 *
 *  Created on: 2017-4-27
 *      Author: amapola
 */
#include<sys/stat.h>
#include<fcntl.h>
#include"tlpi_hdr.h"
#include<unistd.h>
extern int optind,opterr,optopt;
extern char *optarg;
int tee(int argc,char*argv[])
{
	const int maxNumFd = 10;
	int infd = 0,openFlags,numfd=0;
	int outfd[maxNumFd];
	const int buffSize = 10;
	char buff[buffSize];
	mode_t filePerms;
	if(argc>1)
	{

		if( getopt(argc,argv,"a:")=='a')
		{
			printf("we have opt 'a',optind:%d,argc:%d\n",optind,argc);
			openFlags = O_APPEND|O_WRONLY|O_CREAT;
		}
		else
			openFlags = O_WRONLY|O_CREAT|O_TRUNC;
		filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;
		for(int i = optind-1;i<argc;++i)
		{
			if(numfd<maxNumFd)
			{
				printf("filename:%s\n",argv[i]);
				outfd[numfd] = open(argv[i],openFlags,filePerms);
				if(outfd[numfd] == -1)
					errExit("open");
				++numfd;
			}
			else
			{
				printf("The number of outfile our of range:%d",maxNumFd);
				exit(-1);
			}

		}
		ssize_t len;
		while((len = read(infd,buff,buffSize))>0)
		{
			int lastfd = numfd;
			while(lastfd>0)
			{
				ssize_t wlen = write(outfd[--lastfd],buff,len);
				if(wlen == -1)
					errExit("write");
			}
			ssize_t wlen= write(1,buff,len);
			if(wlen == -1)
			{
				errExit("write");
			}
		}
		if(len==-1)
		{
			errExit("read");
		}
	}
	else
	{
		ssize_t len;
		while((len =read(infd,buff,buffSize))>0)
		{
			ssize_t wlen= write(1,buff,len);
			if(wlen == -1)
			{
				errExit("write");
			}
		}
		if(len==-1)
			errExit("read");
	}
	while(numfd>0)
	{
		if(close(outfd[--numfd])==-1)
		{
			errExit("close");
		}
	}
	exit(EXIT_SUCCESS);

}









