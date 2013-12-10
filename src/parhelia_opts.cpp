#include "parhelia_opts.h"
#include "parhelia_helpers.h"

namespace parhelia
{	
	opts::opts(int argc, char ** argv)
	{
		db_passphrase = "";
		db_file_name = "";
		if (argc >= 2)
			db_file_name = argv[1];
		if (argc >= 3)
			db_passphrase = argv[2];
	}

	opts::~opts() 
	{
		// nothing to be freed until now
	}

	bool opts::read_db_passphrase()
	{
		return helper::getline_pw_stdin(db_passphrase);
	}
}
