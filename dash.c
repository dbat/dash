#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef IDE
//#pragma comment(lib, "minicrt.lib")
#endif

#include <windows.h>
//#include <locale.h>
#include <time.h>
//#include <stdio.h>
//#include <stdint.h>
#include "getoptn.h"
#include "escapars.h"
#include "stoint64.h"
#undef UNICODE
#undef _UNICODE
//#include "libctiny.h"

#pragma comment(lib, "bmh.lib")
#pragma warning(disable : 4996)

extern void __stdcall init_bmtable(const BYTE *text, const int len, int * const shift_table);
extern void __stdcall init_bmtable_nc(const BYTE *text, const int len, int * const shift_table);

extern BYTE* __stdcall textpos(const BYTE *data, const int datasize, const BYTE *text, int textlen, int const * const shift_table);
extern BYTE* __stdcall textpos_nc(const BYTE *data, const int datasize, const BYTE *text, int textlen, int const * const shift_table);

extern BYTE* __stdcall memem(const BYTE * data, const unsigned int datasize, const BYTE *text, const int textlen);
extern BYTE* __stdcall memem_nc(const BYTE * data, const unsigned int datasize, const BYTE *text, const int textlen);

BYTE* (__stdcall *funmem)(const BYTE*, const int);
BYTE* (__stdcall *funpos)(const BYTE*, const int);
void  (__stdcall *initbm)(const BYTE*, const int);

#define TXBUFSIZE 4096
BYTE txBuf[TXBUFSIZE] = {0}; // for now, put them all global, fixed size, unstructured
BYTE rpBuf[TXBUFSIZE] = {0}; // for now, put them all global, fixed size, unstructured
int bmTable[256]  = {0};
int txBufLen = 0, rpBufLen = 0;
int vxSize = 0;  // V1BLOCK + txBufLen -1
DWORD options = 0;

BYTE* __stdcall bympos(const BYTE *data, const int datasize) { return textpos(data, datasize, txBuf, txBufLen, bmTable); }
BYTE* __stdcall bympos_nc(const BYTE *data, const int datasize) { return textpos_nc(data, datasize, txBuf, txBufLen, bmTable); }

BYTE* __stdcall mempos(const BYTE *data, const int datasize) { return memem(data, datasize, txBuf, txBufLen); }
BYTE* __stdcall mempos_nc(const BYTE *data, const int datasize) { return memem_nc(data, datasize, txBuf, txBufLen); }

void  __stdcall bmx(const BYTE *text, const int len) { init_bmtable(text, len, bmTable); }
void  __stdcall bmx_nc(const BYTE *text, const int len) { init_bmtable_nc(text, len, bmTable); }

int _showLastErr_NULL(_TCHAR *msg, ...)
// CAUTION: the last given argument MUST BE NULL!!!
{
	_TCHAR *lem, *s;
	va_list args;
	unsigned err;
	err = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lem, 0, NULL);

	//----printf(_T("ERROR: %s.\nLast error code:%d\n%s"), msg, err, lem);
	//----no need to show the last error string

	printf(_T("\nERROR[%d]"), err);

	// error 0 = success,
	// error 183 = file already exists
	if (err != 183 && err != 0) printf(_T(" %s"), lem);

	LocalFree(lem);
	if(msg)	{ // printf(_T("ERROR[%d] %s"), err, msg);
		va_start(args, msg);
		printf(_T("%s"), msg);
		s = va_arg(args, TCHAR*);
		while(s) {
			printf(_T("%s"), s);
			s = va_arg(args, TCHAR*);
		}
		va_end(args);
		//printf(_T("\n"));
	}
	return 0;
}
#define showLastErr(a) _showLastErr_NULL(a, NULL)

int printer0(HANDLE h, const char* action, const char* object) {
	if (h) CloseHandle(h);
	printf("%s %s \"%s\"\n", "ERROR!", action, object);
	return 0;
}
#define printerr(a, b) printer0(0, a, b)

#define HANDLE_ISOK(h) (h != INVALID_HANDLE_VALUE)
#define HANDLE_ISBAD(h) (h == INVALID_HANDLE_VALUE)

