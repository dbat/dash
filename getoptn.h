#ifndef GETOPTN_H
#define GETOPTN_H
#include <tchar.h>

#ifndef EOF
#define EOF _T('\26')
#endif //EOF

extern int optindex, opterror;
extern TCHAR *optarg;

#ifndef PATH_SEPARATOR
#define PATH_SEPARATOR _T('\\')
#endif //PATH_SEPARATOR

TCHAR *getbasename(TCHAR *arg0);

int getoptn(int argc, TCHAR *argv[], TCHAR *optstring);

#endif //GETOPTN_H
