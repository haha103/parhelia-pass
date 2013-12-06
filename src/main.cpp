#include <iostream>

#include "parhelia_db.h"
#include "parhelia_opts.h"
#include "parhelia_config.h"
#include "parhelia_encrypt.h"

using namespace std;

void get_all_fields_from_is(istream & is, string & k, string & u, string & p, string & cat, string & com)
{
	cout << "Key = ";
	getline(is, k);
	cout << "User Name = ";
	getline(is, u);
	cout << "Password = ";
	getline(is, p);
	cout << "Category = ";
	getline(is, cat);
	cout << "Comments = ";
	getline(is, com);
}

int main(int argc, char ** argv) 
{
	ParheliaOpts opts(argc, argv);
	string input;	
	while (opts.db_passphrase.size() == 0) 
	{
		cout << "Please enter your non-empty password or type 'q' to quit: ";
		opts.read_db_passphrase();
		cout << endl;
		if (opts.db_passphrase == "q")
			return 1;
	}

	ParheliaDB db(opts.db_file_name, opts.db_passphrase);

	while (1) {
		cout << endl << "===== Operation Menu =====" << endl
				 << " s - search" << endl
				 << " gs - global search" << endl
				 << " a - add new entry" << endl
				 << " e - edit existing entry" << endl
				 << " q - quit" << endl
				 << "==========================" << endl << endl
				 << ">> Select an operation: ";
				getline(cin, input);

		if (input == "s") {
			cout << ">> Enter a key for search: ";
			string key;
			getline(cin, key);
			cout << endl << endl;
			vector<ParheliaEntry> entries = db.search(key);
			vector<ParheliaEntry>::const_iterator it = entries.begin();
			while (it != entries.end()) {
				cout << *it << endl;
				++it;
			}
		} else if (input == "a") {
			string k, u, p, cat, com;
			get_all_fields_from_is(cin, k, u, p, cat, com);
			int rc = db.add(k, u, p, cat, com);
			if (rc == DB_ERR_KEY_EXISTS) {
				cout << ">> Key '" << k << "' already exists. Updated it? (y/n) ";
				getline(cin, input);
				if (input == "y")
					rc = db.update(k, u, p, cat, com);
			}
			cout << ">> " << (rc == DB_SUCC ? "OK" : "NOK") << endl;
		} else if (input == "e") {
			
		} else if (input == "q") {
			break;
		} else {
			cout << ">> Invalid operation, please try again." << endl;
		}
	}
	
	return 0;
}
