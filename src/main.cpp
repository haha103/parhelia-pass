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

void print_sep(size_t k, size_t u, size_t p, size_t cat, size_t com)
{
	string sep("+");
	sep.append(k + 2, '-'); sep += "+";
	sep.append(u + 2, '-'); sep += "+";
	sep.append(p + 2, '-'); sep += "+";
	sep.append(cat + 2, '-'); sep += "+";
	sep.append(com + 2, '-'); sep += "+";
	cout << sep << endl;
}

void print_table(vector<ParheliaEntry> & entries) 
{
	size_t k = 0, u = 0, p = 0, cat = 0, com = 0;
	ParheliaEntry h("Key", "User", "Password", "Category", "Comments");
	entries.insert(entries.begin(), h);
	vector<ParheliaEntry>::const_iterator it = entries.begin();
	// first pass to get the field length
	while (it != entries.end()) 
	{
		size_t k1, u1, p1, cat1, com1;
		it->fmt_fields_width(k1, u1, p1, cat1, com1);
		k = (k1 > k ? k1 : k);
		u = (u1 > u ? u1 : u);
		p = (p1 > p ? p1 : p);
		cat = (cat1 > cat ? cat1 : cat);
		com = (com1 > com ? com1 : com);
		++it;
	}
	// second pass for printing
	cout << endl;
	it = entries.begin();
	while (it != entries.end())
	{
		if (it == entries.begin() || it == entries.begin() + 1)
			print_sep(k, u, p, cat, com);
		
		string k1, u1, p1, cat1, com1;
		k1 = it->key; k1.resize(k, ' ');
		u1 = it->username; u1.resize(u, ' ');
		p1 = it->password; p1.resize(p, ' ');
		cat1 = it->category; cat1.resize(cat, ' ');
		com1 = it->comments; com1.resize(com, ' ');
		cout << "| " << k1
				 << " | " << u1
				 << " | " << p1
				 << " | " << cat1
				 << " | " << com1
				 << " |" << endl;
		++it;
		if (it == entries.end())
			print_sep(k, u, p, cat, com);
	}	
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
				 << " l - list all entries" << endl
				 << " q - quit" << endl
				 << "==========================" << endl << endl
				 << ">> Select an operation: ";
				getline(cin, input);

		if (input == "s") {
			cout << ">> Enter a key for search: ";
			string key;
			getline(cin, key);
			vector<ParheliaEntry> entries = db.search(key);
			print_table(entries);
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
			string k, u, p, cat, com;
			get_all_fields_from_is(cin, k, u, p, cat, com);
			int rc = db.update(k, u, p, cat, com);
			if (rc == DB_ERR_KEY_NOT_FOUND) {
				cout << ">> Key '" << k << "' not found. Add it? (y/n) ";
				getline(cin, input);
				if (input == "y")
					rc = db.add(k, u, p, cat, com);
			}
			cout << ">> " << (rc == DB_SUCC ? "OK" : "NOK") << endl;
		} else if (input == "l") {
			vector<ParheliaEntry> entries = db.search("");
			print_table(entries);
		} else if (input == "q") {
			break;
		} else {
			cout << ">> Invalid operation, please try again." << endl;
		}
	}
	
	return 0;
}