int hCloserr(HANDLE fh, char *msg) {
	_showLastErr_NULL(msg, NULL);
	CloseHandle(fh);
	return 0;
}

int hmCloserr(HANDLE *fh, HANDLE *mh, char *msg) {
	_showLastErr_NULL(msg, NULL);
	if (*mh) { CloseHandle(*mh); *mh = NULL; }
	if HANDLE_ISOK(*fh) { CloseHandle(*fh); *fh = INVALID_HANDLE_VALUE; }
	return 0;
}

#define OPT_ONE 1
#define OPT_ALL 2
#define OPT_ICS 4
#define OPT_HEX 8
#define OPT_TRN 0x10
#define OPT_PRG 0x20
#define OPT_MEM 0x40
#define OPT_SEA 0x100
#define OPT_REP 0x200
#define OPT_CFM 0x400

#define OPC_ONE  '1'
#define OPC_ZRO  '0'
#define OPC_ALL  'a'
#define OPC_1ST  'f'
#define OPC_ICS  'i'
#define OPC_CSN  'c'
#define OPC_HEX  'x'
#define OPC_DEC  'd'
#define OPC_MEM  'm'
#define OPC_BYM  'b'
#define OPC_POS  'p'
#define OPC_LEN  'l'
#define OPC_TRN  't'
#define OPC_PRG  'g'
#define OPC_SEA  's'
#define OPC_REP  'R'
#define OPC_ESC  'e'
#define OPC_CFM  '!'

#define OPS_SWITCHES  "10aficxdmbp:l:tgs:R:e!"


#define ERR_OPEN   -1
#define ERR_OPEN2  -2
#define ERR_PAT    -3
#define ERR_EMPTY  -4
#define ERR_MAP    -5
#define ERR_VIEW   -6
#define ERR_STUPID -99

#define BUFSIZE     8192

#define SHIFT_1MB    20
#define SHIFT_1GB    30
#define SHIFT_1TB    40

#define SHIFT_16MB   24
#define SHIFT_64MB   26
#define SHIFT_256MB  28

#define SHIFTX       SHIFT_256MB

#define __64K        0x10000LL
#define __1MB        0x100000LL
#define __16MB       0x1000000LL
#define __256MB      0x10000000LL

#define __64MB       0x4000000LL
#define __128MB      0x8000000LL

#define __1GB        0x40000000LL
#define __1GB64K     0x40010000LL

#define XTAIL       __64K
#define V1BLOCK     ((__int64)1LL << SHIFTX)
#define V1MASK      (V1BLOCK - (__int64)1LL)

#define ceilup64k(n) ((((n - 1) >> 16) + 1) << 16)

void printspat() { printf("\n\n   [pattern] is string which can contain ESCAPED CAHARACTERS as below:"); }

