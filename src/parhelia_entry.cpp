#include "parhelia_entry.h"

ParheliaEntry::ParheliaEntry(string k, string uname, string upass, string cat, string com) : key(k), username(uname), password(upass), category(cat), comments(com) 
{
	
}

ParheliaEntry::ParheliaEntry()
{
}

ostream & operator<<(ostream & os, const ParheliaEntry & entry)
{
	os << "Key = " << entry.key << endl
		 << "User Name = " << entry.username << endl
		 << "Password = " << entry.password << endl
		 << "Category = " << entry.category << endl
		 << "Comments = " << entry.comments;
	return os;
}


