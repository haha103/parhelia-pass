#ifndef _PARHELIA_DB_
#define _PARHELIA_DB_

#include <string>
#include <set>
#include <map>
#include <vector>
#include <sqlite3.h>

#include "parhelia_entry.h"
#include "parhelia_config.h"
#include "parhelia_encrypt.h"

namespace parhelia 
{
	using namespace std;
	class db 
	{
	public:
		static void print_table(vector<entry> & entries);
		
		db();
		db(string db_file_name, string db_passphrase);
		~db();

		vector<entry> search(string key);  // search based on key only
		vector<entry> gsearch(string key); // global search on all fields
		vector<entry> cat_search(const string & cat) const;

		int add(const string & k,	const string & u,
						const string & p,	const string & cat,
						const string & com);
		int update(const string & k,	const string & u,
							 const string & p,	const string & cat,
							 const string & com);
		int del(const string & k);
	
	private:
		static void _print_sep(size_t k, size_t u, size_t p, size_t cat, size_t com);
		static int _search_callback(void * data, int ncol, char ** fields, char ** colnames);
		string _gen_sql_search_on_key(const string & key, bool exact_match = false) const;
		string _gen_sql_search_on_cat(const string & cat, bool exact_match = false) const;
		string _gen_sql_update_on_key(const string & k, const string & u,
																	const string & p, const string & cat,
																	const string & com) const;
		string _gen_sql_insert(const string & k, const string & u,
													 const string & p, const string & cat,
													 const string & com) const;
		string _gen_sql_delete(const string & k, bool exact_match = true) const;
		bool _key_exists(const string & k, bool exact_match = false);
	
		sqlite3 * _db_handle;
		string _db_passphrase;
		map<string, entry> _db_cache;
	};
	
}

#endif