int Help(TCHAR *arg0, int esc) {
	printf("\n Synopsys:");
	printf("\n   Get position of binary data in files (and replace with other data)");
	printf("\n");
	printf("\n Usage:");
	printf("\n   %s [ -switches ] [ search-pattern ] [ filemasks... ]", getbasename(arg0));

  if (!esc) {
	printf("\n");
	printf("\n   [switches] are as follows:");
	printf("\n     -%c|-%c  use 1-based pos (default is 0-based)", OPC_ONE, OPC_ZRO);
	printf("\n     -%c|-%c  get all pos (default is only to get the first match)", OPC_ALL, OPC_1ST);
	printf("\n     -%c|-%c  ignore case (default is case-sensitive)", OPC_ICS, OPC_CSN);
	printf("\n     -%c|-%c  use simple memory scan (default is using boyer-moore algorithm)", OPC_MEM, OPC_BYM);
	printf("\n     -%c|-%c  use hexadecimal offset (default is using decimal)", OPC_HEX, OPC_DEC);
	printf("\n");
	printf("\n     -%c NN  start from position NN", OPC_POS);
	printf("\n     -%c NN  length/size to be processed (do not scan further)", OPC_LEN);
	printf("\n");
	printf("\n            NN can be decimal, octal (0-prefix) or hexa (0x-prefix)");
	printf("\n");
	printf("\n     -%c     show translated pattern (default, if no filemask has given)", OPC_TRN);
	printf("\n     -%c     show progress and alert every 1GB scan increment on huge files", OPC_PRG);
	printf("\n     -%c     show Help about ESCAPED CHARACTERS", OPC_ESC);
	printf("\n");
	printf("\n     --     stop evaluating the next arguments as switches");
	printf("\n     -?     this Help");
	printf("\n");
	printf("\n     Search and REPLACE:");
	printf("\n     -%c PAT (lowcase-%c) search for this pattern:PAT instead", OPC_SEA, OPC_SEA);
	printf("\n     -%c PAT (CAPITAL-%c) replace with this pattern:PAT", OPC_REP, OPC_REP);
	printf("\n     -%c     CONFIRM replace, otherwise only show what would be done", OPC_CFM);
	printf("\n");
	printf("\n            if the search-pattern is specified explicitly with this switch");
	printf("\n            then the rest arguments will be considered as filemask instead");
	printf("\n");
	printf("\n            this pair of switches are built for convenience, so you can");
	printf("\n            specify the search pattern first before the replacement pattern");
	printf("\n");
	printf("\n            both patterns should yield an identical length, otherwise");
	printf("\n            - if replacement is shorter, it only write to its length");
	printf("\n            - if replacement is longer, the excess will be ignored");
	printf("\n");
	printf("\n      NOTE: succesful replace will NOT update the file's date time.");
	printspat();
	printf("\n");
	printf("\n   [filemask] can be a filename or wildcards (? and *, inc. dir portion)");
  }
  else {
	printspat();
	printf("\n");
	printf("\n   ESCAPED CHARACTERS:");
	printf("\n");
	printf("\n   1.Equivalent with printf:");
	printf("\n     \\0	null byte");
	printf("\n     \\a	alert");
	printf("\n     \\b	backspace");
	printf("\n     \\f	formfeed");
	printf("\n     \\n	newline/linefeed (0A)");
	printf("\n     \\r	carriage-return (0D)");
	printf("\n     \\t	TAB (horizontal)");
	printf("\n     \\v	vertical TAB");
    printf("\n");
	printf("\n   2.Extension:");
	printf("\n     \\x[N] (locase-x),  [N] is hex, max 2 digits represents byte");
	printf("\n     \\X[N] (CAPITAL-X), [N] is hex, max 8 digits represents dword");
	printf("\n     \\y[N] (locase-y),  [N] is hex, max 4 digits represents word");
	printf("\n     \\Y[N] (CAPITAL-Y), [N] is hex, arbitrary length represents dword ");
	printf("\n     \\d[N] (locase-d),  [N] is dec, arbitrary length represents byte (max=255)");
	printf("\n     \\D[N] (CAPITAL-D), [N] is dec, arbitrary length represents dword (max=4294967295)");
	printf("\n     (any invalid digits will terminate/conclude the interpretation)");
    printf("\n");
	//printf("\n   Note: numbers are stored using Intel's little-endian scheme");
    //printf("\n   ");
	printf("\n   3.Control characters:");
	printf("\n     \\c[C] where [C] is letter @A..Z[\\]^_ represents CTRL-CHAR 00..1F");
	printf("\n       \\c@ = 0 (NULL)");
	printf("\n       \\cA = 1 [CTRL-A]");
	printf("\n       \\cC = 3 [CTRL-C]");
	printf("\n       \\cG = 7 [CTRL-G] (beep)");
	printf("\n       \\cI = 9 [CTRL-I] (TAB)");
	printf("\n       \\cZ = 26 [CTRL-Z] (EOF)");
	printf("\n       ...");
	printf("\n       \\c^ = 30");
	printf("\n       \\c_ = 31");
	printf("\n     (any invalid/unknown chars will terminate the interpretation)");
  }
	printf("\n");
	return -1;
}

