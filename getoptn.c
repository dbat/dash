#include "getoptn.h"

TCHAR *optarg = NULL;
int optindex = 0, opterror = 0;

int getoptn(int argc, TCHAR *argv[], TCHAR *optstring) {
	static TCHAR *next = NULL;
	TCHAR c, *cp;

	if (!optindex) next = NULL;
	optarg = NULL;

	opterror = 0;
	if (!next || !*next) {
		if (!optindex) optindex++;
		#define OP_NEQ(c) (argv[optindex][0] != _T(c))
		if ((optindex >= argc)
			#ifdef OPTN_ALSO_USE_SLASH
			|| (OP_NEQ('-') && OP_NEQ('/'))
			#else
			|| OP_NEQ('-')
			#endif
			|| !argv[optindex][1]) {
			optarg = NULL;
			if (optindex < argc) optarg = argv[optindex];
			return -1;
		}
		#undef OP_NEQ

		if (strcmp(argv[optindex], _T("--")) == 0) {
			optindex++;
			optarg = NULL;
			if (optindex < argc) optarg = argv[optindex];
			return -1;
		}

		next = argv[optindex++];
		next++;
	}

	c = *next++;
	cp = strchr(optstring, c);

	if (!cp || c == _T(':')) {
		opterror = c;
		return _T('?');
	};

	cp++;
	if (*cp == _T(':')) {
		if (*next != _T('\0'))
			{ optarg = next; next = NULL; }
		else if (optindex < argc)
			optarg = argv[optindex++];
		else
			return _T('?');
	}

	return c;
}

TCHAR *getbasename(TCHAR *arg0) {
	TCHAR *p = arg0;
	if (p && *p) {
		while (*p++);
		while ((p > arg0) && (*--p != PATH_SEPARATOR));
		if (*p == PATH_SEPARATOR) p++;
	}
	return p;
}
