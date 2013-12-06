#include "parhelia_entry.h"

ParheliaEntry::ParheliaEntry(string k, string uname, string upass, string cat, string com) : key(k), username(uname), password(upass), category(cat), comments(com) 
{
	
}

ParheliaEntry::ParheliaEntry()
{
}

ostream & operator<<(ostream & os, const ParheliaEntry & entry)
{
	os << "-------- " << entry.key << " --------" << endl
		 << " user: " << entry.username << endl
		 << " pass: " << entry.password << endl
		 << " cate: " << entry.category << endl
		 << " comm: " << entry.comments << endl;
	return os;
}

void ParheliaEntry::fmt_fields_width(size_t & k, size_t & u, size_t & p, size_t & cat, size_t & com) const 
{
	k = key.length();
	u = username.length();
	p = password.length();
	cat = category.length();
	com = comments.length();
}