char *digitizer(const __int64 I, char *buf, char sep, int bloklen) {
	char tmb[32];
	int p = 0, q = sizeof tmb -1,
		n = sprintf(tmb, "%I64d", I);
	for (; n >=0; n--, q--, p++) {
		buf[q] = tmb[n];
		if (n && p && !(p % bloklen))
			buf[--q] = sep;
	}
	return buf + ++q;
}

char *decdigit(const __int64 I, char *buf) { return digitizer(I, buf, ',', 3); }
char *hexdigit(const __int64 I, char *buf) { return digitizer(I, buf, ':', 4); }

#define MyShareMode (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE)
#define MyOpenFlags (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN)

__int64 startPos = 0; // 0-based pos
__int64 stopPos1 = 0; // 1-based pos (length/size)

char *initView(char *filename, HANDLE *fh, HANDLE *mh, int *outLen, LARGE_INTEGER *outSize) {
	char tmb[32];
	LARGE_INTEGER sz, base;
	char *v = NULL;
	int vSize = 0;
	DWORD openMode, mapMode, viewMode;

	if (outLen) *outLen = 0;
	if (options & OPT_REP) {
		openMode = GENERIC_READ | GENERIC_WRITE;
		mapMode = PAGE_READWRITE;
		viewMode = FILE_MAP_WRITE;
	}
	else {
		openMode = GENERIC_READ;
		mapMode = PAGE_READONLY;
		viewMode = FILE_MAP_READ;
	}

	*fh = CreateFile(filename, openMode, MyShareMode, NULL, OPEN_EXISTING, MyOpenFlags, 0);
	if HANDLE_ISBAD(*fh) {
		return (char*)showLastErr("Can not open file");
	}

	sz.LowPart = GetFileSize(*fh, &sz.HighPart);
	if (!sz.QuadPart) {
		return (char*)hCloserr(*fh, "The file is empty");
	}

	printf(". Filesize: %s", decdigit(sz.QuadPart, tmb));

	if (stopPos1 && (stopPos1 < sz.QuadPart))
		sz.QuadPart = stopPos1;

	if (sz.QuadPart < startPos + txBufLen) {
		CloseHandle(*fh);
		printf("\n>>Will never found at start pos: %I64d (ignored)", startPos + (BOOL)(options & OPT_ONE));
		return NULL;
	}

	*mh = CreateFileMapping(*fh, NULL, mapMode, 0, 0, NULL);
	if (!*mh) {
		return (char*)hCloserr(*fh, "Can not create filemap");
	}

	if (sz.QuadPart <= vxSize) {
		v = MapViewOfFile(*mh, viewMode, 0, 0, vSize = sz.LowPart);
	}
	else {
		base.QuadPart = startPos & ~V1MASK;
		vSize = sz.QuadPart - startPos >= vxSize ? vxSize : startPos & V1MASK;
		v = MapViewOfFile(*mh, FILE_MAP_READ, base.HighPart, base.LowPart, vSize);
	}

	if (!v) {
		return (char*) hmCloserr(fh, mh, "Can not create file view");
	}

	if (outLen)
		*outLen = vSize;

	if (outSize)
		outSize->QuadPart = sz.QuadPart;

	return v;
}

#define printdot(Q) printf(Q & (__1GB-1LL) ? ".": "\x02\x07")

void *cycleView(void *v, HANDLE *fh, HANDLE *mh, LARGE_INTEGER *vbase, __int64 *remain, int *outLen) {
	#ifdef DEBUG3
	  printf("\n>>DEBUG: base=0x%0.10I64x remain=0x%0.10I64x len=%d", vbase->QuadPart, *remain, *outLen);
	#endif //DEBUG3

	if (*remain > vxSize)
		{ *outLen = vxSize; *remain -= V1BLOCK; }
	else
		{ *outLen = (int)*remain; *remain = 0; }

	UnmapViewOfFile(v);
	if (!v) {
		return (void*) hmCloserr(fh, mh, "Error unmapping");
	}

	vbase->QuadPart += V1BLOCK;
	v = MapViewOfFile(*mh, FILE_MAP_READ, vbase->HighPart, vbase->LowPart, *outLen);
	if (!v) {
		return (void*) hmCloserr(fh, mh, "Error while cycling view");
	}

	if (options & OPT_PRG)
		printdot(vbase->LowPart);

	#ifdef DEBUG3
	  printf("\n>>DEBUG: base=0x%0.10I64x remain=0x%0.10I64x len=%d\n", vbase->QuadPart, *remain, *outLen);
	#endif //DEBUG3

	return v;
}


