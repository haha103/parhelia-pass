#include <iostream>

#include "parhelia_db.h"

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
	vector<ParheliaEntry> ret;
	string sql = _gen_sql_search_on_key(key);
	
	char * err_msg = 0;
	int rc = sqlite3_exec(_db_handle, sql.c_str(),
												_search_callback, &ret, &err_msg);

	if (rc != SQLITE_OK) {
		cerr << "SQL error: " << err_msg << endl;
	} else {
		cout << "SQL execution succeeded" << endl;
	}
	
	if (err_msg)
		sqlite3_free(err_msg);
	
	return ret;
}

vector<ParheliaEntry> ParheliaDB::gsearch(string key)
{
	vector<ParheliaEntry> ret;
	return ret;
}

int ParheliaDB::_search_callback(void * data, int ncol, char ** fields, char ** colnames)
{
	vector<ParheliaEntry> * p_data = static_cast<vector<ParheliaEntry> *>(data);
	ParheliaEntry entry;
	entry.key = fields[0];
	entry.username = fields[1];
	entry.password = fields[2];
	entry.category = fields[3];
	entry.comments = fields[4];
	p_data->push_back(entry);
	
	return 0;
}

int ParheliaDB::add(bool replace,
										const string & k,	const string & u,
										const string & p,	const string & cat,
										const string & com)
{
	// update db
	vector<ParheliaEntry> entries;
	string sql = _gen_sql_search_on_key(k);
	char * err_msg = 0;
	int rc = sqlite3_exec(_db_handle, sql.c_str(),
												_search_callback, &entries, &err_msg);
	if (rc != SQLITE_OK) {
		cerr << "SQL error: " << err_msg << endl;
	} else {
		cout << "SQL execution succeeded" << endl;
	}

	if (entries.size() > 0)
	{
		if (!replace)
			return DB_ERR_KEY_EXISTS;
		string sql = _gen_sql_update_on_key(k, u, p, cat, com);
		rc = sqlite3_exec(_db_handle, sql.c_str(), NULL, NULL, &err_msg);
		if (rc != SQLITE_OK) {
			cerr << "SQL error: " << err_msg << endl;
		} else {
			cout << "SQL execution succeeded" << endl;
		}	
	}
	else
	{
		string sql = _gen_sql_insert(k, u, p, cat, com);
		rc = sqlite3_exec(_db_handle, sql.c_str(), NULL, NULL, &err_msg);
		if (rc != SQLITE_OK) {
			cerr << "SQL error: " << err_msg << endl;
			cerr << "SQL statement: " << sql << endl;
		} else {
			cout << "SQL execution succeeded" << endl;
		}
	}
	
	if (err_msg)
		sqlite3_free(err_msg);
	
	// update cache
	// TODO
	
	return DB_SUCC;
}

string ParheliaDB::_gen_sql_update_on_key(const string & k, const string & u,
																					const string & p, const string & cat,
																					const string & com) const
{
	string sql = "UPDATE ";
	sql += DB_NAME_INTERNAL;
	sql += "SET ";
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

string ParheliaDB::_gen_sql_search_on_key(const string & key) const
{
	string sql = "SELECT * FROM ";
	sql += DB_NAME_INTERNAL;
	sql += " WHERE KEY LIKE '%";
  sql += key;
	sql += "%';";
	return sql;
}
