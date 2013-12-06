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
