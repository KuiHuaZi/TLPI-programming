/*
 * CH5-5test.c
 *
 *  Created on: May 4, 2017
 *      Author: amapola
 *
 */
#include"tlpi_hdr.h"
int CH5_5(int argc,char *argv[])
{
	int oldfd,dupfd;
	int oldflag,dupflag;
	off_t oldoff,newoff;
	oldflag = O_RDWR;
	if(argc!=3)
	{
		usageErr("useage:%s file <newfd>\n",argv[0]);

	}
	printf("%s\n",argv[1]);
	oldfd = open(argv[1],oldflag);
	if(oldfd == -1)
	{
		errExit("open");
	}
	printf("open success:oldfd:%d\n",oldfd);
	dupfd = getInt(argv[2],GN_ANY_BASE,argv[2]);
	if(dupfd == dup2(oldfd,dupfd))
	{
		dupflag = fcntl(dupfd,F_GETFL);
		oldoff = lseek(oldfd,SEEK_SET,100);
		newoff = lseek(dupfd,SEEK_SET,100);//get the offset now
		printf("oldfd:%d,oldflag:%d,oldoffset:%d;\ndupfd:%d,dupflag:%d,dupoffset:%d;\n",
				oldfd,oldflag,oldoff,dupfd,dupflag,newoff);
	}
	else{
		errExit("dup2");
	}
	if(close(oldfd)==-1)
	{
		errExit("close");
	}
	else//test if we close oldfd,what happend to newfd
	{
		dupflag = fcntl(dupfd,F_GETFL);
		if(dupflag==-1)
		{
			printf("newfd closed because oldfd close!\n");
			errExit("fcntl");
		}
		else
		{
			printf("newfd still exit\n");
			close(dupfd);
		}
	}
	return 1;
}
