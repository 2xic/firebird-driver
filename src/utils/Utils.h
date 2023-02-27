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

// TODO: Replace this 
inline unsigned char ByteToAscii(const char *input) {
  char singleChar;
  unsigned char out;
  memcpy(&singleChar, input, 2);
  out = (int)strtol(&singleChar, NULL, 16);
  return out;
}

inline int HexStringToAscii(const char *input, unsigned int length) {
  int mIndex, sIndex = 0;
  unsigned char buffer[length];
  for (mIndex = 0; mIndex < length; mIndex++) {
    sIndex = mIndex * 2;
    unsigned char b = ByteToAscii(&input[sIndex]);
    memcpy(&buffer[mIndex], &b, 1);
    printf("%i\n", b);
  }
  printf("xx %s == ", buffer);
//  *output = strdup(buffer);
  return 0;
}

#endif
