#ifndef _PARHELIA_ENTRY_
#define _PARHELIA_ENTRY_

#include <string>
#include <ostream>

namespace parhelia 
{
	using namespace std;
	class entry
	{
	public:

		friend ostream & operator<<(ostream & os, const entry & entry);
	
		entry();
		entry(string k, string uname, string upass, string cat, string com);

		void fmt_fields_width(size_t & k, size_t & u, size_t & p, size_t & cat, size_t & com) const;

		string key;
		string username;
		string password;
		string category;
		string comments;
	
	};
}



#endif
