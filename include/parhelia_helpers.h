#ifndef _PARHELIA_HELPERS_H_
#define _PARHELIA_HELPERS_H_

#include <iostream>
#include <string>

#ifdef __GNUC__

#include <termios.h>
#include <unistd.h>

#endif

using namespace std;

class ParheliaHelper 
{
public:
	static bool getline_pw_stdin(string & out);
	static void get_all_fields_from_is(istream & is, string & k, string & u, string & p, string & cat, string & com);
};



#endif