int (*printN)(const __int64);
int printx(const __int64 x) { return printf(" %I64x", x); }
int printd(const __int64 d) { return printf(" %I64d", d); }
int printxx(const __int64 x) { char buf[32]; return printf(" %s", hexdigit(x, buf)); }
int printdx(const __int64 d) { char buf[32]; return printf(" %s", decdigit(d, buf)); }

void printPos(char *p, __int64 pos, int *Ctr) {
	int p1 = (BOOL)(options & OPT_ONE);
	if (Ctr) (*Ctr)++;
	printN(pos + p1);
	if (options & OPT_REP)
		if (options & OPT_CFM)
			memmove(p, rpBuf, rpBufLen);
		else
			printf("<write %d bytes at pos: 0x%I64x (%I64d)>\n", rpBufLen, pos+p1, pos+p1);
}

//#define DEBUG3
//#define DEBUG31

void printac(clock_t tac) {
	int hh, mm, ss;
	unsigned int ms;
	ms = (__int64)tac*1000LL/CLOCKS_PER_SEC;
	ss = ms / 1000;
	hh = ss / 3600;
	mm = (ss % 3600) / 60;
	ss %= 60;
	printf("%.02d:%0.2d:%0.2d.%.03d (%d ms)", hh, mm, ss, ms%1000, ms);
}

__int64 getpos(char *filename, int *Ctr) {
	HANDLE fh, mh;
	LARGE_INTEGER vBase, size;
	__int64 xpos, remain;
	char *p, *v;
	int k, len, pStart, pos = -1;
	clock_t tac = clock();

	if (!filename) return 0;

	v = initView(filename, &fh, &mh, &len, &size);
	if ((int)v <= 0) return (__int64)v;

	#ifdef DEBUG2
	  printf("\n- Success opening %d bytes view at %.08X", len, v);
	#endif //DEBUG2

	pStart = startPos & V1MASK;
	//p1 = (BOOL)(options & OPT_ONE);

	if (size.QuadPart <= vxSize) {
		p = funpos(v+pStart, len-pStart);
		if (!p)
			printf("\n>>Not found.");
		else {
			printf("\nFound at pos (%d-based):", options & OPT_ONE);
			if ((options & OPT_REP) && !(options & OPT_CFM))
				printf("\n");
			printPos(p, p - v, Ctr);
			if (options & OPT_ALL) {
				while (p) {
					if (p = funpos(p + txBufLen, len - (p + txBufLen - v))) {
						printPos(p, p - v, Ctr);
					}
				}
			}
		}
		xpos = pos;
	}
	else {
		remain = size.QuadPart - startPos - V1BLOCK;
		vBase.QuadPart = startPos & ~V1MASK;
		if ((p = funpos(v +pStart, len -pStart)) == NULL) {
			// findfirst
			while (!p && remain > 0) {
				#ifdef DEBUG3
				  printf("\n>>DEBUG: recycling view1: %d", ++*Ctr);
				#endif //DEBUG3
				if (v = cycleView(v, &fh, &mh, &vBase, &remain, &len))
					p = funpos(v, len);
				else
					return showLastErr("Can not do cycle1 view") || ERR_VIEW;
			}
		}
		if (!p) // still not found?
			printf("\nNot found");
		else {
			printf("\nFound at pos (%d-based):", options & OPT_ONE);
			if ((options & OPT_REP) && !(options & OPT_CFM))
				printf("\n");
			printPos(p, vBase.QuadPart + (p - v), Ctr);
			if (options & OPT_ALL) {
				while (remain >= 0) {
					if (p = funpos(p + txBufLen, len - (p + txBufLen - v))) {
						printPos(p, vBase.QuadPart + (p - v), Ctr);
					}
					else {
						if (!remain)
							break;
						#ifdef DEBUG3
						  printf("\n>>DEBUG: recycling view2: %d", ++*Ctr);
						#endif //DEBUG3
						if (v = cycleView(v, &fh, &mh, &vBase, &remain, &len))
							p = v - txBufLen; // rewind -1 to
						else
							return showLastErr("Can not do cycle2 view") || ERR_VIEW;
					}
				}
			}
		}
	}

	tac = clock() - tac;
	printf("\nDone in "); printac(tac);
	if (*Ctr && options & OPT_ALL)
		printf(", %d matches.", *Ctr);

	if (v) UnmapViewOfFile(v);
	if (mh) CloseHandle(mh);
	if HANDLE_ISOK(fh) CloseHandle(fh);

	return xpos + (BOOL)(options & OPT_ONE);
}

