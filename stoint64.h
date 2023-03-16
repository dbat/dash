#ifndef HAVE_STOINT64_H
#define HAVE_STOINT64_H

__int64 atoint64(const char *digits);
/*
	Convert decimal digits to __int64

	digits: Null-terminated string to convert.
*/

__int64 stoint64(const char *digits, char **endptr);
/*
	Convert decimal, octal (prefixed by 0) and
 	hexadecimal digits (prefixed by 0x) to int64

	digits: Null-terminated string to convert.
	endptr: Pointer to character that stops scan.
*/

#endif // HAVE_STOINT64_H

