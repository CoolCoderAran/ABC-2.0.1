#define _DEFAULT_SOURCE

#include <ctype.h>
#include "ascii.h"

Visible int chr_isascii(int c) {
	return isascii(c);
}
