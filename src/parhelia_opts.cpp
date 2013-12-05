#include "parhelia_opts.h"

ParheliaOpts::ParheliaOpts(int argc, char ** argv)
{
	db_passphrase = "";
	db_file_name = "";
	if (argc >= 2)
		db_file_name = argv[1];
	if (argc >= 3)
		db_passphrase = argv[2];
}

ParheliaOpts::~ParheliaOpts() 
{
	// nothing to be freed until now
}
