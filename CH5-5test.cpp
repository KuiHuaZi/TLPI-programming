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
	if(argc!=2)
	{
		printf("useage:%s file <newfd>\n",argv[0]);
	}
	oldfd = open(argv[2],oldflag);
	if(oldfd == -1)
	{
		errExit("open");
	}
	printf("open success:oldfd:%d\n",oldfd);
	//newfd = get
}
