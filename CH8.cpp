/*
 * CH8.cpp
 *
 *  Created on: May 17, 2017
 *      Author: amapola
 *      Implement getpwnam() using setpwent(), getpwent(), and endpwent().
 */
using namespace std;
struct passwd *mygetpwnam(const char *name)
{
	struct passwd *pw;
	string sname(name);
	while((pw=getpwent())!=0)
	{
		if(sname.compare(pw->pw_name)==0)
		{
			endpwent();
			return pw;
		}
	}
	endpwent();
	return 0;
}


