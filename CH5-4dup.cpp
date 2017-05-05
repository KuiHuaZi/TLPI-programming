/*
 * dup.c
 *
 *  Created on: May 4, 2017
 *      Author: amapola
 *      dup() achieve the System function dup()
 *      dup2() achiexe the System fuction dup2()
 */
#include "tlpi_hdr.h"
#include<limits.h>
#include<unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
static int isFDvalid(int);
int mydup(int oldfd)
{
	int newfd;
	if(isFDvalid(oldfd)==-1)
	{
		return -1;
	}
	else
	{
		newfd = fcntl(oldfd,F_DUPFD,0);
		return newfd;
	}
}
int mydup2(int oldfd,int newfd)
{
	if(isFDvalid(oldfd)==-1||newfd<0||newfd>(sysconf( _SC_OPEN_MAX)-1))
	{
		errno = EBADF;
		return -1;

	}
	if(oldfd == newfd)
	{
		return newfd;
	}
	if(isFDvalid(newfd)>0)
	{
		close(newfd);
	}
	int fd = fcntl(oldfd,F_DUPFD,newfd);
	return fd==newfd?fd:-1;

}
static int isFDvalid(int fd)
{
	int flag = fcntl(fd,F_GETFL);
	if(flag ==-1&&errno == EBADF)
	{
		return -1;

	}
	return 1;
}
int duptest()
{
	 char *txt = "hello, world! ";
	  const int       k_old_fd = STDOUT_FILENO, k_favor_fd = 3000;

	    int fd = mydup2(k_old_fd, k_favor_fd);
	    if (-1 == fd) {
	        printf( "ERROR! dup2New(). " );
	        exit(EXIT_FAILURE);
	    }

	    printf("the old descriptor %d,the new descriptor%d",k_old_fd ,fd );
	    write(fd, txt,strlen(txt));

	    return (EXIT_SUCCESS);
}

