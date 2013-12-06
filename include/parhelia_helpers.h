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
	
};



#endif
