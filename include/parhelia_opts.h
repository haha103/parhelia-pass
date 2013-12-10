#ifndef _PARHELIA_OPTS_
#define _PARHELIA_OPTS_

#include <string>

namespace parhelia 
{
	using namespace std;
	class opts
	{
	public:
		opts(int argc, char ** argv);
		~opts();

		bool read_db_passphrase();
	
		string db_file_name;
		string db_passphrase;
	};
}

#endif
