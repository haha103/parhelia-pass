#include "parhelia_entry.h"

namespace parhelia
{	
	entry::entry(string k, string uname, string upass, string cat, string com) : key(k), username(uname), password(upass), category(cat), comments(com) 
	{
	
	}

	entry::entry()
	{
	}

	ostream & operator<<(ostream & os, const entry & entry)
	{
		os << "-------- " << entry.key << " --------" << endl
			 << " user: " << entry.username << endl
			 << " pass: " << entry.password << endl
			 << " cate: " << entry.category << endl
			 << " comm: " << entry.comments << endl;
		return os;
	}

	void entry::fmt_fields_width(size_t & k, size_t & u, size_t & p, size_t & cat, size_t & com) const 
	{
		k = key.length();
		u = username.length();
		p = password.length();
		cat = category.length();
		com = comments.length();
	}
}
