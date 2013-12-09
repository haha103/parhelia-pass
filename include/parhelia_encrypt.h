#ifndef _PARHELIA_ENCRYPT_H_
#define _PARHELIA_ENCRYPT_H_

#include <bitset>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

namespace parhelia 
{
	using namespace std;
	class encryption 
	{
	public:
		static string encrypt(const string & passphrase, const string & unencrypted);
		static string decrypt(const string & passphrase, const string & encrypted);
	};
}

#endif
