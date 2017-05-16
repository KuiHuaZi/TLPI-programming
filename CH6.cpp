/*
 * CH6.cpp
 *
 *  Created on: May 15, 2017
 *      Author: amapola
 *      call longjmpToFunctionReturned,the out is:before call function!
			In function
			initial setjmp()!
			function return !
			we jumped back from longjumpToFunctionReturned!
			In function
			initial setjmp()!
			Segmentation fault (core dumped)
 *
 */

#include "tlpi_hdr.h"
#include<setjmp.h>
#include<stdlib.h>
using namespace std;
static jmp_buf env;
void longjmpToFunctionReturned()
{
	cout<<"before call function!"<<endl;
	function();
	cout<<"function return !"<<endl;
	longjmp(env,1);
	cout<<"return !"<<endl;
	return;

}
void function()
{
	cout<<"In function"<<endl;
	if(setjmp(env)==0)
	{
		cout<<"initial setjmp()!"<<endl;
		return;
	}
	else
	{
		cout<<"we jumped back from longjumpToFunctionReturned!"<<endl;
		return;
	}

}
extern char **environ;
int mySetenv(const char *name,const char *value,int overwrite)
{
	char *env = getenv(name);

	if(env == 0)
	{
		char *newenv = (char*)malloc(strlen(name)+strlen(value)+1);
		if(newenv==0)return -1;
		strcat(newenv,name);
		strcat(newenv,"=");
		strcat(newenv,value);
		cout<<newenv<<endl;
		if(putenv(newenv)>0)
		{
			return -1;
		}

	}
	else{
		if(overwrite == 0)return 0;
		else{
			char *newenv = (char*)malloc(strlen(name)+strlen(value)+1);
			if(newenv == 0)return -1;
			strcat(newenv,name);
			strcat(newenv,"=");
			strcat(newenv,value);
			cout<<newenv<<endl;
			if(putenv(newenv)>0)
			{
				return -1;
			}
		}
	}
	return 0;
}
int myUnsetenv(const char *name)
{
	string senv(name);
	cout<<"myUnsetenv begin:"<<senv<<endl;
	char *env ;
	env = getenv(name);
	if(env ==0)
	{
		cout<<"end!"<<endl;
		return -1;

	}
	else{
		while(env!=0)
		{
			senv+="=";
			senv+=env;
			cout<<senv<<endl;
			char **ep;
			char **lastenv;
			char **find;
			for(ep = environ;*ep != 0;ep++)
			{
				if(senv == *ep)
				{
					find = ep;
				}
				lastenv = ep;
			}
			if(*lastenv != *find)
			{
				char *tmp = *find;
				*find = *lastenv;
				*lastenv = tmp;


			}
			cout<<*lastenv<<endl;
			//free(*lastenv);//not use free,because the env may not malloc,the enc can be static,
			//the problem is ,when env is malloc,we will not free it
			*lastenv = 0;
			env = getenv(name);
		}
	}
	cout<<"end!"<<endl;
	return 0;
}
//intput:"GREET=Guten Tag" SHELL=/bin/bash BYE=Ciao
//output will be:BYE=Ciao SHELL=/bin/bash
int CH6test(int argc,char*argv[])
{
	int j;
	    char **ep;

	    clearenv();         /* Erase entire environment */

	    /* Add any definitions specified on command line to environment */

	    for (j = 1; j < argc; j++)
	        if (putenv(argv[j]) != 0)
	            errExit("putenv: %s", argv[j]);

	    /* Add a definition for GREET if one does not already exist */

	    if (mySetenv("GREET", "Hello world", 1) == -1)
	        errExit("setenv");

	    /* Remove any existing definition of BYE */

	    myUnsetenv("GREET");

	    /* Display current environment */

	    for (ep = environ; *ep != NULL; ep++)
	        puts(*ep);

	    exit(EXIT_SUCCESS);
}