#define DOTDIR  0x2e
#define DOT2DIR 0x2e2e

int procmask(TCHAR *filemask) { //, char *text) {
	WIN32_FIND_DATA data;
	char *base, cPATH[MAX_PATH<<1];
	HANDLE fh;
	int cekidot, Ctr, CCtr = 0, more = 1;

	strcpy(cPATH, filemask);
	base = getbasename(cPATH);

	fh = FindFirstFileA(filemask, &data);
	if HANDLE_ISBAD(fh)
		return showLastErr(filemask);

	while(more) {
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			cekidot = ((int*)data.cFileName)[0];
			cekidot &= 0x0ffffff;
			if((cekidot != DOTDIR) && (cekidot != DOT2DIR))
				printf("\nDirectory skipped: %s", data.cFileName);
		}
		else {
			Ctr = 0; CCtr++;
			strcpy(base, data.cFileName);
			printf("\n");
			printf("\nProcessing file: %s", cPATH);
			getpos(cPATH, &Ctr);
		}
		more = FindNextFile(fh, &data);
	}
	FindClose(fh);
	return CCtr;
}


int errneg(char *sn) { return printf("\n>>ERROR! %s must be a postive number", sn) || ERR_STUPID; }

int main(int argn, TCHAR *args[]) {
	char c, ch;
	char tmb[32], *pattern = NULL, *replace = NULL;
	int i, pChanged = 0;

	//setlocale(LC_ALL, "");
	//setlocale(LC_NUMERIC, "");
	while ((c = getoptn(argn, args, _T(OPS_SWITCHES))) != -1) {
		ch = args[optindex-1][1];
		switch (c) {
			case OPC_ONE: options |= OPT_ONE; break;   // 1-based
			case OPC_ALL: options |= OPT_ALL; break;   // all matches
			case OPC_ICS: options |= OPT_ICS; break;   // ignore-case
			case OPC_HEX: options |= OPT_HEX; break;   // use hexadecimal
			case OPC_MEM: options |= OPT_MEM; break;   // boyer-moore algorithm

			case OPC_ZRO: options &= ~OPT_ONE; break;  // 0-based
			case OPC_1ST: options &= ~OPT_ALL; break;  // first match only
			case OPC_CSN: options &= ~OPT_ICS; break;  // case sensitive
			case OPC_DEC: options &= ~OPT_HEX; break;  // use decimal
			case OPC_BYM: options &= ~OPT_MEM; break;  // simple scan search

			case OPC_ESC: return Help(args[0], 1);

			case OPC_TRN: options |= OPT_TRN; break;
			case OPC_PRG: options |= OPT_PRG; break;

			case OPC_POS: startPos = stoint64(optarg, NULL); pChanged = 1; break;
			case OPC_LEN: stopPos1 = stoint64(optarg, NULL); break;

			case OPC_SEA: options |= OPT_SEA; pattern = optarg; break;
			case OPC_REP: options |= OPT_REP; replace = optarg; break;
			case OPC_CFM: options |= OPT_CFM; break;   // confirm replace

			case '?': if ((char)opterror == '?') return Help(args[0], 0);
			default: return printf("\n>>Unknown switch: -%c, index: %d\n", opterror, optindex-1);
		}
	}

	//DEBUG1 printf("\nargn:%d, optindex:%d", argn, optindex);

    // asm\0\cCah\t\p\Y23768281774\X6500063
	// "h\g\t \cF\\0\Y57043\c09\D9918435729\n\cH\0123\cCah\\t\Y23768281774\X6500063"
	// "\X4f055610\Xe0ecf7bd\Xc8d898dd\Xac6fb3f1"

	//DEBUG1 //printf("\ntxBufLen: %d", txBufLen);
	if ((argn < 2) && !txBufLen)
		return Help(args[0], 0);

	if (stopPos1 < 0)
		return errneg("length");

	if (startPos < 0)
		return errneg("start pos");

	if (stopPos1)
		printf("\nSize to scan: %s", decdigit(stopPos1, tmb));

	if (startPos) {
		printf("\nStart searching at pos: %s (%d-based)", decdigit(startPos, tmb), (options & OPT_ONE));
		startPos -= (BOOL) (options & OPT_ONE);
	}

	if (options & OPT_SEA) txBufLen = escaparse(pattern, txBuf, TXBUFSIZE, NULL);
	if (options & OPT_REP) rpBufLen = escaparse(replace, rpBuf, TXBUFSIZE, NULL);

	if(argn > optindex || pattern)  {
		if (!txBufLen) {
			pattern = args[optindex++];
			txBufLen = escaparse(pattern, txBuf, TXBUFSIZE, NULL);
		}
		if (txBufLen < 0) {
			printf("\n>>Not enough Buffer. Last pos: %d", -txBufLen);
			printf("\n");
			return ERR_PAT; // not enough buffer
		}
		if ((options & OPT_TRN) || (argn == optindex)) {
			printf("\nsearch-pattern(%d): %s", strlen(pattern), pattern);
			printf("\ntranslated(%d): ", txBufLen);
			for (i = 0; i < txBufLen; i++)
				printf("%.02x ", txBuf[i]);
			if ((options & OPT_REP) && replace) {
				printf("\n>>replace-pattern(%d): %s", strlen(replace), replace);
				printf("\n>>translated(%d): ", rpBufLen);
				for (i = 0; i < rpBufLen; i++)
					printf("%.02x ", rpBuf[i]);
			}
		}
		if(argn > optindex) {
			if (stopPos1 && stopPos1 - startPos -1 < txBufLen) {
				printf("\n>>Will never found. Size: %I64d, start at: %I64d, pattern: %d", stopPos1, startPos + (BOOL)(options & OPT_ONE), txBufLen);
				printf("\n");
				return ERR_STUPID;
			}
			vxSize = V1BLOCK + txBufLen -1;
			if (options & OPT_MEM) {
				if (options & OPT_ICS)
					funpos = mempos_nc;
				else
					funpos = mempos;
			}
			else {
				if (options & OPT_ICS)
					{ funpos = bympos_nc; initbm = bmx_nc; }
				else
					{ funpos = bympos; initbm = bmx; }
				initbm(txBuf, txBufLen);
			}
			printN = (options & OPT_HEX) ? printx : printd;
	
			//DEBUG1 printf("\ntxBufLen: %d, rpBufLen: %d", txBufLen, rpBufLen);
	
			if (rpBufLen > txBufLen) rpBufLen = txBufLen;
			if (txBufLen == rpBufLen && !memcmp(txBuf, rpBuf, txBufLen)) rpBufLen = 0;
			if (!rpBufLen) options &= ~OPT_REP;
	
			for (i = optindex; i < argn; i++) {
				printf("\n");
				printf("\nParsing filemask: %s", args[i]);
				procmask(args[i]);
			}
		}

	}
	else
		return Help(args[0], 0);

	//DEBUG1 printf("\n\nPress any key..."); getch();

	printf("\n");
	return 0;

}

                                                                 