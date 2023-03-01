#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <cstring>

inline int alignBy4(int n) {
    return (n + 3) & ~3;
}

inline char *toLowerCase(char *string) {
    char *ref_ptr = (char *)string;
    while(*ref_ptr) {
        *ref_ptr = tolower(*ref_ptr);
        ref_ptr++;
    }
    return string;
}

inline char *toUpperCase(char *string) {
    char *ref_ptr = (char *)string;
    while(*ref_ptr) {
        *ref_ptr = toupper(*ref_ptr);
        ref_ptr++;
    }
    return string;
}

inline char* getAsHex(unsigned char *data, int length) {
    char *results = (char*)malloc(sizeof(char) * (length * 2 + 1));
    for (int i = 0; i < length; i++)
    {
        sprintf(results + i * 2, "%02x", data[i]);
    }
    results[length * 2] = '\0';
    // TODO: Dump should not be returned here, or should be set on the object so it can be properly freed
    return results;
}

#endif
