#ifndef _PARHELIA_ENTRY_
#define _PARHELIA_ENTRY_

#include <string>
#include <ostream>

using namespace std;

class ParheliaEntry
{
public:

	friend ostream & operator<<(ostream & os, const ParheliaEntry & entry);
	
	ParheliaEntry();
	ParheliaEntry(string k, string uname, string upass, string cat, string com);

	string key;
	string username;
	string password;
	string category;
	string comments;
	
};


#endif
