#include <iostream>

#include "parhelia_db.h"

void ParheliaDB::_print_sep(size_t k, size_t u, size_t p, size_t cat, size_t com)
{
	string sep("+");
	sep.append(k + 2, '-'); sep += "+";
	sep.append(u + 2, '-'); sep += "+";
	sep.append(p + 2, '-'); sep += "+";
	sep.append(cat + 2, '-'); sep += "+";
	sep.append(com + 2, '-'); sep += "+";
	cout << sep << endl;
}

void ParheliaDB::print_table(vector<ParheliaEntry> & entries)
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
			_print_sep(k, u, p, cat, com);
		
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
			_print_sep(k, u, p, cat, com);
	}	
}

ParheliaDB::ParheliaDB(string db_file_name, string db_passphrase)
	: _db_handle(0), _db_passphrase(db_passphrase)
{
	if(sqlite3_open(db_file_name.c_str(), &_db_handle) != SQLITE_OK)
	{
		cout << "DB connection failed ("
				 << db_file_name << "): "
				 << sqlite3_errmsg(_db_handle)
				 << endl;
		sqlite3_close(_db_handle);
		_db_handle = 0;
	}
}

ParheliaDB::~ParheliaDB() 
{
	if (_db_handle)
		sqlite3_close(_db_handle);
}


vector<ParheliaEntry> ParheliaDB::search(string key) 
{
	pair<vector<ParheliaEntry>, string> data;
	data.second = _db_passphrase;

	string sql = _gen_sql_search_on_key(key);
	
	char * err_msg = 0;
	int rc = sqlite3_exec(_db_handle, sql.c_str(),
												_search_callback, &data, &err_msg);

	if (rc != SQLITE_OK)
		cerr << "SQL error: " << err_msg << endl;
	
	if (err_msg)
		sqlite3_free(err_msg);
	
	return data.first;
}

vector<ParheliaEntry> ParheliaDB::gsearch(string key)
{
	vector<ParheliaEntry> ret;
	return ret;
}

int ParheliaDB::_search_callback(void * data, int ncol, char ** fields, char ** colnames)
{
	pair<vector<ParheliaEntry>, string> * p_data = static_cast<pair<vector<ParheliaEntry>, string> *>(data);
	ParheliaEntry entry;
	entry.key = fields[0];
	entry.username = fields[1];
	entry.password = ParheliaEncrypt::decrypt(p_data->second, fields[2]);
	entry.category = fields[3];
	entry.comments = fields[4];
	p_data->first.push_back(entry);
	
	return 0;
}

bool ParheliaDB::_key_exists(const string & k, bool exact_match) 
{
	pair<vector<ParheliaEntry>, string> data;
	data.second = _db_passphrase;

	string sql = _gen_sql_search_on_key(k, exact_match);
	
	char * err_msg = 0;
	int rc = sqlite3_exec(_db_handle, sql.c_str(),
												_search_callback, &data, &err_msg);

	if (rc != SQLITE_OK) {
		cerr << "SQL error: " << err_msg << endl;
	}
	
	if (err_msg)
		sqlite3_free(err_msg);
	
	return data.first.size() > 0;
}

int ParheliaDB::update(const string & k,	const string & u,
											 const string & p,	const string & cat,
											 const string & com)
{
	int ret = DB_SUCC;
	char * err_msg = 0;
	if (_key_exists(k, true))
	{		
		string sql = _gen_sql_update_on_key(k, u, ParheliaEncrypt::encrypt(_db_passphrase, p), cat, com);
		int rc = sqlite3_exec(_db_handle, sql.c_str(), NULL, NULL, &err_msg);
		if (rc != SQLITE_OK) {
			cerr << "SQL error: " << err_msg << endl;
			cerr << "SQL statement: " << sql << endl;
			ret = DB_SQL_FAIL;
		}
	}
	else
	{
		ret = DB_ERR_KEY_NOT_FOUND;
	}
	
	if (err_msg)
		sqlite3_free(err_msg);
	
	// update cache
	// TODO
	
	return ret;
}

int ParheliaDB::add(const string & k,	const string & u,
										const string & p,	const string & cat,
										const string & com)
{
	int ret = DB_SUCC;
	char * err_msg = 0;
	if (!_key_exists(k, true))
	{
		string sql = _gen_sql_insert(k, u, ParheliaEncrypt::encrypt(_db_passphrase, p), cat, com);
		int rc = sqlite3_exec(_db_handle, sql.c_str(), NULL, NULL, &err_msg);
		if (rc != SQLITE_OK) {
			cerr << "SQL error: " << err_msg << endl;
			cerr << "SQL statement: " << sql << endl;
			ret = DB_SQL_FAIL;
		}
	}
	else
	{
		ret = DB_ERR_KEY_EXISTS;
	}
	
	if (err_msg)
		sqlite3_free(err_msg);
	
	// update cache
	// TODO
	
	return ret;
}

int ParheliaDB::del(const string & k)
{
	int ret = DB_SUCC;
	char * err_msg = 0;
	if (_key_exists(k, true))
	{
		string sql = _gen_sql_delete(k);
		int rc = sqlite3_exec(_db_handle, sql.c_str(), NULL, NULL, &err_msg);
		if (rc != SQLITE_OK) {
			cerr << "SQL error: " << err_msg << endl;
			cerr << "SQL statement: " << sql << endl;
			ret = DB_SQL_FAIL;
		}
	}
	else
	{
		ret = DB_ERR_KEY_NOT_FOUND;
	}
	
	if (err_msg)
		sqlite3_free(err_msg);
	
	// update cache
	// TODO
	return ret;
}

string ParheliaDB::_gen_sql_update_on_key(const string & k, const string & u,
																					const string & p, const string & cat,
																					const string & com) const
{
	string sql = "UPDATE ";
	sql += DB_NAME_INTERNAL;
	sql += " SET ";
	sql += "username = '" + u + "', ";
	sql += "password = '" + p + "', ";
	sql += "category = '" + cat + "', ";
	sql += "comments = '" + com + "' ";
	sql += " WHERE key = '" + k + "';";
	return sql;
}

string ParheliaDB::_gen_sql_insert(const string & k, const string & u,
																	 const string & p, const string & cat,
																	 const string & com) const
{
	string sql = "INSERT INTO ";
	sql += DB_NAME_INTERNAL;
	sql += " VALUES (";
	sql += "'" + k + "', ";
	sql += "'" + u + "', ";
	sql += "'" + p + "', ";
	sql += "'" + cat + "', ";
	sql += "'" + com + "');";
	return sql;
}

string ParheliaDB::_gen_sql_search_on_key(const string & key, bool exact_match) const
{
	string sql = "SELECT * FROM ";
	sql += DB_NAME_INTERNAL;
	if (key == "") {
		sql += ";";
	} else {
		exact_match ? sql += " WHERE KEY = '" : sql += " WHERE KEY LIKE '%";
		sql += key;
		exact_match ? sql += "';" : sql += "%';";
	}
	return sql;
}

string ParheliaDB::_gen_sql_delete(const string & k, bool exact_match) const
{
	string sql = "DELETE FROM ";
	sql += DB_NAME_INTERNAL;
	exact_match ? sql += " WHERE KEY = '" : sql += " WHERE KEY LIKE '%";
	sql += k;
	exact_match ? sql += "';" : sql += "%';";
	return sql;
}
