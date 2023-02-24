#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <ctype.h>
//#include <string>

inline int alignBy4(int n) {
    return (n + 3) & ~3;
}

inline char *toLowerCase(char *string) {
    char *tptr = (char *)string;
    while(*tptr) {
        *tptr = tolower(*tptr);
        tptr++;
    }
    return string;
}

inline char* getAsHex(int *data, int length) {
    char *results = (char*)malloc(sizeof(char) * (length * 2 + 1));
    for (int i = 0; i < length; i++)
    {
        sprintf(results + i * 2, "%02x", data[i]);
    }
    results[length * 2] = '\0';
    // Dump should not be returned here, or should be set on the object so it can be properly freed
    return results;
}

#endif
