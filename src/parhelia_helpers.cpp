#include "parhelia_helpers.h"

bool ParheliaHelper::getline_pw_stdin(string & out)
{
#ifdef __GNUC__
	termios old_term;
	tcgetattr(STDIN_FILENO, &old_term);
	termios new_term = old_term;
	new_term.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
	getline(cin, out);
	tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
#endif
	return true;
}


void ParheliaHelper::get_all_fields_from_is(istream & is, string & k, string & u, string & p, string & cat, string & com)
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
