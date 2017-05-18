/*
 * CH9.cpp
 *
 *  Created on: May 18, 2017
 *      Author: amapola
 *      Implement initgroups() using setgroups() and library functions
 */
#include<unistd.h>
#include<iostream>
#include<grp.h>
#include<string.h>
#include<errno.h>
#include<pwd.h>
using namespace std;
int myinitgroups(const char *name,gid_t gid)
{
	if(name == 0||name[0] =='\0'||name[0] == '\n'||gid<0)
		return -1;
	struct passwd *pwd;
	errno = 0;
	pwd = getpwnam(name);
	if(pwd == 0)
	{
		if(errno == 0)
			cerr<<"Not found the user:"<<name;
		else
			cerr<<"error"<<endl;
		return -1;
	}
	gid_t group[sysconf(_SC_NGROUPS_MAX)+1];
	group[0] = gid;
	size_t size = 1;
	struct group *tmp;
	while((tmp = getgrent())!=0)
	{
		for(int i =0;tmp->gr_mem[i]!=0;i++)
		{
			if(strcmp(name,tmp->gr_mem[i])==0)
			{
				group[size++] = tmp->gr_gid;
				break;
			}
		}

	}
	return setgroups(size,group);
}




