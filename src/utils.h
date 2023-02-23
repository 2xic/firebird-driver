#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <ctype.h>
//#include <string>

inline char *toLowerCase(char *string) {
    char *tptr = (char *)string;
    while(*tptr) {
        *tptr = tolower(*tptr);
        tptr++;
    }
    return string;
}

inline char* getAsHex(int *data, int length, int ashex) {
    char *results = (char*)malloc(sizeof(char) * (length * 2 + 1));
    for (int i = 0; i < length; i++)
    {
        sprintf(results + i * 2, "%02x", data[i]);
    }
    results[length * 2] = '\0';
    // Dump should not be returned here, or should be set on the object so it can be properly freed
    return results;
    /*
    printf("%i\n", length);
    char *output = (char*)malloc(sizeof(char) * (length + 1));
    strncpy(output, data, length);
    return output;
    */
    // TOOD: This should not be necerrary to do in two compuations
    /*
    std::string hex = results;
    int len = hex.length();
    std::string newString;
    int i=0;
    for(; i< len; i+=2)
    {
        std::string byte = hex.substr(i,2);
        char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
        newString.push_back(chr);

        printf("%i", (int)chr);
    }
    printf("\n");
    printf("%i %i\n", i / 2, length);
    exit(0);
    return (char*)newString.c_str();
    printf("%i\n", len);
    printf("%i\n", newString.length());
    exit(0);*/
}


#endif
