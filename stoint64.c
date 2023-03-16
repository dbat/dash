#include "stoint64.h"

__int64 atoint64(const char *digits) {
    __int64 ret = 0;
	int n, c = 0;

	while ((c = *digits++) != '\0') {
		if (c >= '0' && c <= '9')
			n = (c - '0');
		else
			break;
		ret *= 10;
		ret += (__int64)n;
    }
    return ret;
}


__int64 stoint64(const char *digits, char **endptr)
{
	const char *s;
 	__int64 N, ret;
	int neg, flag, base = 0;
	char c;

	s = digits;
	do { c = *s++; }
		while (c == ' ' || c == '\t');

	if (c == '-')
		{ neg = 1; c = *s++; }
	else
		{ neg = 0; if (c == '+') c = *s++; }

	if (c == '0' && (*s == 'x' || *s == 'X'))
		{ c = s[1]; s += 2; base = 16; }
	else
		base = c == '0' ? 8 : 10;

	ret = flag = 0;

	N = 0;
	for (;;c = *s++) {
		c |= 0x20; // if isnotdghex(c) break;
		c -= '0';
		if (c < 0)
			break;
		if (c > 9)
			c -= '0' - 9;
		if (c < 0 || c >= base)
			break;
		N *= base;
		if(
			(flag < 0)
				|| (neg && (N > ret || (N -= c) > ret)) /* underflow */
				|| (N < ret || (N += c) < ret)          /* overflow */
			)
			{ flag = -1; break; }
		else
			{ ret = N; flag = 1; }
	}

	if (flag < 0)
		ret = neg ? 0x8000000000000000LL : 0x7fffffffffffffffLL;

	if ((int)endptr != 0)
		*endptr = (char *)(flag ? s - 1 : digits);

	return (ret);
}
