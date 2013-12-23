#include <iostream>

#include "parhelia_db.h"
#include "parhelia_opts.h"
#include "parhelia_config.h"
#include "parhelia_encrypt.h"
#include "parhelia_helpers.h"

using namespace std;
using namespace parhelia;

int main(int argc, char ** argv) 
{
	opts opts(argc, argv);
	string input;	
	while (opts.db_passphrase.size() == 0) 
	{
		cout << "Please enter your non-empty password or type 'q' to quit: ";
		opts.read_db_passphrase();
		cout << endl;
		if (opts.db_passphrase == "q")
			return 1;
	}

	db db(opts.db_file_name, opts.db_passphrase);

	while (1) {
		cout << endl << "===== Operation Menu =====" << endl
				 << " s - search" << endl
				 << " cs - category search" << endl
				 << " gs - global search" << endl
				 << " a - add new entry" << endl
				 << " e - edit existing entry" << endl
				 << " d - delete existing entry" << endl
				 << " l - list all entries" << endl
				 << " q - quit" << endl
				 << "==========================" << endl << endl
				 << "<< Select an operation: ";
				getline(cin, input);

		if (input == "s") {
			cout << "<< Enter a key for search: ";
			string key;
			getline(cin, key);
			vector<entry> entries = db.search(key);
			db::print_table(entries);
		} else if (input == "cs") {
			cout << "<< Enter a category keyword for search: ";
			string cat;
			getline(cin, cat);
			vector<entry> entries = db.cat_search(cat);
			db::print_table(entries);
		} else if (input == "a") {
			string k, u, p, cat, com;
			helper::get_all_fields_from_is(cin, k, u, p, cat, com);
			int rc = db.add(k, u, p, cat, com);
			if (rc == DB_ERR_KEY_EXISTS) {
				cout << ">> Key '" << k << "' already exists. Updated it? (y/n) ";
				getline(cin, input);
				if (input == "y")
					rc = db.update(k, u, p, cat, com);
			}
			cout << ">> " << (rc == DB_SUCC ? "OK" : ( rc == DB_NO_CHANGE ? "NO CHANGE" : "NOK")) << endl;
		} else if (input == "e") {
			string k, u, p, cat, com;
			helper::get_all_fields_from_is(cin, k, u, p, cat, com);
			int rc = db.update(k, u, p, cat, com);
			if (rc == DB_ERR_KEY_NOT_FOUND) {
				cout << ">> Key '" << k << "' not found. Add it? (y/n) ";
				getline(cin, input);
				if (input == "y")
					rc = db.add(k, u, p, cat, com);
			}
			cout << ">> " << (rc == DB_SUCC ? "OK" : ( rc == DB_NO_CHANGE ? "NO CHANGE" : "NOK")) << endl;
		} else if (input == "l") {
			vector<entry> entries = db.search("");
			db::print_table(entries);
		} else if (input == "d") {
			string key;
			cout << "<< Enter a key for deletion: ";
			getline(cin, key);
			int rc = db.del(key);
			if (rc == DB_ERR_KEY_NOT_FOUND)
				cout << ">> Key '" << key << "' not found. Skip deletion." << endl;
			cout << ">> " << (rc == DB_SUCC ? "OK" : ( rc == DB_NO_CHANGE ? "NO CHANGE" : "NOK")) << endl;
		} else if (input == "q") {
			break;
		} else {
			cout << ">> Invalid operation, please try again." << endl;
		}
	}
	
	return 0;
}
