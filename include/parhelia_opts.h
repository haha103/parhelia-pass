#ifndef _PARHELIA_OPTS_
#define _PARHELIA_OPTS_

#include <string>

using namespace std;

class ParheliaOpts
{
public:
	ParheliaOpts(int argc, char ** argv);
	~ParheliaOpts();
	
	string db_file_name;
	string db_passphrase;
};

#endif
