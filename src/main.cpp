#include <iostream>

#include "parhelia_db.h"
#include "parhelia_opts.h"
#include "parhelia_config.h"
#include "parhelia_encrypt.h"

using namespace std;

int main(int argc, char ** argv) 
{
	ParheliaOpts opts(argc, argv);
	string input;	
	while (opts.db_passphrase.size() == 0) 
	{
		cout << "Please enter your non-empty password or type 'q' to quit: ";
		getline(cin, input);
		if (input == "q")
			return 1;
		opts.db_passphrase = input;
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
			vector<ParheliaEntry> entries = db.search(key);
			vector<ParheliaEntry>::const_iterator it = entries.begin();
			while (it != entries.end()) {
				cout << *it << endl;
				++it;
			}
		} else if (input == "a") {
			string k, u, p, cat, com;
			cout << "Key = ";
			getline(cin, k);
			cout << "User Name = ";
			getline(cin, u);
			cout << "Password = ";
			getline(cin, p);
			cout << "Category = ";
			getline(cin, cat);
			cout << "Comments = ";
			getline(cin, com);
			if (db.add(false, k, u, p, cat, com) == DB_SUCC) {
				cout << "OK" << endl;
			} else {
				cout << "FAILED" << endl;
			}
		} else if (input == "q") {
			break;
		} else {
			cout << ">> Invalid operation, please try again." << endl;
		}
	}
	
	return 0;
}
